#ifndef CLIENTE_H
#define CLIENTE_H

#include <winsock2.h>

// Muestra el menú de opciones al usuario.
// Las opciones incluyen generar un nombre de usuario, generar una contraseña, o salir.
void mostrarMenu(void);

// Procesa la opción seleccionada por el usuario en el menú.
// Dependiendo de la opción, envía una solicitud al servidor para generar un nombre de usuario o una contraseña.
// Parámetros:
// - opcion: La opción seleccionada por el usuario (1 para nombre de usuario, 2 para contraseña, 3 para salir).
// - socketCliente: El socket del cliente utilizado para comunicarse con el servidor.
// - direccionServidor: Estructura que contiene la dirección IP y el puerto del servidor.
// Retorno:
// - No retorna ningún valor.
void procesarOpcion(int opcion, SOCKET socketCliente, struct sockaddr_in *direccionServidor);

#endif // CLIENTE_H
