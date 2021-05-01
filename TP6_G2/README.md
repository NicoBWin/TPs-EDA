# TP 6 - Grupo 2
Este TP consiste en armar 3 clases refiriendose a displays, una por cada participante del grupo y utilizar los conceptos de herencia y polimorfismo de OOP para poder modificar en tiempo de ejecucion el display que estamos usando ya que de la clase padre "basicLCD" heredamos todas las funciones virtuales puras para que cada integrate resuelva el problema como le parezca  

## Integrantes:
* Nicolás Bustelo
* Juan Ignacio Causse
* Luciano Castrogiovanni

### Funcionamiento:
#### En tiempo de ejecución uno puede cambiar el display con el que esta operando, hay una serie de teclas especiales para poder probar el programa que se detallan a continuación
-----------------------------------
Salir del programa
* ESCAPE KEY 
-----------------------------------
Asignación del display segun alumnos
* N o 1 -> Cambia el display que se usa al de Nicolás
* L o 2 -> Cambia el display que se usa al de Luciano
* J o 3 -> Cambia el display que se usa al de Juán
-----------------------------------
Prueba de operators
* A -> Prueba del operador asignacion con chars mostrando un  "hola" ingresado como  *LCD << 'H' << 'o' << 'l' << 'a'
* S -> Prueba del operador asignacion con *chars como ' *LCD << test ' 
* D -> Prueba del operador asignacion con *chars pero con un mensaje largo
-----------------------------------
Borrado de pantalla
* C -> Borra toda la pantalla
* X -> Borra hasta EOL
-----------------------------------
Movimiento del cursor
* LEFT KEY -> Izquierda
* RIGHT KEY -> Derecha
* UP KEY -> Arriba
* DOWN KEY -> Abajo
-----------------------------------
Teclas para settear o saber la position dle cursor
* P -> Nos muestra en consola la posición actual del cursor
* O -> Configura una posicion especifica (1,5)
-----------------------------------
