#ifndef CONFIG_H
#define CONFIG_H

/* VARIABLES GLOBALES. */

/* Configuracionpara servidor y cliente. */
#define IP_SERVIDOR "127.0.0.1" // Dirección IP del servidor (localhost)
#define PUERTO_SERVIDOR 8888    // Puerto del servidor

#define BUFFER_SIZE 1024 // Tamaño del búfer para recibir datos

/* Variables para la reconexion del cliente.*/
#define MAX_REINTENTOS 5  // Número máximo de intentos de reconexión al servidor
#define REINTENTO_DELAY 5 // Tiempo en segundos entre intentos de reconexión

/* Variables para generar usuarios y passwords aleatorisos */
#define MIN_LONGITUD_USUARIO 5
#define MAX_LONGITUD_USUARIO 15
#define MIN_LONGITUD_CONTRASENA 8
#define MAX_LONGITUD_CONTRASENA 50

#endif // CONFIG_H
