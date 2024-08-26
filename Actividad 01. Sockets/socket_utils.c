#include "socket_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Función para inicializar Winsock
int inicializarWinsock(WSADATA *datosWinsock)
{
    if (WSAStartup(MAKEWORD(2, 2), datosWinsock) != 0)
    {
        fprintf(stderr, "Error al inicializar Winsock. Código de Error: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }
    printf("Winsock inicializado.\n");
    return EXIT_SUCCESS;
}

// Función para crear un socket
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

// Función para configurar la estructura sockaddr_in
void configurarServidor(struct sockaddr_in *direccionServidor, const char *ip, int puerto)
{
    direccionServidor->sin_family = AF_INET;
    direccionServidor->sin_addr.s_addr = inet_addr(ip);
    direccionServidor->sin_port = htons(puerto);
}

// Función para limpiar y cerrar el socket
void manejarErrorYLimpiar(SOCKET socket, WSADATA *datosWinsock)
{
    if (socket != INVALID_SOCKET)
        closesocket(socket);

    WSACleanup();
    exit(EXIT_FAILURE);
}

/* SERVIDOR */

// Función para enlazar el socket
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

// Función para escuchar conexiones entrantes
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
    return EXIT_SUCCESS;
}

// Función para recibir datos del cliente
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

// Función para manejar las conexiones entrantes
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
            fprintf(stderr, "Error al aceptar la conexión: %d\n", WSAGetLastError());
            return EXIT_FAILURE;
        }
        else
        {
            printf("Conexión aceptada.\n");
        }

        // Recibir datos del cliente
        if (recibirDatos(socketCliente, buffer, sizeof(buffer)) == EXIT_SUCCESS)
        {
            printf("Mensaje recibido del cliente: %s\n", buffer);
        }

        // Terminar el mensaje recibido con NULL para imprimirlo correctamente
        buffer[bytesRecibidos] = '\0';
        printf("Mensaje recibido del cliente: %s\n", buffer);

        // Cerrar el socket del cliente
        closesocket(socketCliente);
        return EXIT_SUCCESS;
    }
}

/* CLIENTE */

// Función para conectar al servidor
int conectarAlServidor(SOCKET socketCliente, struct sockaddr_in *direccionServidor)
{
    if (connect(socketCliente, (struct sockaddr *)direccionServidor, sizeof(*direccionServidor)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Falló la conexión con el servidor. Código de Error: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }
    printf("Conexión establecida.\n");
    return EXIT_SUCCESS;
}

// Función para enviar datos al servidor
int enviarDatos(SOCKET socketCliente, const char *mensaje)
{
    if (send(socketCliente, mensaje, sizeof(mensaje), 0) == SOCKET_ERROR)
    {
        fprintf(stderr, "Error al enviar datos. Código de Error: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }

    printf("Mensaje enviado: %s\n", mensaje);
    return EXIT_SUCCESS;
}