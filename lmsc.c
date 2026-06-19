/*
**	Compilador de "lenguaje modular simple" lmsc
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

#include "define.h"
#include "struct.h"
#include "tools.h"

#define		TOKENLENGTH	256
#define   PARAM1      254
#define   PARAM2      255
#define		OBJECTSNUMBER	4096
#define		LINELENGTH	1024

#define		SPACE		0
#define		NUMBER	1
#define		LETTER	2
#define		OTHER		3

#define		INT8		1
#define		UINT8		2
#define		INT16		3
#define		UINT16	4
#define		INT32		5
#define		UINT32	6
#define		INT64		7
#define		FLOAT32	8
#define		FLOAT64	9
#define		POINTER	10
#define		LABEL		11

#define   EXTERN  16

#define COMMENT "\n"
#define CONS    "  .cons#"
#define ETQ     ".l#:"
#define LABEL_ASM    "#:"
#define LMAIN   "#_main:"

#define EXTERN_ASM  "extern #"

#define MOVRACONS   "  mov rax,.cons#"
#define NOP0    ""
#define PUSHA   "  push rax"
#define PUSHR   "  push qword [rdi]"
#define PUSHM   "  push qword [#]"
#define PUSHSP  "  push qword [rsp+#]"
#define POPA    "  pop rax"
#define POP1    "  pop rdi"
#define POP2    "  pop rcx"
#define CMP0    "  cmp rdi,rax"
#define SAR0    "  sar rax,cl"
#define SHL0    "  shl rax,cl"
#define SHR0    "  shr rax,cl"
#define IMUL0   "  imul qword [rsp]"
#define IDIV0   "  idiv qword [rsp]"
#define MOV0    "  mov [rsp],rax"
#define MOV1    "  mov [rsp],rdx"
#define MOV8    "  mov [rdi],al"
#define MOV_8   "  mov al,[rdi]"
#define MOV_16  "  mov ax,[rdi]"
#define MOV_32  "  mov eax,[rdi]"
#define MOVA8   "  mov rax,[rsp+8]"
#define MOV16   "  mov [rdi],ax"
#define MOV32   "  mov [rdi],eax"
#define MOV64   "  mov [rdi],rax"
#define MOVI8   "  mov [#],al"
#define MOVI16  "  mov [#],ax"
#define MOVI32  "  mov [#],eax"
#define MOVI64  "  mov [#],rax"
#define MOVE64  "  mov [rsp+#],rax"
#define MOVAL   "  mov al,#\n  movsx rax,al"
#define MOVAX   "  mov ax,#\n  movsx rax,ax"
#define MOVEA   "  mov eax,#\n  movsx rax,eax"
#define MOVALU  "  xor rax,rax\n  mov al,#"
#define MOVAXU  "  xor rax,rax\n  mov ax,#"
#define MOVEAU  "  xor rax,rax\n  mov eax,#"
#define MOVRA   "  mov rax,#"
#define MOVRASP "  mov rax,[rsp+#]"
#define XOR0    "  xor rdx,rdx"
#define XOR1    "  xor rax,rcx"
#define XORA    "  xor rax,rax"
#define AND0    "  and [rsp],rax"
#define ANDI    "  and rax,#"
#define ADD0    "  add [rsp],rax"
#define OR0     "  or [rsp],rax"
#define SUB0    "  sub [rsp],rax"
#define CALL0   "  call #"
#define CALLRSP	"  call [rsp+#]"
#define STACK   "  add rsp,#"
#define STACKA  "  add rsp,8"
#define NEG0    "  neg qword [rsp]"
#define NOT0    "  not qword [rsp]"
#define MOVSXAL "  movsx rax,al"
#define MOVSXAX "  movsx rax,ax"
#define MOVSXEA "  movsx rax,eax"
#define RET0    "  ret\n"
#define JMP0    "  jmp near .l#"
#define DB0     "  db #,0"
#define DBN     "  db #"
#define DW0     "  dw #"
#define DD0     "  dd #"
#define DQ0     "  dq #"
#define JE0     "  je near .l#"
#define JNE0    "  jne near .l#"
#define JL0     "  jl near .l#"
#define JG0     "  jg near .l#"
#define JLE0    "  jle near .l#"
#define JGE0    "  jge near .l#"
#define JA0     "  ja near .l#"
#define JB0     "  jb near .l#"
#define JAE0    "  jae near .l#"
#define JBE0    "  jbe near .l#"

#define LOAD_INT64 "fild qword [rsp]"
#define LOAD_FLOAT64 "fld qword [rsp]"
#define LOAD_FLOAT32 "fld dword [rsp]"
#define STORE_INT64 "fistp qword [rsp]"
#define STORE_FLOAT64 "fstp qword [rsp]"
#define STORE_FLOAT32 "fstp dword [rsp]"
#define F64ADD "fadd qword [rsp]"
#define F32ADD "fadd dword [rsp]"
#define FIADD "fild qword [rsp]\nfaddp"
#define F64SUB "fsubr qword [rsp]"
#define F32SUB "fsubr dword [rsp]"
#define FISUB "fild qword [rsp]\nfsubrp"

#define F64MUL "fmul qword [rsp]"
#define F32MUL "fmul dword [rsp]"
#define FIMUL "fild qword [rsp]\nfmulp"
#define F64DIV "fdivr qword [rsp]"
#define F32DIV "fdivr dword [rsp]"
#define FIDIV "fild qword [rsp]\nfdivrp"

#define POWER "fyl2x\nfld st0\nfrndint\nfsubr st1\nf2xm1\nfld1\nfaddp st1\nfscale\nfstp qword [rsp]"
#define LOG1 "fldl2t\nfld1"
#define LOG2 "fyl2x\nfdivrp st1\nfstp qword [rsp]"

#define UNIT	";unit # "
#define TEXT    "\nBITS 64\nSECTION .text"
#define DATA    "\nBITS 64\nSECTION .data"
#define GLOBAL0 "GLOBAL #_main"
#define GLOBAL  "GLOBAL #"

#define EXIT   "  add rsp,24\n  mov rdi,[rsp]\n  mov rax,60\n  syscall\n"
#define CALLS  "  mov rdi,[rsp]\n  lea rsi,[rsp+8]\n  lea rax,[rsi+rdi*8+8]\n  push qword 0\n  push rdi\n  push qword [rsi]\n  push rax\n  call #.main"

#define CCALL0   "ccall:\n  mov rbp,0x8\n  test rsp,0xf\n  jnz .align\n    mov rbp,0x10\n    push rbp\n.align:\n  push rbp"
#define CCALL1   "  mov rax,[rsp+rbp+0x58]\n  push rax\n  mov rax,[rsp+rbp+0x58]\n  push rax\n  mov rax,[rsp+rbp+0x58]\n  push rax\n  mov rax,[rsp+rbp+0x58]\n  push rax\n  mov r9,[rsp+rbp+0x58]\n  mov r8,[rsp+rbp+0x50]"
#define CCALL2   "  mov rcx,[rsp+rbp+0x48]\n  mov rdx,[rsp+rbp+0x40]\n  mov rsi,[rsp+rbp+0x38]\n  mov rdi,[rsp+rbp+0x30]\n  mov rax,[rsp+rbp+0x28]\n  call rax\n  mov rbp,[rsp+0x20]\n  add rbp,0x20\n  add rsp,rbp\n  ret"


extern float64 pe10 (float64,int64);

int8 syntaxStatement ();
int8 syntaxPrimaryExpression ();
int8 syntaxExpression ();
int8 syntaxBraces (int8,int16,int16);

int64 fileSource,fileNASM,lineNumber;

int64 blockLabel[OBJECTSNUMBER];

int8
  tk[TOKENLENGTH],
  token[OBJECTSNUMBER][TOKENLENGTH],
  stringBuffer[TOKENLENGTH],
  line[LINELENGTH],
  unitName[TOKENLENGTH],
  externalUnit[OBJECTSNUMBER][TOKENLENGTH],
  blockName[OBJECTSNUMBER][TOKENLENGTH],
  variableName[OBJECTSNUMBER][TOKENLENGTH],
  variableType[OBJECTSNUMBER],
  globalName[OBJECTSNUMBER][TOKENLENGTH],
  globalType[OBJECTSNUMBER],
  bufferCharacter[1],
  unreadCharacter[1],
  unreadToken[1],
  nasm[TOKENLENGTH],
  param[TOKENLENGTH],
  *comment,
  useCCALL;
  
int16
  tokenIndex,
  numberOfExternalUnits,
  variableIndex,
  globalIndex,
  actualLabel,
  labelIndex,
  blockIndex;

void writeASM (int8 *a,int8 *p,int8 *c) {
  int16 l;

  if (!string_compare (a,NOP0)) return;
  if (!string_compare (nasm,PUSHA) && !string_compare (a,POPA)) {
    string_copy (nasm,";push rax\n;pop rax");
    return;
  }
  l=string_indexOfChar (nasm,'#');
  if (l==-1) {
    string_write (fileNASM,nasm);
    string_write (fileNASM,comment);
  } else {
    system_write (fileNASM,nasm,l);
    string_write (fileNASM,param);
    string_write (fileNASM,nasm+l+1);
    string_write (fileNASM,comment);
  }
  string_copy (nasm,a);
  string_copy (param,p);
  comment=c;
}

int8 type (int8 c) {
  if ('0'<=c && c<='9') return NUMBER;
  if ('a'<=c && c<='z' || 'A'<=c && c<='Z' || c=='_' || c=='.')
    return LETTER;
  if (c==' ' || c=='\t' || c=='\r' || c=='\n') return SPACE;
  return OTHER;
}

void varASM (int8 t) {
  if (t==INT8 || t==UINT8) writeASM (DBN,NOP0,NOP0);
  else if (t==INT16 || t==UINT16) writeASM (DW0,NOP0,NOP0);
  else if (t==INT32 || t==UINT32 || t==FLOAT32) writeASM (DD0,NOP0,NOP0);
  else if (t==INT64 || t==FLOAT64 || t==POINTER) writeASM (DQ0,NOP0,NOP0);
}

void movR (int8 t,int8* p) {
  if (t==INT8) writeASM (MOVAL,p,COMMENT);
  else if (t==UINT8) writeASM (MOVALU,p,COMMENT);
  else if (t==INT16) writeASM (MOVAX,p,COMMENT);
  else if (t==UINT16) writeASM (MOVAXU,p,COMMENT);
  else if (t==INT32) writeASM (MOVEA,p,COMMENT);
  else if (t==UINT32 || t==FLOAT32) writeASM (MOVEAU,p,COMMENT);
  else if (t==INT64 || t==FLOAT64 || t==POINTER) writeASM (MOVRA,p,COMMENT);
}

int8 varType (int8 *t) {
  if (!string_compare (t,"int8")) return INT8;
  else if (!string_compare (t,"int16")) return INT16;
  else if (!string_compare (t,"int32")) return INT32;
  else if (!string_compare (t,"uint8")) return UINT8;
  else if (!string_compare (t,"uint16")) return UINT16;
  else if (!string_compare (t,"uint32")) return UINT32;
  else if (!string_compare (t,"int64") || !string_compare (t,"uint64"))
    return INT64;
  else if (!string_compare (t,"float32")) return FLOAT32;
  else if (!string_compare (t,"float64")) return FLOAT64;
  else if (!string_compare (t,"pointer")) return POINTER;
  return 0;
}

int8 unread (int8 c,int8 *p) {
  int8 b;

  b=*p; *p=c; return b;
}

int8 readCharacter () {
  int8 c;
  int16 l;

  c=unread (0,unreadCharacter);
  if (!c) {
    l=system_read (fileSource,bufferCharacter,1);
    if (!l) c=0;
    else c=*bufferCharacter;
    if (c=='\n') {
      lineNumber++;
      line[0]=0;
    } else {
      l=string_length (line);
      line[l]=c;
      line[l+1]=0;
    }
  }
  return c;
}

void readToken () {
  int8 t,c;
  int16 i;

#ifdef DEBUG
    string_write (STDOUT,"Function token \'");
    string_write (STDOUT,tk);
    string_write (STDOUT,"\' line ");
    string_write (STDOUT,string_decimal (lineNumber));
    string_write (STDOUT,".\n");
#endif
  if (unread (0,unreadToken)) return;
  tokenMain:
  for (;;) {
    for (;;) {
      c=readCharacter ();
      if (type (c)!=SPACE) break;
    }
    if (c=='/') {
      c=readCharacter ();
      if (c=='*') {
        for (;;) {
          c=readCharacter ();
	  if (c=='*') {
	    c=readCharacter ();
	    if (c=='/') goto tokenMain;
	    unread (c,unreadCharacter);
	  }
        }
      } else if (c=='/') {
        for (;c!='\n';c=readCharacter ());
	goto tokenMain;
      } else {
        unread (c,unreadCharacter);
	c='/';
        goto tokenEnd;
      }
    } else goto tokenEnd;
  }
  tokenEnd:
  tk[0]=c;
  i=1;
  if (c=='\"' || c=='\'') {
    for (;;) {
      c=readCharacter ();
      tk[i]=c;
      i++;
      if (c==tk[0] && (tk[i-2]!='\\' || i<3 || tk[i-3]=='\\')) break;
    }
  }  else if (type (c)!=OTHER) {
    for (;;i++) {
      c=readCharacter ();
      tk[i]=c;
      t=type (c);
      if (tk[0]=='.' || t==SPACE || t==OTHER || type (tk[0])==NUMBER && t==LETTER) {
        unread (c,unreadCharacter);
	break;
      }
    }
  }
  tk[i]=0;
#ifdef DEBUG
  string_write (STDOUT,"Function token return \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\'\n");
#endif
}

int8 *parseString (int8 *s) {
  
  if (*s=='\"' || *s=='\''){
    s[0]=96;			//'`'
    s[string_length (s)-1]=96;	//'`'
  }
  return s;
}

int64 find (int64 n,int8 v[OBJECTSNUMBER][TOKENLENGTH],int8 *s) {
  int64 i;

#ifdef DEBUG
  string_write (STDOUT,"Function find \'");
  string_write (STDOUT,s);
  string_write (STDOUT,"\'\n");
#endif
  for (i=0;i<n;i++) {
#ifdef DEBUG
    string_write (STDOUT,"  \'");
    string_write (STDOUT,v[i]);
    string_write (STDOUT,"\'\n");
#endif
    if (!string_compare (s,v[i])) {
#ifdef DEBUG
    string_write (STDOUT,"  \'");
    string_write (STDOUT,string_hexadecimal (i));
    string_write (STDOUT,"\'\n");
#endif
      return i;
    }
  }
  return -1;
}

int8 syntaxNumber (int8 d) {
  int64 e=0,b;
  float64 f=0,c=0.1E0;
  int16 it=tokenIndex;
  int8 i,p,s,f32=0;
  union ParameterType pt;

#ifdef DEBUG
  string_write (STDOUT,"Function syntaxNumber \'");
  string_write (STDOUT,tk);
  if (d) string_write (STDOUT," true ");
  else string_write (STDOUT," false ");
  string_write (STDOUT,"\'\n");
#endif
  b=0;
  if (tk[0]==39) {
    if (d) writeASM (tk,NOP0,NOP0);
    else {
      writeASM (MOVRA,parseString (tk),COMMENT);
      writeASM (PUSHA,NOP0,COMMENT);
      variableName[variableIndex][0]=0;
      variableType[variableIndex]=POINTER;
      variableIndex++;
    }
  } else {
    s=0;
    if (type (tk[0])==LETTER) {
      if (d) {
        b=find (globalIndex,globalName,tk);
        if (b==-1) return 0;
        string_copy (stringBuffer,unitName);
        string_concat (stringBuffer,".");
        string_concat (stringBuffer,tk);
        writeASM (stringBuffer,NOP0,NOP0);
      } else {
        writeASM (MOVRA,tk,COMMENT);
        writeASM (PUSHA,NOP0,COMMENT);
        variableName[variableIndex][0]=0;
        variableType[variableIndex]=POINTER;
        variableIndex++;
      }
    } else if (!string_compare (tk,"+")) readToken ();
    else if (!string_compare (tk,"-")) {s=-1;readToken ();}
    if (type (tk[0])==NUMBER) {
      for (i=0;tk[i];i++) f=f*10+tk[i]-'0';
      tokenIndex++;
      string_copy (token[it],tk);
      readToken ();
      if (tk[0]=='x' && token[it][0]=='0' && !token[it][1]) {
        string_concat (token[it],tk);
        if (d) writeASM (token[it],NOP0,NOP0);
        else {
          writeASM (MOVRA,token[it],COMMENT);
          writeASM (PUSHA,NOP0,COMMENT);
        }
        variableName[variableIndex][0]=0;
        variableType[variableIndex]=INT64;
        variableIndex++;
        tokenIndex=it;
        return -1;
      } else if (!string_compare (tk,".")) {
        b=-1;
        readToken ();
        if (type (tk[0])!=NUMBER) return 0;
        string_concat (token[it],".");
        for (i=0;tk[i];i++,c=c*0.1E0) f=f+(tk[i]-'0')*c;
        string_concat (token[it],tk);
        readToken ();
      }
      if (tk[0]=='e' || tk[0]=='E') {
        f32=(tk[0]=='e');
        b=-1;
        if ((type (tk[1])==NUMBER)) {
          for (i=1;tk[i];i++) e=e*10+tk[i]-'0';
          f=pe10 (f,e);
          string_concat (token[it],tk);
        } else {
          readToken ();
          if (string_compare (tk,"+") && string_compare (tk,"-"))
            return 0;
          p=tk[0];
          string_concat (token[it],tk);
          readToken ();
          if (type (tk[0])!=NUMBER) return 0;
          for (i=0;tk[i];i++) e=e*10+tk[i]-'0';
          if (p==(int8)'+') f=pe10 (f,e);
          else f=pe10 (f,-e);
          string_concat (token[it],tk);
        }
      } else unread (-1,unreadToken);
      if (s) f=-f;
      if (d) {
        if (s) {
          string_copy (token[it+1]+1,token[it]);
          token[it+1][0]='-';
          writeASM (token[it+1],NOP0,NOP0);
        } else writeASM (token[it],NOP0,NOP0);
      } else {
        variableName[variableIndex][0]=0;
        if (!b) {
          if (s) {
            string_copy (token[it+1]+1,token[it]);
            token[it+1][0]='-';
            writeASM (MOVRA,token[it+1],COMMENT);
          } else writeASM (MOVRA,token[it],COMMENT);
          writeASM (PUSHA,NOP0,COMMENT);
          variableType[variableIndex]=INT64;
        } else {
          if (f32) {
            pt.pfloat32=f;
            string_copy (token[it+1],string_hexadecimal (pt.pint32));
            variableType[variableIndex]=FLOAT32;
          } else {
            pt.pfloat64=f;
            string_copy (token[it+1],string_hexadecimal (pt.pint64));
            variableType[variableIndex]=FLOAT64;
          }
          writeASM (MOVRA,token[it+1],COMMENT);
          writeASM (PUSHA,NOP0,COMMENT);
        }
        variableIndex++;
      }
    }
  }
  tokenIndex=it;
  return -1;
}

int8 syntaxString () {
  if ((tk[0]=='\"')) {
    writeASM (JMP0,string_hexadecimal (labelIndex),COMMENT);
    writeASM (CONS,string_hexadecimal (labelIndex),NOP0);
    writeASM (DB0,parseString (tk),COMMENT);
    writeASM (ETQ,string_hexadecimal (labelIndex),COMMENT);
    writeASM (MOVRACONS,string_hexadecimal (labelIndex),COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
    variableName[variableIndex][0]=0;
    variableType[variableIndex]=POINTER;
    variableIndex++;
    labelIndex++;
    return -1;
  }
  return 0;
}

int8 syntaxPartialParameter (int16 c) {
  int8 d,cr=0;
  
  for (;;) {
    readToken ();
    d=varType (tk);
    if (!d) break;
    readToken ();
    string_copy (variableName[variableIndex],tk);
    variableType[variableIndex]=d;
    variableIndex++;
    cr=cr+1;
    readToken ();
    if (string_compare (tk,",")) break;
  }
  globalName[globalIndex][c]=cr;
  return -1;
}

int8 syntaxParameter () {
  if (!syntaxPartialParameter (PARAM1)) return 0;
  if (string_compare (tk,";")) return 0;
  if (!syntaxPartialParameter (PARAM2)) return 0;
  return -1;
}

int8 syntaxBreakContinue (int8 *n) {
  int16 b;

#ifdef DEBUG
  string_write (STDOUT,"Function syntaxBreakContinue \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' \'");
  string_write (STDOUT,n);
  string_write (STDOUT,"\'\n");
#endif
  if (string_compare (tk,n)) return 0;
  readToken ();
  if ((type (tk[0])==LETTER)) {
    b=find (blockIndex,blockName,tk);
    if (b==-1) return 0;
    b=blockLabel[b];
    readToken ();
  } else b=actualLabel;
  if (string_compare (tk,";")) return 0;
  if (*n=='c') writeASM (JMP0,string_hexadecimal (b)," ;begin\n");
  else writeASM (JMP0,string_hexadecimal (b+1)," ;end\n");
  return -1;
}

void writeCondition (int8 *s,int16 l) {
  writeASM (POPA,NOP0,COMMENT);
  writeASM (POP1,NOP0,COMMENT);
  writeASM (CMP0,NOP0,COMMENT);
  writeASM (s,string_hexadecimal (l)," ;cond\n");
  variableIndex-=2;
}

int8 syntaxPrimaryCondition (int8 c,int16 b) {
  int8 *s,*ss;
  
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxPrimaryCondition \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' \n");
#endif
  if (!syntaxExpression ()) return 0;
  readToken ();
  if (!string_compare (tk,"carry")) {s=JB0;ss=JAE0;}
  else if (!string_compare (tk,"zero")) {s=JE0;ss=JNE0;}
  else if (!string_compare (tk,"<")) {
    readToken ();
    if (!string_compare (tk,"=")) {s=JLE0;ss=JG0;}
    else if (!string_compare (tk,">")) {s=JNE0;ss=JE0;}
    else if (!string_compare (tk,"<")) {
      readToken ();
      if (!string_compare (tk,"=")) {s=JBE0;ss=JA0;}
      else {s=JB0;ss=JAE0;unread (-1,unreadToken);}
    } else {s=JL0;ss=JGE0;unread (-1,unreadToken);}
  } else if (!string_compare (tk,">")) {
    readToken ();
    if (!string_compare (tk,"=")) {s=JGE0;ss=JL0;}
    else if (!string_compare (tk,">")) {
      readToken ();
      if (!string_compare (tk,"=")) {s=JAE0;ss=JB0;}
      else {s=JA0;ss=JBE0;unread (-1,unreadToken);}
    } else {s=JG0;ss=JLE0;unread (-1,unreadToken);}
  } else if (!string_compare (tk,"=")) {s=JE0;ss=JNE0;}
  else return 0;
  readToken ();
  if (!syntaxExpression ()) return 0;
  if (c) s=ss;
  writeCondition (s,b);
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxPrimaryCondition end \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' ");
#endif
  return -1;
}

int8 syntaxNot (int8 c,int16 b,int16 el) {
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxNot \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' \n");
#endif
    writeASM (";syntaxNot","",COMMENT);
    readToken ();
    if (!string_compare (tk,"["))
      return syntaxBraces (!c,b,el);
    else return syntaxPrimaryCondition (!c,b);
}

int8 syntaxOrAnd (int8 c,int16 b,int16 el,int8 o) {
  int8 *oa,*et;
  int16 s;

  if (o) {
    oa="or";et=" ;or\n";
  } else {
    oa="and";et=" ;and\n";
  }
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxOrAnd ");
  string_write (STDOUT,oa);
  string_write (STDOUT," \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' \n");
#endif
  for (;;) {
    s=labelIndex;
    labelIndex++;
    if (o) {
      if (!syntaxOrAnd (c,b,s,0)) return 0;
    } else {
      if (!string_compare (tk,"[")) {
        if (!syntaxBraces (c,s,el)) return 0;
      } else if (!string_compare (tk,"not")) {
        if (!syntaxNot (!c,el,s)) return 0;
      } else {
	if (!syntaxPrimaryCondition (!c,el)) return 0;
      }
    }
    writeASM (ETQ,string_hexadecimal (s)," ;orand\n");
    readToken ();
    if (string_compare (tk,oa)) break;
    readToken ();
  }
  unread (-1,unreadToken);
  if (o) {
    writeASM (JMP0,string_hexadecimal (el)," ;else\n");
  } else {
    writeASM (JMP0,string_hexadecimal (b)," ;if\n");
  }
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxOrAnd end ");
  string_write (STDOUT,oa);
  string_write (STDOUT," \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' \n");
#endif
  return -1;
}

int8 syntaxBraces (int8 c,int16 b,int16 el) {
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxBraces \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' \n");
#endif
  readToken ();
  if (!syntaxOrAnd (c,b,el,-1)) return 0;
  readToken ();
  if (string_compare (tk,"]")) return 0;
  return -1;
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxBraces end \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' \n");
#endif
  return 0;
}

int8 syntaxIf () {
  int16 b,el,e;		//label if,else,end
  
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxIf \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' ");
  string_write (STDOUT,"\n");
#endif
  if (string_compare (tk,"if")) return 0;
  readToken ();
  if (string_compare (tk,"(")) return 0;
  b=labelIndex;
  labelIndex++;
  el=labelIndex;
  labelIndex++;
  e=labelIndex;
  labelIndex++;
  readToken ();
  if (!syntaxOrAnd (0,b,el,-1)) return 0;
  readToken ();
  if (string_compare (tk,")")) return 0;
  readToken ();
  writeASM (ETQ,string_hexadecimal (b)," ;if\n");
  if (!syntaxStatement ()) return 0;
  readToken ();
  if (!string_compare (tk,"else")) {
    writeASM (JMP0,string_hexadecimal (e)," ;fi\n");
    writeASM (ETQ,string_hexadecimal (el)," ;else\n");
    readToken ();
    if (!syntaxStatement ()) return 0;
  } else {
    writeASM (ETQ,string_hexadecimal (el)," ;else\n");
    unread (-1,unreadToken);
  }
  writeASM (ETQ,string_hexadecimal (e)," ;fi\n");
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxIf end \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' ");
  string_write (STDOUT,"\n");
#endif
  return -1;
}

void error (int8 *s) {
  string_write (STDOUT,s);
  string_write (STDOUT," line ");
  string_write (STDOUT,string_decimal (lineNumber));
  string_write (STDOUT,".\n-->");
  string_write (STDOUT,line);
  string_write (STDOUT,"<--\n");
}

void writeFunction (int8 *s,uint16 d) {
  if (2!=d) error ("warning: function parameters<>definition");
  writeASM (POP2,NOP0,COMMENT);
  writeASM (POPA,NOP0,COMMENT);  
  writeASM (s,NOP0,COMMENT);
  writeASM (PUSHA,NOP0,COMMENT);
  variableIndex--;
}

int8 syntaxFunctionCall (int8 z,int16 c) {
  int64 sl=variableIndex,f;

#ifdef DEBUG
  string_write (STDOUT,"Function syntaxFunctionCall \'");
  string_write (STDOUT,token[c]);
  string_write (STDOUT,"\'\n");
#endif
  for (;;) {
    readToken ();
    if (!string_compare (tk,")")) break;
    if (!syntaxExpression ()) return 0;
    readToken ();
    if (!string_compare (tk,")")) break;
    if (string_compare (tk,",")) return 0;
  }
  if (!string_compare (token[c],"sar")) {
    writeFunction (SAR0,variableIndex-sl);
    return -1;
  } else if (!string_compare (token[c],"shr")) {
    writeFunction (SHR0,variableIndex-sl);
    return -1;
  } else if (!string_compare (token[c],"shl")) {
    writeFunction (SHL0,variableIndex-sl);
    return -1;
  } else if (!string_compare (token[c],"xor")) {
    writeFunction (XOR1,variableIndex-sl);
    return -1;
  } else if (!string_compare (token[c],"stack")) {
    return -1;
  } else if (!string_compare (token[c],"ccall")) {
    writeASM (CALL0,token[c],COMMENT);
    useCCALL=1;
    if (variableIndex!=sl+z) {
      writeASM (STACK,string_hexadecimal ((variableIndex-sl-z)<<3),COMMENT);
      variableIndex=sl+z;
    }
    if (z) writeASM (MOV0,NOP0,COMMENT);
    return -1;
  } else if (!string_compare (token[c],"log")) {
    if (1!=variableIndex-sl)
      error ("warning: function parameters<>definition");
    writeASM (LOG1,NOP0,COMMENT);
    if (variableType[variableIndex-1]==FLOAT64)
      writeASM (LOAD_FLOAT64,NOP0,COMMENT);
    else if (variableType[variableIndex-1]==FLOAT32)
      writeASM (LOAD_FLOAT32,NOP0,COMMENT);
    else writeASM (LOAD_INT64,NOP0,COMMENT);
    writeASM (LOG2,NOP0,COMMENT);
    return -1;
  }
  f=find (variableIndex,variableName,token[c]);
  if (f==-1) {
    f=find (globalIndex,globalName,token[c]);
    if (f==-1) return 0;
    else {
      if (z && globalName[f][PARAM1]) z=globalName[f][PARAM1];
      if ((globalType[f]&~EXTERN)!=POINTER) {
        if (globalName[f][PARAM1]+globalName[f][PARAM2]!=variableIndex-sl)
          error ("warning: function parameters<>definition");
      }
      if (!(globalType[f]&EXTERN)) {
        string_copy (tk,unitName);
        string_concat (tk,".");
        string_concat (tk,token[c]);      
      } else string_copy (tk,token[c]);
      if ((globalType[f]&~EXTERN)==LABEL) writeASM (CALL0,tk,COMMENT);
      else if ((globalType[f]&~EXTERN)==POINTER) {
        string_copy (stringBuffer,"[");
        string_concat (stringBuffer,tk);
        string_concat (stringBuffer,"]");
        writeASM (CALL0,stringBuffer,COMMENT);
      } else return 0;
    }
  } else writeASM (
    CALLRSP,string_hexadecimal ((variableIndex-f-1)<<3),COMMENT);
  if (variableIndex!=sl+z) {
    writeASM (STACK,string_hexadecimal ((variableIndex-sl-z)<<3),COMMENT);
    variableIndex=sl+z;
  }
  return -1;
}

int8 syntaxPrimaryExpressionNoPointer () {
  int8 p,c;
  int16 it=tokenIndex;
  int64 sl;

#ifdef DEBUG
  string_write (STDOUT,"Function syntaxPrimaryExpressionNoPointer \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\'\n");
#endif
  if (!string_compare (tk,"+")) {
    readToken ();
    return syntaxPrimaryExpression ();
  }
  if (!string_compare (tk,"-")) {
    readToken ();
    sl=syntaxPrimaryExpression ();
    writeASM (NEG0,NOP0,COMMENT);
    return sl;
  }
  if (!string_compare (tk,"~")) {
    readToken ();
    sl=syntaxPrimaryExpression ();
    writeASM (NOT0,NOP0,COMMENT);
    return sl;
  }
  if (!string_compare (tk,"(")) {
    readToken ();
    if (!syntaxExpression ()) return 0;
    readToken ();
    if (!string_compare (tk,")")) return -1;
    return 0;
  }
  if ((type (tk[0])==LETTER)) {
    tokenIndex++;
    string_copy (token[it],tk);
    readToken ();
    if (!string_compare (tk,"(")) {
      if (!syntaxFunctionCall (1,it)) return 0;
      tokenIndex=it;
      return -1;
    } else unread (-1,unreadToken);
    sl=find (variableIndex,variableName,token[it]);
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxPrimaryExpressionNoPointer \'");
  string_write (STDOUT,token[it]);
  string_write (STDOUT,"\' \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' ");
  string_write (STDOUT," ");
  string_write (STDOUT,"\n");
#endif
    if (sl==-1) {
      sl=find (globalIndex,globalName,token[it]);
      if (sl==-1) return 0;
      p=globalType[sl];
      if (!(p&EXTERN)) {
        if (p==LABEL) c=3;
			  else c=0;
      } else {
        p&=~EXTERN;
        if (p==LABEL) c=5;
        else c=4;
      }
    } else {
      p=variableType[sl];
      c=2;
    }
    if (!c) {
      string_copy (token[it+1],"[");
      string_concat (token[it+1],unitName);
      string_concat (token[it+1],".");
      string_concat (token[it+1],token[it]);
      string_concat (token[it+1],"]");
      movR (p,token[it+1]);
    } else if (c==1) {
      string_copy (token[it+1],"[");
      string_concat (token[it+1],token[it]);
      string_concat (token[it+1],"]");
      writeASM (MOVRA,token[it+1],COMMENT);
    } else if (c==3) {
      string_copy (token[it+1],unitName);
      string_concat (token[it+1],".");
      string_concat (token[it+1],token[it]);
      writeASM (MOVRA,token[it+1],COMMENT);
		} else if (c==4) {
      string_copy (stringBuffer,"[");
      string_concat (stringBuffer,token[it]);
      string_concat (stringBuffer,"]");
      movR (p,stringBuffer);
		} else if (c==5) {
      string_copy (stringBuffer,token[it]);
		  writeASM (MOVRA,stringBuffer,COMMENT);
    } else writeASM (MOVRASP,
	    string_hexadecimal ((variableIndex-sl-1)<<3),COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
    variableName[variableIndex][0]=0;
    variableType[variableIndex]=p;
    variableIndex++;
    tokenIndex=it;
    return -1;
  }
  return syntaxNumber (0);
}

int8 syntaxPointer () {
  int8 t;
  
  readToken ();
  t=varType (tk);
  if (!t) {
    if (!syntaxExpression ()) return 0;
    readToken ();
    if (string_compare (tk,",")) return 0;
    readToken ();
    t=varType (tk);
    if (!t) return 0;
    writeASM (POPA,NOP0,COMMENT);
    writeASM (ADD0,NOP0,COMMENT);
    variableIndex-=1;
  }
  writeASM (POP1,NOP0,COMMENT);
  if (t==INT8) {
    writeASM (MOV_8,NOP0,COMMENT);
    writeASM (MOVSXAL,NOP0,COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
  } else if (t==UINT8) {
    writeASM (XORA,NOP0,COMMENT);
    writeASM (MOV_8,NOP0,COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
  } else if (t==INT16) {
    writeASM (MOV_16,NOP0,COMMENT);
    writeASM (MOVSXAX,NOP0,COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
  } else if (t==UINT16) {
    writeASM (XORA,NOP0,COMMENT);
    writeASM (MOV_16,NOP0,COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
  } else if (t==INT32) {
    writeASM (MOV_32,NOP0,COMMENT);
    writeASM (MOVSXEA,NOP0,COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
  } else if (t==UINT32) {
    writeASM (XORA,NOP0,COMMENT);
    writeASM (MOV_32,NOP0,COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
  } else if (t==FLOAT32) {
    writeASM (XORA,NOP0,COMMENT);
    writeASM (MOV_32,NOP0,COMMENT);
    writeASM (PUSHA,NOP0,COMMENT);
  } else if (t==INT64 || t==FLOAT64 || t==POINTER)
    writeASM (PUSHR,NOP0,COMMENT);
  variableType[variableIndex-1]=t;
  variableName[variableIndex-1][0]=0;
  readToken ();
  if (!string_compare (tk,"]")) return -1;
  return 0;
}

int8 syntaxPrimaryExpression () {
  if (!syntaxPrimaryExpressionNoPointer ()) return 0;
  readToken ();
  if (!string_compare (tk,"[")) return syntaxPointer ();
  unread (-1,unreadToken);
  return -1;
}

int8 syntaxPower () {
  
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxPower \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\'\n");
#endif
  if (!syntaxPrimaryExpression ()) return 0;
  for (;;) {
    readToken ();
    if (string_compare (tk,"^")) break;
    readToken ();
    if (!syntaxPrimaryExpression ()) return 0;
    variableIndex--;
    if (variableType[variableIndex]==FLOAT64)
      writeASM (LOAD_FLOAT64,NOP0,COMMENT);
    else if (variableType[variableIndex]==FLOAT32)
      writeASM (LOAD_FLOAT32,NOP0,COMMENT);
    else writeASM (LOAD_INT64,NOP0,COMMENT);
      writeASM (STACKA,NOP0,COMMENT);
    if (variableType[variableIndex-1]==FLOAT64)
      writeASM (LOAD_FLOAT64,NOP0,COMMENT);
    else if (variableType[variableIndex-1]==FLOAT32)
      writeASM (LOAD_FLOAT32,NOP0,COMMENT);
    else writeASM (LOAD_INT64,NOP0,COMMENT);
    writeASM (POWER,NOP0,COMMENT);
    variableType[variableIndex]=FLOAT64;
  }
  unread (-1,unreadToken);
  return -1;
}

int8 syntaxAddMul (int8 a) {
  int64 sl=variableIndex;
  int16 it=tokenIndex,i;
  int8 b,*c,*d,*e,*f,*g,*co;

#ifdef DEBUG
  string_write (STDOUT,"Function syntaxAddMul \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\'\n");
#endif
  token[it][0]=0;
  tokenIndex++;
  if (a) {
    if (!syntaxAddMul (0)) return 0;
  } else {
    if (!syntaxPower ()) return 0;
  }
  for (;;) {
    readToken ();
    b=tk[0];
    if (a && (b=='+' || b=='-') || !a && (b=='*' || b=='/' || b=='%')) {
      readToken ();
      if (!string_compare (tk,"[")) {
        readToken ();
        string_copy (token[it],tk);
        readToken ();
        if (string_compare (tk,"]")) return 0;
        readToken ();
      }
      if (a) {
        if (!syntaxAddMul (0)) return 0;
      } else {
        if (!syntaxPower ()) return 0;
      }
      if (token[it][0]) {
        i=-1;
        if (b=='+') {c="add";d=".add";}
        else if (b=='*') {c="mul";d=".mul";}
        else return 0;
        if (!string_compare (token[it],unitName))
          i=find (globalIndex,globalName,c);
        string_concat (token[it],d);
        if (i==-1) i=find (globalIndex,globalName,token[it]);
        writeASM (CALL0,token[it],COMMENT);
        if (i==-1) return 0;
        if (globalName[i][PARAM1]+globalName[i][PARAM2]!=variableIndex-sl)
          error ("warning: function parameters<>definition");
        if (variableIndex!=sl+globalName[i][PARAM1]) {
          writeASM (STACK,string_hexadecimal ((variableIndex-sl-globalName[i][PARAM1])<<3),COMMENT);
          variableIndex=sl+globalName[i][PARAM1];
        }
        return -1;
      }
      d=STACKA;f=XORA;e=NOP0;g=NOP0;
      variableIndex--;
      if (variableType[variableIndex]==FLOAT64) c=LOAD_FLOAT64;
      else if (variableType[variableIndex]==FLOAT32)           
        c=LOAD_FLOAT32;
      else if (variableType[variableIndex-1]!=FLOAT64 &&
        variableType[variableIndex-1]!=FLOAT32) {
        if (b=='+') {c=POPA;d=NOP0;e=NOP0;f=ADD0;g=NOP0;}
        else if (b=='-') {c=POPA;d=NOP0;e=NOP0;f=SUB0;g=NOP0;}
        else if (b=='*') {c=XOR0;d=POPA;e=NOP0;f=IMUL0;g=MOV0;}
        else if (b=='/') {c=XOR0;d=MOVA8;e=IDIV0;f=STACKA;g=MOV0;}
        else if (b=='%') {c=XOR0;d=MOVA8;e=IDIV0;f=STACKA;g=MOV1;}
        variableType[variableIndex-1]=INT64;
      } else c=LOAD_INT64;
      if (variableType[variableIndex-1]==FLOAT64) {
        g=STORE_FLOAT64;
      } else if (variableType[variableIndex-1]==FLOAT32) {
        if (b=='+') f=F32ADD;
        else if (b=='-') f=F32SUB;
        else if (b=='*') f=F32MUL;
        else if (b=='/') f=F32DIV;
        if (c==LOAD_FLOAT64) {
          g=STORE_FLOAT64;
          variableType[variableIndex-1]=FLOAT64;
        } else g=STORE_FLOAT32;
      } else if (f==XORA) {
        if (b=='+') f=FIADD;
        else if (b=='-') f=FISUB;
        else if (b=='*') f=FIMUL;
        else if (b=='/') f=FIDIV;
        if (c==LOAD_FLOAT64) {
          g=STORE_FLOAT64;
          variableType[variableIndex-1]=FLOAT64;
        } else {
          g=STORE_FLOAT32;
          variableType[variableIndex-1]=FLOAT32;
        }
      }
      if (f==XORA) {
        if (b=='+') f=F64ADD;
        else if (b=='-') f=F64SUB;
        else if (b=='*') f=F64MUL;
        else if (b=='/') f=F64DIV;
      }
    } else if (b=='|') {
      readToken ();
      if (!syntaxAddMul (0)) return 0;
      c=NOP0;d=POPA;e=NOP0;f=OR0;g=NOP0;
      variableIndex--;
    } else if (b=='&') {
      readToken ();
      if (!syntaxPower ()) return 0;
      c=NOP0;d=POPA;e=NOP0;f=AND0;g=NOP0;
      variableIndex--;
    } else break;
    if (c==NOP0) co=NOP0; else co=COMMENT;
    writeASM (c,NOP0,co);
    if (d==NOP0) co=NOP0; else co=COMMENT;
    writeASM (d,NOP0,co);
    if (e==NOP0) co=NOP0; else co=COMMENT;
    writeASM (e,NOP0,co);
    if (f==NOP0) co=NOP0; else co=COMMENT;
    writeASM (f,NOP0,co);
    if (g==NOP0) co=NOP0; else co=COMMENT;
    writeASM (g,NOP0,co);
  }
  unread (-1,unreadToken);
  tokenIndex=it;
  return -1;
}

int8 syntaxExpression () {
  if (syntaxString ()) return -1;
  return syntaxAddMul (-1);
}

int8 syntaxPointerAssign () {
  int8 p;

#ifdef DEBUG
  string_write (STDOUT,"Function syntaxPointerAssign\n");
#endif
  readToken ();
  p=varType (tk);
  if (!p) {
    if (!syntaxExpression ()) return 0;
    writeASM (POPA,NOP0,COMMENT);
    variableIndex--;
    writeASM (ADD0,NOP0,COMMENT);
    readToken ();
    if (string_compare (tk,",")) return 0;
    readToken ();
    p=varType (tk);
    if (!p) return 0;
  }
  readToken ();
  if (string_compare (tk,"]")) return 0;
  readToken ();
  if (string_compare (tk,":")) return 0;
  readToken ();
  if (string_compare (tk,"=")) return 0;
  readToken ();
  if (!syntaxExpression ()) return 0;
  readToken ();
  if (string_compare (tk,";")) return 0;
  writeASM (POPA,NOP0,COMMENT);
  writeASM (POP1,NOP0,COMMENT);
  variableIndex-=2;
  if (p==INT8 || p==UINT8) writeASM (MOV8,NOP0,COMMENT);
  else if (p==INT16 || p==UINT16) writeASM (MOV16,NOP0,COMMENT);
  else if (p==INT32 || p==UINT32 || p==FLOAT32)
    writeASM (MOV32,NOP0,COMMENT);
  else if (p==INT64 || p==FLOAT64 || p==POINTER)
    writeASM (MOV64,NOP0,COMMENT);
  return -1;
}

int8 syntaxAssign (int8 n,int16 it) {
  int8 p;
  int64 sl;

#ifdef DEBUG
  string_write (STDOUT,"Tipo de variable\n");
  string_write (STDOUT,"Function syntaxAssign \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\'\n");
#endif
  readToken ();
  if (string_compare (tk,"=")) return 0;
  readToken ();
  if (!syntaxExpression ()) return 0;
  readToken ();
  if (string_compare (tk,";")) return 0;
  for (;n>0;n--) {
    sl=find (variableIndex,variableName,token[it+n-1]);
    variableIndex--;
    if (sl==-1) {
      writeASM (POPA,NOP0,COMMENT);
      sl=find (globalIndex,globalName,token[it+n-1]);
      if (sl==-1) return 0;
      p=globalType[sl];
      stringBuffer[0]=0;
      if (!(p&EXTERN)) {
        string_copy (stringBuffer,unitName);
        string_concat (stringBuffer,".");
      } else p=p&~EXTERN;
      string_concat (stringBuffer,token[it+n-1]);
      if (p==INT64 || p==FLOAT64 || p==POINTER)
        writeASM (MOVI64,stringBuffer,COMMENT);
      else if (p==FLOAT32 || p==INT32 || p==UINT32)
        writeASM (MOVI32,stringBuffer,COMMENT);
      else if (p==INT16 || p==UINT16)
        writeASM (MOVI16,stringBuffer,COMMENT);
      else if (p==INT8 || p==UINT8)
        writeASM (MOVI8,stringBuffer,COMMENT);
      else return 0;
      return -1;
    } else {
      p=variableType[sl];
      if (p<=INT64) {
        if (variableType[variableIndex]==FLOAT64) {
          writeASM (LOAD_FLOAT64,NOP0,COMMENT);
          writeASM (STORE_INT64,NOP0,COMMENT);
        } else if (variableType[variableIndex]==FLOAT32) {
          writeASM (LOAD_FLOAT32,NOP0,COMMENT);
          writeASM (STORE_FLOAT64,NOP0,COMMENT);
        }
      } else if (p==FLOAT32 && variableType[variableIndex]==FLOAT64) {
        writeASM (LOAD_FLOAT64,NOP0,COMMENT);
        writeASM (STORE_FLOAT32,NOP0,COMMENT);
      } else if (p==FLOAT64 && variableType[variableIndex]==FLOAT32) {
        writeASM (LOAD_FLOAT32,NOP0,COMMENT);
        writeASM (STORE_FLOAT64,NOP0,COMMENT);
      } else if (variableType[variableIndex]<=INT64) {
        if (p==FLOAT64) {
          writeASM (LOAD_INT64,NOP0,COMMENT);
          writeASM (STORE_FLOAT64,NOP0,COMMENT);
        } else if (p==FLOAT32) {
          writeASM (LOAD_INT64,NOP0,COMMENT);
          writeASM (STORE_FLOAT32,NOP0,COMMENT);
        }
      }
      writeASM (POPA,NOP0,COMMENT);
    }
    writeASM (MOVE64,string_hexadecimal ((variableIndex-sl-1)<<3),COMMENT);
  }
  return -1;
}

int8 syntaxDeclaration (int8 g,int8 *t) {
  int8 p;
  int16 it;

#ifdef DEBUG
  string_write (STDOUT,"Function syntaxDeclaration \'");
  string_write (STDOUT,t);
  string_write (STDOUT,"\'\n");
#endif
  p=varType (t);
  if (!p) return 0;
  for (;;) {
    readToken ();
    if (type (tk[0])!=LETTER) return 0;
    if (g) {
      string_copy (stringBuffer,unitName);
      string_concat (stringBuffer,".");
      string_concat (stringBuffer,tk);
      writeASM (GLOBAL,stringBuffer,COMMENT);
      writeASM (stringBuffer,NOP0,NOP0);
      string_copy (globalName[globalIndex],tk);
      globalName[globalIndex][PARAM1]=0;
      globalName[globalIndex][PARAM2]=0;
      globalType[globalIndex]=p;	//globalIndex++ mas adelante
      readToken ();
      if (!string_compare (tk,"=")) {
        readToken ();
        if (tk[0]=='\"') {
          globalType[globalIndex]=LABEL;
	        writeASM (DB0,parseString (tk),NOP0);
        } else {
	        varASM (p);
	        if (!syntaxNumber (-1)) return 0;
        }
	      readToken ();
      } else if (!string_compare (tk,"[")) {
	      varASM (p);
        globalType[globalIndex]=LABEL;
        for (;;) {
          readToken ();
          if (!syntaxNumber (-1)) return 0;
          readToken ();
          if (!string_compare (tk,","))
	          writeASM (",",NOP0,NOP0);
          else if (!string_compare (tk,"]")) {
            readToken ();
	          break;
          } else return 0;
        }
      } else {
        varASM (p);
        writeASM ("0",NOP0,NOP0);
      }
      writeASM (COMMENT,NOP0,NOP0);
      globalIndex++;
    } else {
      it=tokenIndex;
      tokenIndex++;
      string_copy (token[it],tk);
      readToken ();
      if (!string_compare (tk,"=")) {
        readToken ();
	      if (!syntaxExpression ()) return 0;
        variableIndex--;
        readToken ();
      } else {
        writeASM (XORA,NOP0,COMMENT);
        writeASM (PUSHA,NOP0,COMMENT);
      }
      string_copy (variableName[variableIndex],token[it]);
      variableType[variableIndex]=p;
      variableIndex++;
      tokenIndex=it;
    }
    if (!string_compare (tk,";")) {
      writeASM (COMMENT,NOP0,NOP0);
      return -1;
    }
    if (string_compare (tk,",")) return 0;
  }
}

int8 syntaxBlock (int8 s,int8 *n) {
  int16 a,b,v,it=tokenIndex;

#ifdef DEBUG
  string_write (STDOUT,"Function syntaxBlock \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"'\n");
#endif

  tokenIndex++;
  if (!string_compare (tk,"do")) {
    readToken ();
    if (string_compare (tk,"{")) {
      if ((type (tk[0])!=LETTER)) return 0;
      writeASM (";do #\n",tk,COMMENT);
      string_copy (token[it],tk);
      if (n[0] && string_compare (token[it],n)) return 0;
      readToken ();
      if (string_compare (tk,"{")) return 0;
    } else string_copy (token[it],n);
  } else string_copy (token[it],n);
  string_copy (blockName[blockIndex],token[it]);
  blockLabel[blockIndex]=labelIndex;
  blockIndex=blockIndex+1;
  v=variableIndex;
  a=actualLabel;
  actualLabel=labelIndex;
  b=labelIndex;
  labelIndex+=2;
  writeASM (ETQ,string_hexadecimal (b)," ;begin\n");
  for (;;) {
    readToken ();
    if (!syntaxDeclaration (0,tk)) break;
  }
  for (;;) {
    if (!string_compare (tk,"}")) break;
    if (!syntaxStatement ()) return 0;
    readToken ();
  }
  writeASM (ETQ,string_hexadecimal (b+1)," ;end\n");
  if (variableIndex!=v) {
    writeASM (STACK,string_hexadecimal ((variableIndex-v)<<3),COMMENT);
    variableIndex=v;
  }
  if (s) writeASM (RET0,NOP0,COMMENT);
  actualLabel=a;
  tokenIndex=it;
  blockIndex=blockIndex-1;
  return -1;
}

int8 syntaxStatement () {
  int64 sl;
  int16 it=tokenIndex;
  int8 r;
  
#ifdef DEBUG
  string_write (STDOUT,"Function syntaxStatement \'");
  string_write (STDOUT,tk);
  string_write (STDOUT,"\' ");
  string_write (STDOUT,"-->");
  string_write (STDOUT,string_hexadecimal (actualLabel));
  string_write (STDOUT,"\n");
#endif
  if (syntaxBreakContinue ("break")) return -1;
  if (syntaxBreakContinue ("continue")) return -1;
  if (!string_compare (tk,"do") || !string_compare (tk,"{"))
    return syntaxBlock (0,"");
  if (!string_compare (tk,"if")) return syntaxIf ();
  if (type (tk[0])==LETTER) {
    tokenIndex++;
    string_copy (token[it],tk);
    readToken ();
    if (!string_compare (tk,",")) {
      for (;;) {
        readToken ();
        if (!(type (tk[0])==LETTER)) return 0;
        string_copy (token[tokenIndex],tk);
        tokenIndex++;
        readToken ();
        if (!string_compare (tk,",")) continue; 
        if (!string_compare (tk,":")) {
          if (syntaxAssign (tokenIndex-it,it)) {
            tokenIndex=it;return -1;
          }
        }
        return 0;
      }
    } else if (!string_compare (tk,":")) {
      if (syntaxAssign (1,it)) {
        tokenIndex=it;return -1;
      }
      return 0;
    }
    if (!string_compare (tk,"(")) {
      if (!syntaxFunctionCall (0,it)) return 0;
      readToken ();
      if (string_compare (tk,";")) return 0;
      tokenIndex=it;
      return -1;
    }
    if (!string_compare (tk,"[")) {
      sl=find (variableIndex,variableName,token[it]);
      if (sl==-1) {
        sl=find (globalIndex,globalName,token[it]);
        if (sl==-1) return 0;
        else {
          stringBuffer[0]=0;
          if (!(globalType[sl]&EXTERN)) {
	          string_copy (stringBuffer,unitName);
	          string_concat (stringBuffer,".");
	        }
          string_concat (stringBuffer,token[it]);
          if ((globalType[sl]&~EXTERN)==LABEL) {
	          writeASM (MOVRA,stringBuffer,COMMENT);
	          writeASM (PUSHA,NOP0,COMMENT);
	        } else writeASM (PUSHM,stringBuffer,COMMENT);
        }
      } else {
        writeASM (PUSHSP,
          string_hexadecimal ((variableIndex-sl-1)<<3),COMMENT);
        variableType[variableIndex]=variableType[sl];
      }
      variableName[variableIndex][0]=0;
      variableIndex++;
      tokenIndex=it;
      return syntaxPointerAssign ();
    }
  }
  return 0;
}
int8 syntaxFunction () {
  int16 vi;
  
#ifdef DEBUG
    string_write (STDOUT,"Function syntaxFunction \'");
    string_write (STDOUT,tk);
    string_write (STDOUT,"\'\n");
#endif

  vi=variableIndex;
  readToken ();
  if (type (tk[0])!=LETTER) return 0;
  if (tk[0]=='c' && tk[1]=='_') {
    string_copy (stringBuffer,unitName);
    string_concat (stringBuffer,"_");
    string_concat (stringBuffer,tk);
    writeASM (GLOBAL,stringBuffer,COMMENT);
    writeASM (LABEL_ASM,stringBuffer,COMMENT);
  }
  string_copy (stringBuffer,unitName);
  string_concat (stringBuffer,".");
  string_concat (stringBuffer,tk);
  writeASM (GLOBAL,stringBuffer,COMMENT);
  writeASM (LABEL_ASM,stringBuffer,COMMENT);
  string_copy (globalName[globalIndex],tk);
  globalType[globalIndex]=LABEL;
  readToken ();
  if (!string_compare (tk,"(")) {
    if (!syntaxParameter ()) return 0;
    if (string_compare (tk,")")) return 0;
    readToken ();
  }
  globalIndex=globalIndex+1;
  variableName[variableIndex][0]=0;
  variableType[variableIndex]=POINTER;
  variableIndex++;
  if (!syntaxBlock (-1,globalName[globalIndex-1])) return 0;
  variableIndex=vi;
  return -1;
}

int8 syntaxFunctions (int8 r,int8 *name) {
  int8 s,t=r;

  for (;;) {
    readToken ();
    s=varType (tk);
    if (s) readToken ();
    else s=LABEL;
    if (t==EXTERN) {
      globalName[globalIndex][0]=0;
      if (name[0]) {
        string_copy (globalName[globalIndex],name);
        string_concat (globalName[globalIndex],".");
      }
      string_concat (globalName[globalIndex],tk);
      writeASM (EXTERN_ASM,globalName[globalIndex],COMMENT);
    } else string_copy (globalName[globalIndex],tk);
    globalType[globalIndex]=s|t;
    readToken ();
    if (tk[0]==':') {
      readToken ();
      r=decimalToInt (tk);
      globalName[globalIndex][PARAM1]=r;
      readToken ();
      if (tk[0]!=':') return 0;
      readToken ();
      r=decimalToInt (tk);
      globalName[globalIndex][PARAM2]=r;
      readToken ();
    }
    globalIndex=globalIndex+1;
    if (tk[0]!=',') break;
  }
  return -1;
}

int8 syntaxImport (int8 *name) {
  name[0]=0;
  readToken ();
  if (type (tk[0])!=LETTER) name[0]=0;
  else {
    string_concat (name,tk);
    readToken ();
  }
  if (tk[1]) return 0;
  if (tk[0]=='(') {
    if (!syntaxFunctions (EXTERN,name)) return 0;
    if (tk[0]!=')') return 0;
    readToken ();
  }
  if (tk[0]==';') return -1;
  return 0;
}

int8 syntaxUnitIdentifier (int8 *name) {
#ifdef DEBUG
    string_write (STDOUT,"Function syntaxUnitIdentifier \'");
    string_write (STDOUT,tk);
    string_write (STDOUT,"\'\n");
#endif

  name[0]=0;
  for (;;) {
    readToken ();
    if (type (tk[0])!=LETTER) return 0;
    string_concat (name,tk);
    readToken ();
    if (tk[1]) return 0;
    if (tk[0]==';') return -1;
    if (tk[0]!='.') return 0;
    string_concat (name,tk);
  }
}

int8 syntaxUnit () {
  readToken ();
#ifdef DEBUG
    string_write (STDOUT,"Function syntaxUnit \'");
    string_write (STDOUT,tk);
    string_write (STDOUT,"\'\n");
#endif
  if (string_compare (tk,"unit")) return 0;
  if (!syntaxUnitIdentifier (unitName)) return 0;
  writeASM (UNIT,unitName,COMMENT);
  writeASM (TEXT,NOP0,COMMENT);
  writeASM (GLOBAL0,unitName,COMMENT);
  for (numberOfExternalUnits=0;;numberOfExternalUnits++) {
    readToken ();
    if (string_compare (tk,"import")) break;
    if (!syntaxImport (externalUnit[numberOfExternalUnits]))
      return 0;
  }
  writeASM (NOP0,NOP0,COMMENT);
  if (!string_compare (tk,"functions")) {
    if (!syntaxFunctions (0,"")) return 0;
    if (string_compare (tk,";")) return 0;
    readToken ();
  }

  for (;;) {
    if (string_compare (tk,"define")) break;
    readToken ();
  if (!syntaxDeclaration (-1,tk)) return 0;
    readToken ();
  }
  writeASM (DATA,NOP0,COMMENT);
  for (;;) {
    if (!syntaxDeclaration (-1,tk)) break;
    readToken ();
  }
  writeASM (TEXT,NOP0,COMMENT);
  for (;;) {
    if (string_compare (tk,"function")) break;
    if (!syntaxFunction ()) return 0;
    readToken ();
  }
  writeASM (LMAIN,unitName,COMMENT);
  writeASM (CALLS,unitName,COMMENT);
  writeASM (EXIT,NOP0,COMMENT);
  writeASM (COMMENT,NOP0,COMMENT);
  if (useCCALL) {
    writeASM (CCALL0,NOP0,COMMENT);
    writeASM (CCALL1,NOP0,COMMENT);
    writeASM (CCALL2,NOP0,COMMENT);
    writeASM (COMMENT,NOP0,COMMENT);
  }
  readToken ();
  if (tk[0]) return 0;
  return -1;
}

int main (int argc,char *argv[]) {
  if (argc<3) {
    string_write (STDOUT,"usage: \'lmsc sourcefile nasmfile\'\n");
    return 1;
  }
  fileSource=system_open (argv[1],O_RDONLY,0);
  fileNASM=system_open (argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666);
  
  useCCALL=0;
  numberOfExternalUnits=0;
  tokenIndex=0;
  blockIndex=0;
  variableIndex=0;
  globalIndex=0;
  labelIndex=0;
  lineNumber=1;
  unreadCharacter[0]=0;
  unreadToken[0]=0;
  comment=NOP0;

  if (!syntaxUnit ()) {
    string_write (STDOUT,"Syntax error line ");
    string_write (STDOUT,string_decimal (lineNumber));
    string_write (STDOUT,".\n-->");
    string_write (STDOUT,line);
    string_write (STDOUT,"<--\n");
    return 2;
  }
  writeASM (NOP0,NOP0,COMMENT);
  system_close (fileNASM);
  system_close (fileSource);
  return 0;
}
