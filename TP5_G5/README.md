# TP 5 - Grupo 5
Este TP consiste en armar un servidor y in cliente con el protocolo de transmisión TCP/IP y el protocolo de comunicación HTTP.

### Server:
_En el server se utilizo la libreria "boost" junto con "asio" para implementar TCP_
_Es un server asincrono y con posibilidad de tener varias conexiones a la vez_
* Si esta corriendo el server solo finaliza el programa cerrando la terminal
* Al server solo le pueden llegar peticiones HTTP o en su defecto mensajes que terminen con "\r\n\r\n"
* El server puede enviar los siguientes tipos de archivos: ".gif" - ".htm" - ".html" - ".jpg" - ".png" - ".css" - ".js" - ".rar" - ".7z" - ".json" - ".pdf" 

### Client:
_En el cliente se utilizo la libreria "libCurl" para poder implementar HTTP_
_Corriendo el programa se pueden hacer multiples peticiones_
* 1.Compilar el cliente y situarse dentro de la carpeta Debug 
* 2.Con power_shell (shift + click derecho + abrir ventana de powe shell aquí) ingresar comando: .\Client_G5.exe host/path/filename
* Ejemplo:	.\Client_G5.exe 127.0.0.1/index.html 127.0.0.1/pepe.txt