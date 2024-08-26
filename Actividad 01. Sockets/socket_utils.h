#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <winsock2.h>

#define IP_SERVIDOR "127.0.0.1" // Dirección IP del servidor (localhost)
#define PUERTO_SERVIDOR 8888    // Puerto del servidor

#define BUFFER_SIZE 1024 // Tamaño del búfer para recibir datos

/* UTILIDADES */
int inicializarWinsock(WSADATA *datosWinsock);
SOCKET crearSocket(void);
void configurarServidor(struct sockaddr_in *direccion, const char *ip, int puerto);
void manejarErrorYLimpiar(SOCKET socket, WSADATA *datosWinsock);

/* SERVIDOR */
int enlazarSocket(SOCKET socketServidor, struct sockaddr_in *direccionServidor);
int escucharConexionesEntrantes(SOCKET socketServidor);
int recibirDatos(SOCKET socketCliente, char *buffer, int bufferSize);

// Función para limpiar y cerrar el socket
int manejarConexionesEntrantes(SOCKET socketServidor);

/* CLIENTE */
int conectarAlServidor(SOCKET socketCliente, struct sockaddr_in *direccionServidor);
int enviarDatos(SOCKET socketCliente, const char *mensaje);

#endif // SOCKET_UTILS_H
