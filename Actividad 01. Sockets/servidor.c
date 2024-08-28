#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "servidor.h"
#include "socket_utils.h"

#define MIN_LONGITUD_USUARIO 5
#define MAX_LONGITUD_USUARIO 15
#define MIN_LONGITUD_CONTRASENA 8
#define MAX_LONGITUD_CONTRASENA 50

const char vocales[] = "aeiou";
const char consonantes[] = "bcdfghjklmnpqrstvwxyz";
const char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

/**
 * Genera un nombre de usuario aleatorio de longitud dada.
 *
 * El nombre de usuario se genera de la siguiente manera: se alternan
 * entre vocales y consonantes, y se elige una letra al azar de las
 * vocales o consonantes disponibles.
 *
 * @param longitud La longitud del nombre de usuario, debe estar entre
 *                 MIN_LONGITUD_USUARIO y MAX_LONGITUD_USUARIO.
 * @return Un puntero a un string que contiene el nombre de usuario
 *         generado.
 */
char *generarUsername(int longitud)
{
    if (longitud < MIN_LONGITUD_USUARIO || longitud > MAX_LONGITUD_USUARIO)
    {
        fprintf(stderr, "La longitud del nombre de usuario debe estar entre %d y %d.\n", MIN_LONGITUD_USUARIO, MAX_LONGITUD_USUARIO);
        exit(EXIT_FAILURE);
    }

    char *nombreUsuario = malloc(longitud + 1);
    int esVocal = rand() % 2; // 0 = consonante, 1 = vocal
    for (int i = 0; i < longitud; i++)
    {
        if (esVocal)
        {
            nombreUsuario[i] = vocales[rand() % strlen(vocales)];
        }
        else
        {
            nombreUsuario[i] = consonantes[rand() % strlen(consonantes)];
        }
        esVocal = !esVocal; // Alternar entre vocal y consonante
    }
    nombreUsuario[longitud] = '\0';
    return nombreUsuario;
}

/**
 * Genera una contraseña aleatoria de longitud dada.
 *
 * La contraseña se genera seleccionando de forma aleatoria
 * caracteres de la cadena caracteres.
 *
 * @param longitud La longitud de la contraseña, debe estar entre
 *                 MIN_LONGITUD_CONTRASENA y MAX_LONGITUD_CONTRASENA.
 * @return Un puntero a un string que contiene la contraseña
 *         generada.
 */
char *generarPassword(int longitud)
{
    if (longitud < MIN_LONGITUD_CONTRASENA || longitud > MAX_LONGITUD_CONTRASENA)
    {
        fprintf(stderr, "La longitud de la password debe estar entre %d y %d.\n", MIN_LONGITUD_CONTRASENA, MAX_LONGITUD_CONTRASENA);
        exit(EXIT_FAILURE);
    }

    char *contrasena = malloc(longitud + 1);
    for (int i = 0; i < longitud; i++)
    {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0';
    return contrasena;
}

/**
 * Maneja las conexiones entrantes y responde a los clientes.
 *
 * Acepta conexiones entrantes y lee los datos del cliente. Si el
 * mensaje del cliente comienza con "USER", se genera un nombre
 * de usuario y se envía al cliente. Si el mensaje del cliente
 * comienza con "PASS", se genera una contraseña y se envía al
 * cliente. De lo contrario, se envía un mensaje de error.
 *
 * @param socketServidor El socket del servidor que acepta
 *                        conexiones entrantes.
 * @return EXIT_SUCCESS si se manejó correctamente la conexión
 *         y la recepción de datos, EXIT_FAILURE en caso de error.
 */
int manejarConexionesEntrantes(SOCKET socketServidor)
{
    SOCKET socketCliente;
    char buffer[BUFFER_SIZE];
    int bytesRecibidos;

    int escuchando = 1;
    while (escuchando)
    {
        // Aceptar una conexión entrante
        socketCliente = accept(socketServidor, NULL, NULL);
        if (socketCliente == INVALID_SOCKET)
        {
            fprintf(stderr, "Error al aceptar la conexion: %d\n", WSAGetLastError());
            return EXIT_FAILURE;
        }
        else
        {
            printf("Conexion aceptada.\n");
        }

        // Recibir datos del cliente
        if (recibirDatos(socketCliente, buffer, sizeof(buffer)) == EXIT_SUCCESS)
        {
            int longitud;
            char *resultado;
            if (strncmp(buffer, "USER", 4) == 0)
            {
                longitud = atoi(buffer + 5);
                resultado = generarUsername(longitud);
                snprintf(buffer, sizeof(buffer), "Nombre de usuario generado: %s", resultado);
                free(resultado);
            }
            else if (strncmp(buffer, "PASS", 4) == 0)
            {
                longitud = atoi(buffer + 5);
                resultado = generarPassword(longitud);
                snprintf(buffer, sizeof(buffer), "Password generada: %s", resultado);
                free(resultado);
            }
            else
            {
                snprintf(buffer, sizeof(buffer), "Comando no reconocido.");
            }
        }

        // Enviar respuesta al cliente
        if (enviarDatos(socketCliente, buffer) != EXIT_SUCCESS)
        {
            fprintf(stderr, "Error al enviar datos al cliente.\n");
            break;
        }

        // Cerrar el socket del cliente
        closesocket(socketCliente);
    }

    return EXIT_SUCCESS;
}

/* SERVIDOR */
int main()
{
    WSADATA datosWinsock;
    SOCKET socketServidor;
    struct sockaddr_in direccionServidor;

    // Inicializar Winsock
    if (inicializarWinsock(&datosWinsock) != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    // Crear el socket
    if ((socketServidor = crearSocket()) == INVALID_SOCKET)
    {
        manejarErrorYLimpiar(socketServidor, &datosWinsock);
    }

    // Configurar la dirección del socket
    configurarServidor(&direccionServidor, IP_SERVIDOR, PUERTO_SERVIDOR);

    // Enlazar el socket
    if (enlazarSocket(socketServidor, &direccionServidor) != EXIT_SUCCESS)
    {
        manejarErrorYLimpiar(socketServidor, &datosWinsock);
    }

    // Escuchar conexiones entrantes
    if (escucharConexionesEntrantes(socketServidor) != EXIT_SUCCESS)
    {
        manejarErrorYLimpiar(socketServidor, &datosWinsock);
    }

    // Manejar las conexiones entrantes
    manejarConexionesEntrantes(socketServidor);

    // Limpiar y cerrar el socket
    closesocket(socketServidor);
    WSACleanup();

    return EXIT_SUCCESS;
}
