# Proyecto3SisOps-threads

### INTEGRANTES:
  * Cristian Camilo Tabares Perez.
  * Jean Paul Gonzalez Pedraza.

### COMPILAR EL PROGRAMA:
  * El proyecto cuenta con archivo makefile, por lo tanto, unicamente es necesario ejecutar los dos siguientes comandos:
  * Ejecutar en la terminal el comando make socket_server.
  * Ejecutar en la terminal el comando make socket_client.

### EJECUTAR EL PROGRAMA:
  * Para la ejecucion del programa, es necesario tener dos terminales abiertas, una para el servidor y otra para el cliente:
  * El servidor debe ser ejecutado primero, y el comando es ./socket_server.
  * El cliente se ejecuta despues, y el comando es ./socket_client.

### CONSIDERACIONES:
  * El proyecto cuenta con algunos comandos para ejecutar en el programa, pero funciona con muchos mas, son solo algunos ejemplos de prueba.
  * Para crear un contenedor, se necesita poner create seguido del nombre del contenedor y luego su imagen asi: create ejemplo ubuntu:20.04.
  * Para listar los contenedores, es necesario poner list containers, esto listara todos los contenedores.
  * Para detener un contenedor, es necesario poner stop seguido del nombre del contenedor asi: stop ejemplo.
  * Para eliminar un contenedor, es necesario poner delete seguido del nombre del contenedor asi: delete ejemplo
  * Para ejecutar cualquier otro comando que no es por defecto, se necesita poner el sudo y luego el resto del comando, por ejemplo: sudo docker images
