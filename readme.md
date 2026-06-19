# Compilador para "Leguaje Modular Simple" lmsc

Es cierto que ya hay en el mundo muchos lenguajes de programación, pero no me he podido resistir a crear uno mas.
Este lenguaje es muy sencillo, no se utilizan Lex o Yacc ni estructuras de datos complejas para generar el binario,
practicamente se va creando el ensamblador (NASM) al vuelo, el codigo generado no es especialmente eficiente, pero el
compilador es sencillo y muy pequeñito (la version en C 46KB y el autocompilador en lms 62KB), como se ve se crea codigo
mas largo, esto puede deberse a que en las llamadas a función se cargan todos los parametros en la pila y que por ejemplo en los if...else se crean demasiadas etiquetas, además la única optimización implementada en el compilador es que si detecta que se crea un push rax seguida de un pop rax no se hace nada :-) .

El lenguaje es modular como se ha dicho antes, el nombre del modulo se indica al principio con **unit** y cada modulo puede tener
su propio main, las funciones se indican con **function** y solo existen los tipos de datos "primitivos", int8, int16, int32,
 int64, uint8, uint16, uint32, uint64 y pointer, la unica diferencia entre un int y un uint es a la de las conversiones (se
añaden ceros o extension de signo).

Los bloques de codigo se indican con **{}** y si se quieren nombrar se puede añadir **do nombre** delante, **continue** salta
al comienzo del bloque y **break** al final, pudiendo añadir el nombre del bloque.

Coloco aquí dos ejemplos de codigo para que se pueda ver la pinta que tiene y que es posible hacer alguna cosa con este
lenguaje, el primero es un ejemplo de como rescatar los parametros y el entorno del SO:

```
unit param;

import string (
  write:0:2,
  writeln:0:2,
  length:1:0
);

import system (
  int64 out
);


function main (int8 code;
  int64 parameters,pointer parameter,pointer enviroment) {
  pointer p,line;
  int16 i;

  p:=parameter;
  i:=0;
  do {
    string.writeln (system.out,p);
    p:=p+string.length (p)+1;
    i:=i+1;
    if (i<parameters) continue;
  }
  string.write (system.out,"ENV...\n");
  do {
    p:=enviroment[pointer];
    if (p=0) break;
    string.writeln (system.out,p);
    enviroment:=enviroment+8;
    continue;
  }
}
```

El segundo es un ejemplo con numeros complejos y suma/multiplicacion sobrecargada:

```
unit complex;

import system (
  int64 out
);

import string (
  float:1:0,
  copy:0:2,
  concat:0:2,
  write:0:2,
  writeln:0:2
);

pointer buffer="01234567890123456789012345678901234567890123456789";

function complex (float64 a.r,float64 a.i;) {
}

function add (float64 a.r,float64 a.i;float64 b.r,float64 b.i) {
  a.r:=a.r+b.r;
  a.i:=a.i+b.i;
}

function mul (float64 a.r,float64 a.i;float64 b.r,float64 b.i) {
  float64 r;
  
  r:=a.r*b.r-a.i*b.i;
  a.i:=a.i*b.r+b.i*a.r;
  a.r:=r;
}

function toString (float64 r;float64 i) {
  string.copy (buffer,"(");
  string.concat (buffer,string.float (r));
  string.concat (buffer,",");
  string.concat (buffer,string.float (i));
  string.concat (buffer,")");
  r:=buffer;
}

function main (int8 code;
  int64 parameters,pointer parameter,pointer enviroment) {
  float64 a.r=1.0,a.i=1.0,b.r=0.5,b.i=0.5,c.r,c.i;

  string.writeln (system.out,toString (a.r,a.i));
  string.writeln (system.out,toString (b.r,b.i));
  c.r,c.i:=complex (a.r,a.i)+[complex]complex (b.r,b.i);
  string.write (system.out,"Suma     : ");
  string.writeln (system.out,toString (c.r,c.i));
  c.r,c.i:=complex (a.r,a.i)*[complex]complex (b.r,b.i);
  string.write (system.out,"Producto : ");
  string.writeln (system.out,toString (c.r,c.i));
}
```

Como puede se ve en este ejemplo, los identificadores pueden incluir el "punto".

Pero para que pueda verse que se puede hacer algo un poco mas, he escrito un emulador de z80 que pasa los test de las
instrucciones documentadas y no documentadas de este procesador, y un modulo de spectrum que emula la pantalla y teclas
con lo que se puede ejecutar algun programa...