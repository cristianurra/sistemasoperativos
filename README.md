# Tarea 1 Sistemas operativos
https://github.com/cristianurra/sistemasoperativos
Tarea numero 1 Sistemas operativos
# Integrantes:
Ignacio Veragua 201604019-6

Cristhian Urra 201721076-1

# Compilacio

El programa se compilara automaticamente con sh install.sh


# Descripcion del programa

Se trabaja utilizando un minimo de ram, puesto que cada dato es introducido en la posix instantaneamente despues de haber sido creado. 
Los datos son almacenados fila por fila en la memoria posix.
Primero se tiene un int, por ejemplo 128, este dato es convertido en cadena de caracteres usando sprintf, quedando de la siguiente manera

128 -> [1,2,8,&#92;,n]
Lo cual es introducido en la posix, facilitando su lectura gracias a los saltos de linea. (&#92; corresponde a backslash, el cual no puede ser introducido directamente en un markdown
