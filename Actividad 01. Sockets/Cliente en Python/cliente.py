import socket
import time

# Constantes

MIN_LONGITUD_USUARIO = 5
MAX_LONGITUD_USUARIO = 15
MIN_LONGITUD_CONTRASENA = 8
MAX_LONGITUD_CONTRASENA = 50

IP_SERVIDOR = "127.0.0.1"
PUERTO_SERVIDOR = 8888

BUFFER_SIZE = 1024

MAX_REINTENTOS = 5
REINTENTO_DELAY = 5  # en segundos

def mostrar_menu():
    """
    Muestra el menú al usuario y devuelve la opción ingresada como entero.

    :return: La opción ingresada por el usuario (1, 2 o 3).
    """

    print("Menú:")
    print("1. Generar nombre de usuario")
    print("2. Generar password")
    print("3. Salir")
    return int(input("Ingrese la opción deseada: "))

def enviar_datos(sock, mensaje):
    try:
        sock.sendall(mensaje.encode())
        return True
    except socket.error as e:
        print(f"Error al enviar datos: {e}")
        return False

def recibir_datos(sock):
    try:
        datos = sock.recv(BUFFER_SIZE).decode()
        if not datos:
            print("El servidor se desconectó.")
            return None
        return datos
    except socket.error as e:
        print(f"Error al recibir datos: {e}")
        return None

def intentar_reconexion(sock, direccion_servidor):
    
    """
    Intenta reconectar al servidor.

    :param socket sock: El socket a usar para la reconexión.
    :param tuple direccion_servidor: La dirección del servidor (IP, puerto).

    :return: True si se logra la reconexión, False en caso contrario.
    """

    intento = 0
    while intento < MAX_REINTENTOS:
        try:
            sock.connect(direccion_servidor)
            print("Reconexión exitosa.")
            return True
        except socket.error:
            print(f"Reintento de conexión en {REINTENTO_DELAY} segundos...")
            time.sleep(REINTENTO_DELAY)
            intento += 1
    return False

def procesar_opcion(opcion, sock, direccion_servidor):

    """
    Procesa la opción ingresada por el usuario.

    :param int opcion: La opción ingresada por el usuario (1, 2 o 3).
    :param socket sock: El socket para enviar los datos al servidor.
    :param tuple direccion_servidor: La dirección del servidor (IP, puerto).

    :return: None
    """
    
    if opcion == 1:
        longitud = int(input(f"Ingrese la longitud del nombre de usuario ({MIN_LONGITUD_USUARIO}-{MAX_LONGITUD_USUARIO}): "))
        mensaje = f"USER {longitud}"
    elif opcion == 2:
        longitud = int(input(f"Ingrese la longitud de la password ({MIN_LONGITUD_CONTRASENA}-{MAX_LONGITUD_CONTRASENA}): "))
        mensaje = f"PASS {longitud}"
    elif opcion == 3:
        print("Saliendo...")
        sock.close()
        exit(0)
    else:
        print("Opción no válida.")
        return

    if enviar_datos(sock, mensaje):
        respuesta = recibir_datos(sock)
        if respuesta:
            print(f"Respuesta del servidor: {respuesta}")
        else:
            print("Se perdió la conexión con el servidor. Intentando reconectar...")
            if intentar_reconexion(sock, direccion_servidor):
                print("Reconexión exitosa.")
            else:
                print("No se pudo reconectar al servidor. Saliendo...")
                sock.close()
                exit(1)

def main():

    """
    Punto de entrada principal del cliente.

    Establece una conexión con el servidor y luego entra en un bucle
    donde se muestra el menú al usuario y se procesa la opción ingresada.

    :return: None
    """

    direccion_servidor = (IP_SERVIDOR, PUERTO_SERVIDOR)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        direccion_servidor = (IP_SERVIDOR, PUERTO_SERVIDOR)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            sock.connect(direccion_servidor)
            print("Conexión establecida.\n")
        except socket.error as e:
            print(f"Fallo la conexión con el servidor: {e}")
            sock.close()
            return

        while True:
            opcion = mostrar_menu()
            print("\n")
            procesar_opcion(opcion, sock, direccion_servidor)
            print("\n")

    except Exception as e:
        print(f"Ocurrió un error inesperado: {e}")
    finally:
        sock.close()
        print("\n")
        input("Presiona Enter para salir...")

if __name__ == "__main__":
    main()
