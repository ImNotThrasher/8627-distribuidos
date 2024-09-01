:: Posiciona en la carpeta actual
:: %cd%

:: Compila los archivos .c usando GCC y genera el ejecutable 'servidor'
:: Nota: '-lws2_32' enlaza la biblioteca winsock para funciones de red en Windows
gcc servidor.c socket_utils.c -o servidor -lws2_32
:: Ejecuta el archivo generado 'servidor'
.\servidor

:: Pausa el script para evitar que la ventana de la consola se cierre inmediatamente
:: después de ejecutar el programa
:: pause

