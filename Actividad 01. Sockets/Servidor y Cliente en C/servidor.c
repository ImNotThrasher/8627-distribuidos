#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#include "servidor.h"
#include "socket_utils.h"
#include "config.h"

const char vocales[] = "aeiou";
const char consonantes[] = "bcdfghjklmnpqrstvwxyz";
const char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// Estructura que contiene los parámetros necesarios para manejar una conexión con el cliente.
typedef struct
{
    SOCKET socketCliente;
} ClienteParam;

/**
 * Inicializa la biblioteca de números aleatorios con una semilla basada en el
 * tiempo actual. Debe llamarse a esta función antes de utilizar rand() o
 * srand() para generar números aleatorios.
 */
void inicializarAleatorio()
{
    srand(time(NULL)); // Establece la semilla basada en el tiempo actual
}

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
    inicializarAleatorio();
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
    inicializarAleatorio();
    if (longitud < MIN_LONGITUD_CONTRASENA || longitud > MAX_LONGITUD_CONTRASENA)
    {
        char mensajeError[BUFFER_SIZE];
        snprintf(mensajeError, sizeof(mensajeError), "Error: La longitud de la password debe estar entre %d y %d.\n", MIN_LONGITUD_CONTRASENA, MAX_LONGITUD_CONTRASENA);
        printf("Error en generarPassword: %s\n", mensajeError);
        enviarDatos(socketCliente, mensajeError);
        return NULL; // Retornar NULL para indicar un error y evitar continuar
    }

    char *password = malloc(longitud + 1);
    if (password == NULL)
    {
        fprintf(stderr, "Error al asignar memoria para la contraseña.\n");
        return NULL; // Manejar error de malloc
    }

    for (int i = 0; i < longitud; i++)
    {
        password[i] = caracteres[rand() % strlen(caracteres)];
    }
    password[longitud] = '\0';

    return password;
}

/**
 * Maneja la comunicación con un cliente conectado.
 *
 * Esta función se ejecuta en un hilo separado para cada cliente y se encarga
 * de recibir y procesar los datos enviados por el cliente. Si el mensaje del
 * cliente comienza con "USER", se genera un nombre de usuario basado en la
 * longitud especificada y se envía al cliente. Si el mensaje del cliente
 * comienza con "PASS", se genera una contraseña de longitud especificada y se
 * envía al cliente. Para cualquier otro mensaje, se envía un mensaje de error.
 *
 * @param lpParam Parámetro que contiene un puntero a una estructura `ClienteParam`,
 *                la cual incluye el socket del cliente que se debe manejar.
 * @return EXIT_SUCCESS si se procesaron correctamente los datos del cliente y se
 *         cerró la conexión, EXIT_FAILURE en caso de error.
 */
DWORD WINAPI manejarCliente(LPVOID lpParam)
{
    ClienteParam *param = (ClienteParam *)lpParam;
    SOCKET socketCliente = param->socketCliente;
    free(param);

    char buffer[BUFFER_SIZE];

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
            char *password = generarPassword(longitudPassword, socketCliente);
            if (password != NULL)
            {
                enviarDatos(socketCliente, password);
                free(password);
            }
        }
        else
        {
            enviarDatos(socketCliente, "Comando no reconocido.\n");
        }
    }

    // Cerrar la conexión con el cliente
    printf("Cerrando conexion con el cliente...\n");
    printf("\n");
    closesocket(socketCliente);
    return EXIT_SUCCESS;
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
    HANDLE hiloCliente;
    DWORD idHiloCliente;

    // Aceptar conexiones entrantes
    while ((socketCliente = accept(socketServidor, (struct sockaddr *)&direccionCliente, &clienteLen)) != INVALID_SOCKET)
    {
        printf("Cliente conectado.\n");

        // Crear un nuevo hilo para manejar la conexión del cliente
        ClienteParam *param = malloc(sizeof(ClienteParam));
        if (param == NULL)
        {
            fprintf(stderr, "Error al asignar memoria para el parámetro del cliente.\n");
            closesocket(socketCliente);
            continue;
        }
        param->socketCliente = socketCliente;

        hiloCliente = CreateThread(NULL, 0, manejarCliente, param, 0, &idHiloCliente);
        if (hiloCliente == NULL)
        {
            fprintf(stderr, "Error al crear el hilo del cliente. Código de Error: %d\n", GetLastError());
            closesocket(socketCliente);
            free(param);
        }
        else
        {
            // Cerrar el hilo cuando haya terminado
            CloseHandle(hiloCliente);
        }
    }
}

/* SERVIDOR */
int main()
{
    inicializarAleatorio();

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
