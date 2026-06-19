/*
**	Definiciones de estructuras para la compilacion de lmsc
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

#define		int8		char
#define		int16		short
#define		int32		int
#define		int64		long long
#define		float32		float
#define		float64		double

#define		uint8		unsigned char
#define		uint16		unsigned short
#define		uint32		unsigned int

union
  ParameterType
  {
      int32		pint8;
      int32		pint16;
      int32		pint32;
      int64		pint64;
      float32		pfloat32;
      float64		pfloat64;
      int64		plocal;
      int64		pglobal;
      int64		pstack;
  };

