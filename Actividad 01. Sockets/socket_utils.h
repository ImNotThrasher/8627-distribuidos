#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <winsock2.h>

#define IP_SERVIDOR "127.0.0.1" // Dirección IP del servidor (localhost)
#define PUERTO_SERVIDOR 8888    // Puerto del servidor

#define BUFFER_SIZE 1024 // Tamaño del búfer para recibir datos

// Función para inicializar Winsock
// Parámetros:
// - datosWinsock: puntero a una estructura WSADATA donde se almacenarán los datos de la inicialización.
// Retorno:
// - EXIT_SUCCESS si Winsock se inicializó correctamente.
// - EXIT_FAILURE si ocurrió un error al inicializar Winsock.
int inicializarWinsock(WSADATA *datosWinsock);

// Función para crear un socket
// Retorno:
// - El descriptor del socket creado si la operación fue exitosa.
// - INVALID_SOCKET si ocurrió un error al crear el socket.
SOCKET crearSocket(void);

// Función para configurar la estructura sockaddr_in con dirección IP y puerto.
// Parámetros:
// - direccionServidor: puntero a la estructura sockaddr_in que se configurará.
// - ip: dirección IP en formato de cadena (ej. "127.0.0.1").
// - puerto: número de puerto al que se configurará el socket.
// Retorno:
// - Ninguno.
void configurarServidor(struct sockaddr_in *direccionServidor, const char *ip, int puerto);

// Función para limpiar y cerrar el socket en caso de error.
// Parámetros:
// - socket: el descriptor del socket que se cerrará.
// - datosWinsock: puntero a la estructura WSADATA para limpiar Winsock.
// Retorno:
// - Ninguno.
void manejarErrorYLimpiar(SOCKET socket, WSADATA *datosWinsock);

/* SERVIDOR */

// Función para enlazar el socket con una dirección IP y un puerto específicos.
// Parámetros:
// - socketServidor: El socket del servidor que se va a enlazar.
// - direccionServidor: Estructura que contiene la dirección IP y el puerto a los que se enlazará el socket.
// Retorno:
// - EXIT_SUCCESS si el enlace se realizó correctamente.
// - EXIT_FAILURE si ocurrió un error durante el enlace.
int enlazarSocket(SOCKET socketServidor, struct sockaddr_in *direccionServidor);

// Función para poner el socket en modo de escucha para aceptar conexiones entrantes.
// Parámetros:
// - socketServidor: El socket del servidor que se pondrá en modo de escucha.
// Retorno:
// - EXIT_SUCCESS si el socket está en modo de escucha correctamente.
// - EXIT_FAILURE si ocurrió un error al poner el socket en modo de escucha.
int escucharConexionesEntrantes(SOCKET socketServidor);

// Función para recibir datos del cliente a través de un socket.
// Parámetros:
// - socketCliente: El socket del cliente desde el que se recibirán los datos.
// - buffer: El búfer donde se almacenarán los datos recibidos.
// - bufferSize: El tamaño máximo del búfer.
// Retorno:
// - EXIT_SUCCESS si los datos se recibieron correctamente.
// - EXIT_FAILURE si ocurrió un error durante la recepción de datos o si el cliente se desconectó.
int recibirDatos(SOCKET socketCliente, char *buffer, int bufferSize);

// Función para manejar las conexiones entrantes y recibir datos de los clientes.
// Parámetros:
// - socketServidor: El socket del servidor que acepta nuevas conexiones de clientes.
// Retorno:
// - EXIT_SUCCESS si se manejó correctamente la conexión y la recepción de datos.
// - EXIT_FAILURE si ocurrió un error al aceptar la conexión o al recibir datos.
int manejarConexionesEntrantes(SOCKET socketServidor);

/* CLIENTE */

// Función para conectar al servidor
// Parámetros:
// - socketCliente: El socket del cliente que se conectará al servidor.
// - direccionServidor: Estructura que contiene la dirección IP y el puerto del servidor al que se conectará el socket.
// Retorno:
// - EXIT_SUCCESS si la conexión se estableció correctamente.
// - EXIT_FAILURE si ocurrió un error al intentar conectar al servidor.
int conectarAlServidor(SOCKET socketCliente, struct sockaddr_in *direccionServidor);

// Función para enviar datos al servidor
// Parámetros:
// - socketCliente: El socket del cliente desde el cual se enviarán los datos.
// - mensaje: El mensaje que se enviará al servidor, debe ser una cadena de caracteres terminada en NULL.
// Retorno:
// - EXIT_SUCCESS si los datos se enviaron correctamente.
// - EXIT_FAILURE si ocurrió un error al intentar enviar los datos.
int enviarDatos(SOCKET socketCliente, const char *mensaje);

#endif // SOCKET_UTILS_H
