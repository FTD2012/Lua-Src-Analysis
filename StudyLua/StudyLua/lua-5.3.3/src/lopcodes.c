/+ACo-
+ACoAKg- +ACQ-Id: lopcodes.c,v 1.55 2015/01/05 13:48:33 roberto Exp +ACQ-
+ACoAKg- Opcodes for Lua virtual machine
+ACoAKg- See Copyright Notice in lua.h
+ACo-/

+ACM-define lopcodes+AF8-c
+ACM-define LUA+AF8-CORE

+ACM-include +ACI-lprefix.h+ACI-


+ACM-include +ADw-stddef.h+AD4-

+ACM-include +ACI-lopcodes.h+ACI-


/+ACo- ORDER OP +ACo-/

LUAI+AF8-DDEF const char +ACo-const luaP+AF8-opnames+AFs-NUM+AF8-OPCODES+-1+AF0- +AD0- +AHs-
  +ACI-MOVE+ACI-,
  +ACI-LOADK+ACI-,
  +ACI-LOADKX+ACI-,
  +ACI-LOADBOOL+ACI-,
  +ACI-LOADNIL+ACI-,
  +ACI-GETUPVAL+ACI-,
  +ACI-GETTABUP+ACI-,
  +ACI-GETTABLE+ACI-,
  +ACI-SETTABUP+ACI-,
  +ACI-SETUPVAL+ACI-,
  +ACI-SETTABLE+ACI-,
  +ACI-NEWTABLE+ACI-,
  +ACI-SELF+ACI-,
  +ACI-ADD+ACI-,
  +ACI-SUB+ACI-,
  +ACI-MUL+ACI-,
  +ACI-MOD+ACI-,
  +ACI-POW+ACI-,
  +ACI-DIV+ACI-,
  +ACI-IDIV+ACI-,
  +ACI-BAND+ACI-,
  +ACI-BOR+ACI-,
  +ACI-BXOR+ACI-,
  +ACI-SHL+ACI-,
  +ACI-SHR+ACI-,
  +ACI-UNM+ACI-,
  +ACI-BNOT+ACI-,
  +ACI-NOT+ACI-,
  +ACI-LEN+ACI-,
  +ACI-CONCAT+ACI-,
  +ACI-JMP+ACI-,
  +ACI-EQ+ACI-,
  +ACI-LT+ACI-,
  +ACI-LE+ACI-,
  +ACI-TEST+ACI-,
  +ACI-TESTSET+ACI-,
  +ACI-CALL+ACI-,
  +ACI-TAILCALL+ACI-,
  +ACI-RETURN+ACI-,
  +ACI-FORLOOP+ACI-,
  +ACI-FORPREP+ACI-,
  +ACI-TFORCALL+ACI-,
  +ACI-TFORLOOP+ACI-,
  +ACI-SETLIST+ACI-,
  +ACI-CLOSURE+ACI-,
  +ACI-VARARG+ACI-,
  +ACI-EXTRAARG+ACI-,
  NULL
+AH0AOw-


+ACM-define opmode(t,a,b,c,m) (((t)+ADwAPA-7) +AHw- ((a)+ADwAPA-6) +AHw- ((b)+ADwAPA-4) +AHw- ((c)+ADwAPA-2) +AHw- (m))

LUAI+AF8-DDEF const lu+AF8-byte luaP+AF8-opmodes+AFs-NUM+AF8-OPCODES+AF0- +AD0- +AHs-
/+ACo-       T  A    B       C     mode		   opcode	+ACo-/
  opmode(0, 1, OpArgR, OpArgN, iABC)		/+ACo- OP+AF8-MOVE +ACo-/
 ,opmode(0, 1, OpArgK, OpArgN, iABx)		/+ACo- OP+AF8-LOADK +ACo-/
 ,opmode(0, 1, OpArgN, OpArgN, iABx)		/+ACo- OP+AF8-LOADKX +ACo-/
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/+ACo- OP+AF8-LOADBOOL +ACo-/
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/+ACo- OP+AF8-LOADNIL +ACo-/
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/+ACo- OP+AF8-GETUPVAL +ACo-/
 ,opmode(0, 1, OpArgU, OpArgK, iABC)		/+ACo- OP+AF8-GETTABUP +ACo-/
 ,opmode(0, 1, OpArgR, OpArgK, iABC)		/+ACo- OP+AF8-GETTABLE +ACo-/
 ,opmode(0, 0, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-SETTABUP +ACo-/
 ,opmode(0, 0, OpArgU, OpArgN, iABC)		/+ACo- OP+AF8-SETUPVAL +ACo-/
 ,opmode(0, 0, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-SETTABLE +ACo-/
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/+ACo- OP+AF8-NEWTABLE +ACo-/
 ,opmode(0, 1, OpArgR, OpArgK, iABC)		/+ACo- OP+AF8-SELF +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-ADD +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-SUB +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-MUL +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-MOD +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-POW +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-DIV +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-IDIV +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-BAND +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-BOR +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-BXOR +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-SHL +ACo-/
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-SHR +ACo-/
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/+ACo- OP+AF8-UNM +ACo-/
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/+ACo- OP+AF8-BNOT +ACo-/
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/+ACo- OP+AF8-NOT +ACo-/
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/+ACo- OP+AF8-LEN +ACo-/
 ,opmode(0, 1, OpArgR, OpArgR, iABC)		/+ACo- OP+AF8-CONCAT +ACo-/
 ,opmode(0, 0, OpArgR, OpArgN, iAsBx)		/+ACo- OP+AF8-JMP +ACo-/
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-EQ +ACo-/
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-LT +ACo-/
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/+ACo- OP+AF8-LE +ACo-/
 ,opmode(1, 0, OpArgN, OpArgU, iABC)		/+ACo- OP+AF8-TEST +ACo-/
 ,opmode(1, 1, OpArgR, OpArgU, iABC)		/+ACo- OP+AF8-TESTSET +ACo-/
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/+ACo- OP+AF8-CALL +ACo-/
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/+ACo- OP+AF8-TAILCALL +ACo-/
 ,opmode(0, 0, OpArgU, OpArgN, iABC)		/+ACo- OP+AF8-RETURN +ACo-/
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/+ACo- OP+AF8-FORLOOP +ACo-/
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/+ACo- OP+AF8-FORPREP +ACo-/
 ,opmode(0, 0, OpArgN, OpArgU, iABC)		/+ACo- OP+AF8-TFORCALL +ACo-/
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/+ACo- OP+AF8-TFORLOOP +ACo-/
 ,opmode(0, 0, OpArgU, OpArgU, iABC)		/+ACo- OP+AF8-SETLIST +ACo-/
 ,opmode(0, 1, OpArgU, OpArgN, iABx)		/+ACo- OP+AF8-CLOSURE +ACo-/
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/+ACo- OP+AF8-VARARG +ACo-/
 ,opmode(0, 0, OpArgU, OpArgU, iAx)		/+ACo- OP+AF8-EXTRAARG +ACo-/
+AH0AOw-

