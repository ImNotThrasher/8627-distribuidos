#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <winsock2.h>

/**
 * Genera un nombre de usuario con la longitud especificada, alternando entre vocales y consonantes.
 * La longitud debe estar entre 5 y 15 caracteres.
 *
 * @param longitud La longitud deseada del nombre de usuario.
 * @return Un puntero a una cadena de caracteres que contiene el nombre de usuario generado.
 *         El llamador es responsable de liberar la memoria asignada.
 */
char *generarUsername(int longitud);

/**
 * Genera una contraseña con la longitud especificada, utilizando caracteres alfanuméricos (mayúsculas y minúsculas).
 * La longitud debe estar entre 8 y 50 caracteres.
 *
 * @param longitud La longitud deseada de la contraseña.
 * @return Un puntero a una cadena de caracteres que contiene la contraseña generada.
 *         El llamador es responsable de liberar la memoria asignada.
 */
char *generarPassword(int longitud);

#endif // SERVIDOR_H
