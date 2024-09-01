:: Posiciona en la carpeta actual
:: %cd%

:: Compila los archivos .c usando GCC y genera el ejecutable 'cliente'
:: Nota: '-lws2_32' enlaza la biblioteca winsock para funciones de red en Windows
gcc cliente.c socket_utils.c -o cliente -lws2_32

:: Ejecuta el archivo generado 'cliente'
.\cliente

:: Pausa el script para evitar que la ventana de la consola se cierre inmediatamente
:: después de ejecutar el programa
:: pause
