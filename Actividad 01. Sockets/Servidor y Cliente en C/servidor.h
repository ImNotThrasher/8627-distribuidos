#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <winsock2.h>

/**
 * Genera un nombre de usuario aleatorio basado en una longitud especificada.
 *
 * El nombre se crea alternando entre vocales y consonantes, seleccionando
 * letras al azar dentro de los conjuntos disponibles.
 *
 * @param longitud La longitud deseada del nombre de usuario, que debe estar
 *                 entre MIN_LONGITUD_USUARIO y MAX_LONGITUD_USUARIO.
 * @return Un puntero a un string con el nombre de usuario generado, o NULL
 *         en caso de error.
 */
char *generarUsername(int longitud, SOCKET socketCliente);
/**
 * Genera una contraseña aleatoria de longitud especificada.
 *
 * La contraseña se compone de caracteres seleccionados al azar de un conjunto
 * predefinido.
 *
 * @param longitud La longitud deseada de la contraseña, que debe estar entre
 *                 MIN_LONGITUD_CONTRASENA y MAX_LONGITUD_CONTRASENA.
 * @return Un puntero a un string con la contraseña generada, o NULL en caso de error.
 */
char *generarPassword(int longitud, SOCKET socketCliente);

#endif // SERVIDOR_H
