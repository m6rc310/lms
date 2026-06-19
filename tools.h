/*
**	Definiciones de funciones utiles para lmsc
**
*/
/*
    Este codigo es parte de "lmsc"
    Copyright (C) 2026  Marcelo Reina Aguilar

    Este programa es software libre: usted puede redistribuirlo y/o modificarlo
    bajo los términos de la Licencia Pública General de GNU publicada por
    la Fundación para el Software Libre, ya sea la versión 3 de la Licencia,
    o (a su elección) cualquier versión posterior.

    Este programa se distribuye con la esperanza de que sea útil, pero
    SIN NINGUNA GARANTÍA; ni siquiera la garantía implícita de
    COMERCIABILIDAD o IDONEIDAD PARA UN FIN DETERMINADO. Consulte la
    Licencia Pública General de GNU para más detalles.

    Debería haber recibido una copia de la Licencia Pública General de GNU
    junto con este programa. En caso contrario, consulte <https://www.gnu.org/licenses/>.
*/

int64 system_write (int64,void *,int64);
int64 system_read (int64,void *,int64);
int64 system_open (void *,int64,int64);
int64 system_close (int64);
int64 system_exit (int64);
int8 *string_hexadecimal (int64);
int8 *string_hex (int64);
void reverse (int8 *);
int8 *string_decimal (int64);
void string_write (int64,int8 *);
int64 string_length (int8 *);
int8 *string_copy (int8 *,int8 *);
int8 *string_concat (int8 *,int8 *);
int64 string_compare (int8 *,int8 *);
int64 string_indexOfChar (int8 *,int8);
int64 decimalToInt (int8 *);

