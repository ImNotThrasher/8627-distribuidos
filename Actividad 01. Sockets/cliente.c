#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "cliente.h"
#include "socket_utils.h"
#include "config.h"

/**
 * Muestra el menú principal al usuario.
 *
 * Imprime un menú con las opciones disponibles y pide al usuario
 * que ingrese la opción deseada.
 */
void mostrarMenu(void)
{
    printf("Menu:\n");
    printf("1. Generar nombre de usuario\n");
    printf("2. Generar password\n");
    printf("3. Salir\n");
    printf("Ingrese la opcion deseada: ");
}

/**
 * Procesa la opción ingresada por el usuario y envía una solicitud al servidor.
 *
 * Según la opción ingresada, se pide al usuario que ingrese la longitud del
 * nombre de usuario o la contraseña y se envía al servidor la solicitud
 * correspondiente.
 *
 * @param opcion La opción ingresada por el usuario.
 * @param socketCliente El socket del cliente que se utiliza para enviar la solicitud.
 */
void procesarOpcion(int opcion, SOCKET socketCliente)
{
    char buffer[BUFFER_SIZE];
    int longitud;

    switch (opcion)
    {
    case 1:
        printf("Ingrese la longitud del nombre de usuario (%d-%d): ", MIN_LONGITUD_USUARIO, MAX_LONGITUD_USUARIO);
        scanf("%d", &longitud);
        snprintf(buffer, sizeof(buffer), "USER %d", longitud);
        break;
    case 2:
        printf("Ingrese la longitud de la password (%d-%d): ", MIN_LONGITUD_CONTRASENA, MAX_LONGITUD_CONTRASENA);
        scanf("%d", &longitud);
        snprintf(buffer, sizeof(buffer), "PASS %d", longitud);
        break;
    case 3:
        printf("Saliendo...\n");
        exit(0);
    default:
        printf("Opcion no valida.\n");
        return;
    }

    if (enviarDatos(socketCliente, buffer) == EXIT_SUCCESS)
    {
        if (recibirDatos(socketCliente, buffer, sizeof(buffer)) == EXIT_SUCCESS)
        {
            printf("Respuesta del servidor: %s\n", buffer);
        }
        printf("\n");
    }
}

/* CLIENTE */
int main()
{
    WSADATA datosWinsock;
    SOCKET socketCliente;
    struct sockaddr_in direccionServidor;
    int opcion;
    char mensaje[BUFFER_SIZE]; // Tamaño del búfer para el mensaje del usuario

    // Solicitar mensaje al usuario
    /*
    printf("Ingrese el mensaje a enviar: ");
    if (fgets(mensaje, sizeof(mensaje), stdin) == NULL)
    {
        fprintf(stderr, "Error al leer el mensaje.\n");
        return EXIT_FAILURE;
    }
    */

    // Eliminar el carácter de nueva línea si existe
    size_t len = strlen(mensaje);
    if (len > 0 && mensaje[len - 1] == '\n')
    {
        mensaje[len - 1] = '\0';
    }

    // Inicializar Winsock
    if (inicializarWinsock(&datosWinsock) != 0)
    {
        return EXIT_FAILURE;
    }

    // Crear el socket
    if ((socketCliente = crearSocket()) == INVALID_SOCKET)
    {
        manejarErrorYLimpiar(socketCliente, &datosWinsock);
    }

    // Configurar la dirección del socket
    configurarServidor(&direccionServidor, IP_SERVIDOR, PUERTO_SERVIDOR);

    // Conectar al servidor
    if (conectarAlServidor(socketCliente, &direccionServidor) != EXIT_SUCCESS)
    {
        manejarErrorYLimpiar(socketCliente, &datosWinsock);
    }

    // Enviar datos al servidor
    /*
    if (enviarDatos(socketCliente, mensaje) != EXIT_SUCCESS)
        manejarErrorYLimpiar(socketCliente, &datosWinsock);
    */

    while (1)
    {
        mostrarMenu();
        scanf("%d", &opcion);
        printf("\n");
        procesarOpcion(opcion, socketCliente);
    }

    // Limpiar y cerrar el socket
    closesocket(socketCliente);
    WSACleanup();

    return EXIT_SUCCESS;
}
