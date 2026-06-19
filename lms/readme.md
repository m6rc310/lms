# Autocompilador para "Leguaje Modular Simple" lmsc

Lo primero que debe hacerse para que un lenguaje de programación demuestre que sirve para algo es escribir el compilador
en el propio lenguaje y aqui está.

Hay que decir que esta versión una vez compilada (y haciendo un **strip**) ocupa 62KB, un 35% más que en **C**, pero gcc
es un programa muy optimizado y muy grande, y este compilador es bastante pequeño, y además no tiene dependencias que no
sean con los modulos **system** y **string** que proporcionan llamadas al sistema y manimulación de strings.
