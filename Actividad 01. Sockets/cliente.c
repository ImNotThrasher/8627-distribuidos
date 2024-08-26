#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "socket_utils.h"

// Cliente
int main()
{
    WSADATA datosWinsock;
    SOCKET socketCliente;
    struct sockaddr_in direccionServidor;
    char mensaje[1024]; // Tamaño del búfer para el mensaje del usuario

    // Solicitar mensaje al usuario
    printf("Ingrese el mensaje a enviar: ");
    if (fgets(mensaje, sizeof(mensaje), stdin) == NULL)
    {
        fprintf(stderr, "Error al leer el mensaje.\n");
        return EXIT_FAILURE;
    }

    // Eliminar el carácter de nueva línea si existe
    size_t len = strlen(mensaje);
    if (len > 0 && mensaje[len - 1] == '\n')
        mensaje[len - 1] = '\0';

    // Inicializar Winsock
    if (inicializarWinsock(&datosWinsock) != 0)
        return EXIT_FAILURE;

    // Crear el socket
    if ((socketCliente = crearSocket()) == INVALID_SOCKET)
        manejarErrorYLimpiar(socketCliente, &datosWinsock);

    // Configurar la dirección del socket
    configurarServidor(&direccionServidor, IP_SERVIDOR, PUERTO_SERVIDOR);

    // Conectar al servidor
    if (conectarAlServidor(socketCliente, &direccionServidor) != EXIT_SUCCESS)
        manejarErrorYLimpiar(socketCliente, &datosWinsock);

    // Enviar datos al servidor
    if (enviarDatos(socketCliente, mensaje) != EXIT_SUCCESS)
        manejarErrorYLimpiar(socketCliente, &datosWinsock);

    // Limpiar y cerrar el socket
    closesocket(socketCliente);
    WSACleanup();

    return EXIT_SUCCESS;
}
