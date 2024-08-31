#include "socket_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* UTILIDADES SOCKETS */

/**
 * Inicializa la biblioteca Winsock 2.2.
 *
 * @param[out] datosWinsock Puntero a una estructura WSADATA donde se almacenarán los datos de la
 *                          inicialización.
 * @return EXIT_SUCCESS si Winsock se inicializó correctamente, EXIT_FAILURE si ocurrió un error al
 *         inicializar Winsock.
 */
int inicializarWinsock(WSADATA *datosWinsock)
{
    if (WSAStartup(MAKEWORD(2, 2), datosWinsock) != 0)
    {
        fprintf(stderr, "Error al inicializar Winsock. Codigo de Error: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }
    printf("Winsock inicializado.\n");
    return EXIT_SUCCESS;
}

/**
 * Crea un socket para el servidor.
 *
 * @return El descriptor del socket creado si la operación fue exitosa,
 *         INVALID_SOCKET si ocurrió un error al crear el socket.
 */
SOCKET crearSocket(void)
{
    SOCKET socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServidor == INVALID_SOCKET)
    {
        fprintf(stderr, "Error al crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Socket creado.\n");
    return socketServidor;
}

/**
 * Configura la estructura sockaddr_in con la dirección IP y el puerto dados.
 *
 * @param[in] direccionServidor Estructura que se configurará con la dirección IP y el puerto.
 * @param[in] ip Dirección IP en formato de cadena (por ejemplo, "127.0.0.1").
 * @param[in] puerto Número de puerto al que se configurará el socket.
 */
void configurarServidor(struct sockaddr_in *direccionServidor, const char *ip, int puerto)
{
    direccionServidor->sin_family = AF_INET;
    direccionServidor->sin_addr.s_addr = inet_addr(ip);
    direccionServidor->sin_port = htons(puerto);
}

/**
 * Maneja un error en la creación o configuración de un socket,
 * limpiando recursos y terminando el programa.
 *
 * @param[in] socket descriptor del socket que se cerrará.
 * @param[in] datosWinsock puntero a la estructura WSADATA para
 *                         limpiar Winsock.
 */
void manejarErrorYLimpiar(SOCKET socket, WSADATA *datosWinsock)
{
    if (socket != INVALID_SOCKET)
        closesocket(socket);

    WSACleanup();
    exit(EXIT_FAILURE);
}

/* SERVIDOR */

/**
 * Enlaza un socket con una dirección IP y un puerto específicos.
 * @param[in] socketServidor descriptor del socket que se va a enlazar.
 * @param[in] direccionServidor estructura que contiene la dirección IP y el puerto a los que se
 *                              enlazará el socket.
 * @return EXIT_SUCCESS si el enlace se realizó correctamente, EXIT_FAILURE en caso de error.
 */

int enlazarSocket(SOCKET socketServidor, struct sockaddr_in *direccionServidor)
{
    if (bind(socketServidor, (struct sockaddr *)direccionServidor, sizeof(*direccionServidor)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Error en el enlace: %d\n", WSAGetLastError());
        closesocket(socketServidor);
        WSACleanup();
        return EXIT_FAILURE;
    }
    printf("Enlace realizado.\n");
    return EXIT_SUCCESS;
}

/**
 * Escucha conexiones entrantes en un socket.
 *
 * @param[in] socketServidor descriptor del socket que se va a escuchar.
 * @return EXIT_SUCCESS si se estableció correctamente el socket en modo de
 *         escucha, EXIT_FAILURE si ocurrió un error.
 */
int escucharConexionesEntrantes(SOCKET socketServidor)
{
    if (listen(socketServidor, 5) == SOCKET_ERROR)
    {
        fprintf(stderr, "Error al escuchar conexiones: %d\n", WSAGetLastError());
        closesocket(socketServidor);
        WSACleanup();
        return EXIT_FAILURE;
    }
    printf("Escuchando conexiones entrantes...\n");
    printf("\n");

    return EXIT_SUCCESS;
}

/**
 * Recibe datos del cliente a través de un socket.
 *
 * @param socketCliente El socket del cliente desde el que se recibirán los datos.
 * @param buffer El búfer donde se almacenarán los datos recibidos.
 * @param bufferSize El tamaño del búfer.
 * @return EXIT_SUCCESS si los datos se recibieron correctamente o EXIT_FAILURE
 *         si ocurrió un error durante la recepción de datos o si el cliente se
 *         desconectó.
 */
int recibirDatos(SOCKET socketCliente, char *buffer, int bufferSize)
{
    int bytesRecibidos = recv(socketCliente, buffer, bufferSize - 1, 0);
    if (bytesRecibidos == SOCKET_ERROR)
    {
        fprintf(stderr, "Error al recibir datos: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }
    else if (bytesRecibidos == 0)
    {
        printf("Cliente desconectado.\n");
        return EXIT_FAILURE;
    }
    buffer[bytesRecibidos] = '\0'; // Asegurarse de que el buffer esté terminado en NULL
    return EXIT_SUCCESS;
}

/* CLIENTE */

/**
 * Conecta un socket a un servidor.
 * @param[in] socketCliente descriptor del socket que se va a conectar.
 * @param[in] direccionServidor estructura que contiene la dirección IP y el puerto del servidor al que se va a conectar.
 * @return EXIT_SUCCESS si la conexión se estableció correctamente, EXIT_FAILURE en caso de error.
 */
int conectarAlServidor(SOCKET socketCliente, struct sockaddr_in *direccionServidor)
{
    if (connect(socketCliente, (struct sockaddr *)direccionServidor, sizeof(*direccionServidor)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Fallo la conexion con el servidor. Codigo de Error: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }
    printf("Conexion establecida.\n");
    printf("\n");
    return EXIT_SUCCESS;
}

/**
 * Intenta enviar el mensaje especificado a través del socket del cliente
 * y devuelve EXIT_SUCCESS si se envió correctamente o EXIT_FAILURE
 * si ocurrió un error.
 *
 * @param socketCliente El socket del cliente que se utilizará para enviar
 *                      el mensaje.
 * @param mensaje       El mensaje que se enviará al servidor, debe ser una
 *                      cadena de caracteres terminada en NULL.
 * @return              EXIT_SUCCESS si el mensaje se envió correctamente,
 *                      EXIT_FAILURE si ocurrió un error.
 */
int enviarDatos(SOCKET socketCliente, const char *mensaje)
{
    // Intentar enviar el mensaje al servidor a través del socket del cliente
    // Nota: No usar sizeof, devuelve el tamaño del puntero, no el tamaño de la cadena...
    int mensajeLongitud = strlen(mensaje);
    int bytesEnviados = send(socketCliente, mensaje, mensajeLongitud, 0);
    if (bytesEnviados == SOCKET_ERROR)
    {
        fprintf(stderr, "Error al enviar datos. Codigo de Error: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }
    else if (bytesEnviados < mensajeLongitud)
    {
        fprintf(stderr, "Advertencia: No se enviaron todos los datos.\n");
        return EXIT_FAILURE;
    }

    // printf("Mensaje enviado: %s\n", mensaje);
    return EXIT_SUCCESS;
}