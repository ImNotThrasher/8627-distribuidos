#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "socket_utils.h"

// Servidor
int main()
{
    WSADATA datosWinsock;
    SOCKET socketServidor;
    struct sockaddr_in direccionServidor;

    // Inicializar Winsock
    if (inicializarWinsock(&datosWinsock) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Crear el socket
    if ((socketServidor = crearSocket()) == INVALID_SOCKET)
        manejarErrorYLimpiar(socketServidor, &datosWinsock);

    // Configurar la dirección del socket
    configurarServidor(&direccionServidor, IP_SERVIDOR, PUERTO_SERVIDOR);

    // Enlazar el socket
    if (enlazarSocket(socketServidor, &direccionServidor) != EXIT_SUCCESS)
        manejarErrorYLimpiar(socketServidor, &datosWinsock);

    // Escuchar conexiones entrantes
    if (escucharConexionesEntrantes(socketServidor) != EXIT_SUCCESS)
        manejarErrorYLimpiar(socketServidor, &datosWinsock);

    // Manejar las conexiones entrantes
    manejarConexionesEntrantes(socketServidor);

    // Limpiar y cerrar el socket
    closesocket(socketServidor);
    WSACleanup();

    return EXIT_SUCCESS;
}
