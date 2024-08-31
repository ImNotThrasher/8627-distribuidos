#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "servidor.h"
#include "socket_utils.h"

#define MIN_LONGITUD_USUARIO 5
#define MAX_LONGITUD_USUARIO 15
#define MIN_LONGITUD_CONTRASENA 8
#define MAX_LONGITUD_CONTRASENA 50

const char vocales[] = "aeiou";
const char consonantes[] = "bcdfghjklmnpqrstvwxyz";
const char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

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
char *generarUsername(int longitud, SOCKET socketCliente)
{
    if (longitud < MIN_LONGITUD_USUARIO || longitud > MAX_LONGITUD_USUARIO)
    {
        char mensajeError[BUFFER_SIZE];
        snprintf(mensajeError, sizeof(mensajeError), "Error: La longitud del nombre de usuario debe estar entre %d y %d.\n", MIN_LONGITUD_USUARIO, MAX_LONGITUD_USUARIO);
        printf("Error en generarUsername: %s\n", mensajeError);
        enviarDatos(socketCliente, mensajeError);
        return NULL; // Retornar NULL para indicar un error y evitar continuar
    }

    char *nombreUsuario = malloc(longitud + 1);
    if (nombreUsuario == NULL)
        return NULL; // Manejar error de malloc

    int esVocal = rand() % 2; // 0 = consonante, 1 = vocal
    for (int i = 0; i < longitud; i++)
    {
        if (esVocal)
        {
            nombreUsuario[i] = vocales[rand() % strlen(vocales)];
        }
        else
        {
            nombreUsuario[i] = consonantes[rand() % strlen(consonantes)];
        }
        esVocal = !esVocal; // Alternar entre vocal y consonante
    }
    nombreUsuario[longitud] = '\0';
    return nombreUsuario;
}

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
char *generarPassword(int longitud, SOCKET socketCliente)
{
    if (longitud < MIN_LONGITUD_CONTRASENA || longitud > MAX_LONGITUD_CONTRASENA)
    {
        char mensajeError[BUFFER_SIZE];
        snprintf(mensajeError, sizeof(mensajeError), "Error: La longitud de la password debe estar entre %d y %d.\n", MIN_LONGITUD_CONTRASENA, MAX_LONGITUD_CONTRASENA);
        printf("Error en generarPassword: %s\n", mensajeError);
        enviarDatos(socketCliente, mensajeError);
        return NULL; // Retornar NULL para indicar un error y evitar continuar
    }

    char *contrasena = malloc(longitud + 1);
    if (contrasena == NULL)
    {
        fprintf(stderr, "Error al asignar memoria para la contraseña.\n");
        return NULL; // Manejar error de malloc
    }

    for (int i = 0; i < longitud; i++)
    {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0';

    return contrasena;
}

/**
 * Maneja las conexiones entrantes y responde a los clientes.
 *
 * Acepta conexiones entrantes y lee los datos del cliente. Si el
 * mensaje del cliente comienza con "USER", se genera un nombre
 * de usuario y se envía al cliente. Si el mensaje del cliente
 * comienza con "PASS", se genera una contraseña y se envía al
 * cliente. De lo contrario, se envía un mensaje de error.
 *
 * @param socketServidor El socket del servidor que acepta
 *                        conexiones entrantes.
 * @return EXIT_SUCCESS si se manejó correctamente la conexión
 *         y la recepción de datos, EXIT_FAILURE en caso de error.
 */
int manejarConexionesEntrantes(SOCKET socketServidor)
{
    SOCKET socketCliente;
    struct sockaddr_in direccionCliente;
    int clienteLen = sizeof(direccionCliente);
    char buffer[BUFFER_SIZE];

    // Aceptar conexiones entrantes
    while ((socketCliente = accept(socketServidor, (struct sockaddr *)&direccionCliente, &clienteLen)) != INVALID_SOCKET)
    {
        printf("Cliente conectado.\n");

        // Recibir datos del cliente
        while (recibirDatos(socketCliente, buffer, BUFFER_SIZE) == EXIT_SUCCESS)
        {
            printf("Datos recibidos (User/Pass Longitud): %s\n", buffer);

            // Validar y procesar datos
            if (strncmp(buffer, "USER", 4) == 0)
            {
                int longitudUsuario = atoi(buffer + 5);
                char *usuario = generarUsername(longitudUsuario, socketCliente);
                if (usuario != NULL)
                {
                    enviarDatos(socketCliente, usuario);
                    free(usuario);
                }
            }
            else if (strncmp(buffer, "PASS", 4) == 0)
            {
                int longitudPassword = atoi(buffer + 5);
                char *contrasena = generarPassword(longitudPassword, socketCliente);
                if (contrasena != NULL)
                {
                    enviarDatos(socketCliente, contrasena);
                    free(contrasena);
                }
            }
            else
            {
                enviarDatos(socketCliente, "Comando no reconocido.\n");
            }
        }

        // Cerrar la conexión con el cliente
        printf("Cerrando conexion con el cliente.\n");
        closesocket(socketCliente);
    }

    return EXIT_SUCCESS;
}

/* SERVIDOR */
int main()
{
    WSADATA datosWinsock;
    SOCKET socketServidor;
    struct sockaddr_in direccionServidor;

    // Inicializar Winsock
    if (inicializarWinsock(&datosWinsock) != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    // Crear el socket
    if ((socketServidor = crearSocket()) == INVALID_SOCKET)
    {
        manejarErrorYLimpiar(socketServidor, &datosWinsock);
    }

    // Configurar la dirección del socket
    configurarServidor(&direccionServidor, IP_SERVIDOR, PUERTO_SERVIDOR);

    // Enlazar el socket
    if (enlazarSocket(socketServidor, &direccionServidor) != EXIT_SUCCESS)
    {
        manejarErrorYLimpiar(socketServidor, &datosWinsock);
    }

    // Escuchar conexiones entrantes
    if (escucharConexionesEntrantes(socketServidor) != EXIT_SUCCESS)
    {
        manejarErrorYLimpiar(socketServidor, &datosWinsock);
    }

    // Manejar las conexiones entrantes
    manejarConexionesEntrantes(socketServidor);

    // Limpiar y cerrar el socket
    closesocket(socketServidor);
    WSACleanup();

    return EXIT_SUCCESS;
}
