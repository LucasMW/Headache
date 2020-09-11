#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>

#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#if !defined(codeEss_h)
	#include "codeEss.h"
	#define codeEss_h
#endif
#if !defined(compilerFunctions_h)
	#include "compilerFunctions.h"
	#define compilerFunctions_h
#endif
#if !defined(symbolTable_h)
	#include "symbolTable.h"
	#define symbolTable_h
#endif
#if !defined(optimizer_h)
	#include "optimizer.h"
	#define optimizer_h
#endif
#define REGISTER_COUNT 5

void codeDefVar(DefVar* dv);
void codeDefFunc(DefFunc* df);
void codeDefVarList(DefVarL* dvl);
void codeDefList(Def* d);

void codeType(Type* t);
void codeParams(Parameter* params);
void codeForAllocParams(Parameter* params);
void codeForDeAllocParams(Parameter* params);
void codeCommandList(CommandL* cl);
int codeExpAccess(Exp* e);
void codeBlock(Block* b);
int codeExp(Exp* e);
int codeExpOperator(Exp* e);
void codeConstant(Constant* c);
void codeExpList(ExpList* el);
int codeAccessElemPtr(Exp* e);
char* stringForType(Type* t);



static void codePrint(const char* str);
static int unitsToMoveTo(int cellAddr);
static void codeGoTo(int cellIndex);
static void codeZero(int x);
int allocateCellsForType(Type* t);


static FILE* output = NULL;


const int registerCount = REGISTER_COUNT;
int currentTempRegs[REGISTER_COUNT] = {0,1,2,3,4};
static int currentAllocationIndex = REGISTER_COUNT;
static int currentCell = 0;

//static char* memory[30000]; //debugs and controls free memory

static int currentFunctionTIndex = 0;
static int currentBrIndex = 0;

static int currentStringConstant = 0;
static int declareTop = 0;
char* stringsToDeclare[100];


extern char forceExpand; 

static int pushCells(int x);
static int popCells(int x);
static int currentFuncHasReturn = 0;

static char DBG_File = 1;
static char DBG_Console = 0;
static void codeDebugMessage(const char* str){
	if(DBG_File){
		fprintf(output, "%s\n",str);
	}
	if(DBG_Console){
		fprintf(stdout, "%s\n",str);
	}
}


void bfalgo(char* str, ...){

//based on http://www.gnu.org/software/libc/manual/html_node/Variadic-Example.html
  va_list ap;
  int i;
  int count = 0;

  for(i=0;str[i];i++){
  	if(str[i] == '$' || str[i] == '@') {
  		count++;
  	}
  }
  //printf("count %d\n",count );
  va_start (ap, str);         /* Initialize the argument list. */

  for(i=0;str[i];i++){
  	if(str[i] == '$') {
  		int d =  va_arg (ap, int);
  		codeGoTo(d);
  	} else if (str[i] == '@'){ // Not sure if I will use
  		char* str = va_arg(ap,char*);
  		codeStr(str);
  	}
  	else {
  		fprintf(output, "%c",str[i]);
  	}
  }
  va_end (ap);                  /* Clean up. */
}

typedef struct slice{
	int* start;
	int* end;
	char state;
} Slice;



int allocateCellsForType(Type* t){
	currentAllocationIndex = pushCells(cellsForType(t));
	return currentAllocationIndex;
}
static int pushCells(int x){
	// for(int i=0;i<x;i++){
	// 	memory[currentAllocationIndex+i] = 1;
	// }
	currentAllocationIndex+=x;
	return currentAllocationIndex;
}
static int popCells(int x){
	// for(int i=0;i<x;i++){
	// 	memory[currentAllocationIndex-i] = 0;
	// }
	currentAllocationIndex-=x;
	return currentAllocationIndex;
}






static void codePrint(const char* str) {
	int preserve = currentCell;
	codeGoTo(0);
	codeZero(currentCell);
	char last = 0;
	char move = '?';
	for(char* p=(char*)str;*p;p++){
		char diff = *p - last;
		if(diff < 0) {
			move = '-';
			diff = -diff;
		}
		else {
			move = '+';
		}
		for(;diff>0;diff--) {
			fprintf(output, "%c",move);
		}
		fprintf(output, ".");
		last = *p;
	}
	codeZero(0);
	codeGoTo(preserve);
}



static int unitsToMoveTo(int cellAddr) {
	//printf("%d vs %d \n", cellAddr, currentCell );
	return cellAddr - currentCell;
}

static void codeGoTo(int cellIndex) {
	int units = unitsToMoveTo(cellIndex);
	char direction = '>';
	int count=0;
	assert(currentCell + units == cellIndex);
	if(units < 0) {
		direction = '<';
		units = -units;
	}
	assert(units < 30000);
	currentCell += units;
	//printf("units: %d\n",units );
	for(;units>0;units--) {
		count++;
		fprintf(output, "%c",direction);
	}
	currentCell = cellIndex;
}


	
static void codeZero(int x) {
	codeGoTo(x); codeStr("[-]");
}

/*	temp0[-]
		y[x+temp0+y-]
		temp0[y+temp0-]	*/

static void moveXToY(int x, int y){
	codeZero(y);
	bfalgo("$[$+$-]",x,y,x);
}


static void codeConstantIncrement(int x, int q){
	codeGoTo(x);
	while(q){
		bfalgo("+");
		q--;
	}
}
static void codeConstantDecrement(int x, int q){
	codeGoTo(x);
	while(q){
		bfalgo("-");
		q--;
	}
}

static void incrementXbyY2(int x, int y){
	int temp0 = pushCells(1);
	bfalgo("$[-] $[$+$+$-] $[$+$-]",temp0,y,x,temp0,y,
		temp0,y,temp0);
	popCells(1);
}
static void incrementXbyY(int x,int y) {
	//printf("add %d to %d\n", x,y);
	int temp0 = pushCells(1);
	codeGoTo(temp0); codeStr("[-]");
	codeGoTo(y); codeStr("["); codeGoTo(x); codeStr("+"); 
	codeGoTo(temp0); codeStr("+"); codeGoTo(y); codeStr("-]");
	codeGoTo(temp0); codeStr("[");  codeGoTo(y); codeStr("+"); 
	codeGoTo(temp0); codeStr("-]");
	popCells(1);
}

/*
temp0[-]
y[x-temp0+y-]
temp0[y+temp0-]
*/
static void decrementXbyY(int x,int y) {
	//printf("sub %d to %d\n", x,y);
	int temp0 = currentTempRegs[2];
	codeGoTo(temp0); codeStr("[-]");
	codeGoTo(y); codeStr("["); codeGoTo(x); codeStr("-"); codeGoTo(temp0); codeStr("+"); codeGoTo(y); codeStr("-]");
	codeGoTo(temp0); codeStr("[");  codeGoTo(y); codeStr("+"); codeGoTo(temp0); codeStr("-]");
}
	/*	temp0[-]
	x[-]
	y[x+temp0+y-]
	temp0[y+temp0-]	*/
void setXtoY(int x, int y) {
	printf("set %d to %d\n",x,y );
	int temp0 = currentTempRegs[2];
	printf("temp %d\n",temp0 );
	codeGoTo(temp0); codeStr("[-]");
	codeGoTo(x); codeStr("[-]");
	codeGoTo(y); codeStr("["); codeGoTo(x); codeStr("+"); codeGoTo(temp0); codeStr("+"); codeGoTo(y); codeStr("-]");
	codeGoTo(temp0); codeStr("["); codeGoTo(y); codeStr("+"); codeGoTo(temp0); codeStr("-]");
}
	/* temp0[-]
	temp1[-]
	x[temp1+x-]
	temp1[
	 y[x+temp0+y-]
	 temp0[y+temp0-]
	temp1-] */

static void multiplyXbyY(int x, int y){
	int temp0 = currentTempRegs[0];
	int temp1 = currentTempRegs[1];
	printf("mult %d to %d\n",x,y );
	printf("temp0:%d temp1:%d \n",temp0,temp1 );
	codeZero(temp0);
	codeZero(temp1);
	codeGoTo(x); codeStr("["); codeGoTo(temp1); codeStr("+"); codeGoTo(x); codeStr("-]");
	codeGoTo(temp1); codeStr("[");
	codeGoTo(y); codeStr("["); codeGoTo(x); codeStr("+"); codeGoTo(temp0);codeStr("+"); codeGoTo(y); codeStr("-]");
	codeGoTo(temp0); codeStr("["); codeGoTo(y); codeStr("+"); codeGoTo(temp0); codeStr("-]");
	codeGoTo(temp1); codeStr("-]");
}
/*
temp0[-]
temp1[-]
temp2[-]
temp3[-]
x[temp0+x-]
temp0[
 y[temp1+temp2+y-]
 temp2[y+temp2-]
 temp1[
  temp2+
  temp0-[temp2[-]temp3+temp0-]
  temp3[temp0+temp3-]
  temp2[
   temp1-
   [x-temp1[-]]+
  temp2-]
 temp1-]
 x+
temp0]
*/
static void divideXbyY(int x,int y){
	printf("divide\n");
	int temp0=currentTempRegs[0];
	int temp1=currentTempRegs[1];
	int temp2=currentTempRegs[2];
	int temp3=currentTempRegs[3];
	bfalgo("$[-]$[-]$[-]$[-] $[$+$-] $[ $[$+$+$-] $[$+$-] $[ $+ $-[$[-]$+$-] $[$+$-] $[$-[$-$[-]]+$-]$-]$+$]",
		temp0,temp1,temp2,temp3
		,x,temp0,x
		,temp0,
		y,temp1,temp2,y
		,temp2,y,temp2,
		temp1,
		temp2,temp0,temp2,temp3,temp0,
		temp3,temp0,temp3,
		temp2,
		temp1,
		x,temp1,
		temp2,
		temp1,
		x,
		temp0);
	//fprintf(output, "%s[-]%s[-]%s[-]%s[-]x[%s+x-]%s[y[%s+%s+y-]%s[y+%s-]%s[%s+%s-[%s[-]%s+%s-]%s[%s+%s-]%s[%s-[x-%s[-]]+%s-]%s-]x+%s]\n", );
}


//# 0 >n d 0 0 0
//[>->+<[>]>[<+>-]<<[<]>-]
//# 0 >0 d-n%d n%d 0 0

// x = x % y
void modulus(int x,int y){
	printf("mod %d = %d %% %d\n", x,x,y);
	printf("%d \n", currentAllocationIndex);
	pushCells(10);
	int algo_start = currentAllocationIndex-6;
	printf("mod start %d\n", algo_start);
	incrementXbyY(algo_start+1,x);
	incrementXbyY(algo_start+2,x);
	bfalgo("$[>+>->+<[>]>[<+>-]<<[<]>-]$$",
		algo_start+1,algo_start+2,algo_start+3);
	char dbg[50];
	sprintf(dbg,"value should should be at cell %d \n",algo_start+3);
	codeDebugMessage(dbg);
	codeDebugMessage("Stop here");
	bfalgo("$[-]$[$+$-]$",x,algo_start+3,x,algo_start+3,x);
	popCells(10);
	printf("currentAllocationIndex %d vs x %d, vs algo_start+3 %d \n", currentAllocationIndex,x, algo_start+3 );
}

/*temp0[-]
x[temp0+x-]
y[x+y-]
temp0[y+temp0-] */
void swapXbyY(int x, int y){
	int temp0=currentTempRegs[0];
	codeGoTo(temp0);codeStr("[-]");
	codeGoTo(x);codeStr("[");codeGoTo(temp0);codeStr("+");codeGoTo(x);codeStr("-]");
	codeGoTo(y);codeStr("[");codeGoTo(x);codeStr("+");codeGoTo(y);codeStr("-]");
	codeGoTo(temp0); codeStr("["); codeGoTo(y); codeStr("+"); codeGoTo(temp0); codeStr("-]");
}

/*temp0[-]
x[temp0-x-]
temp0[x-temp0+] */
void sigswitchX(int x){
	int temp0=currentTempRegs[0];
	codeZero(temp0);
	codeGoTo(x);codeStr("[");codeGoTo(temp0);codeStr("-");codeGoTo(x);codeStr("-]");
	codeGoTo(temp0);codeStr("[");codeGoTo(x);codeStr("-");codeGoTo(temp0);codeStr("+]");
}

/*
Attribution: Jeffry Johnston

The algorithm returns either 0 (false) or 1 (true).
temp0[-]
x[temp0+x[-]]+
temp0[x-temp0-]*/
void logicalnot(int x){
	int temp0=currentTempRegs[0];
	bfalgo("$[-]$[$+$[-]]+$[$-$-]",temp0,x,temp0,x,temp0,x,temp0);
}

/* temp0[-]
temp1[-]
x[temp1+x-]+
y[temp1-temp0+y-]
temp0[y+temp0-]
temp1[x-temp1[-]]*/
void equals(int x,int y){
	int temp0 = currentTempRegs[0];
	int temp1 = currentTempRegs[1];
	bfalgo("$[-]$[-]$[$+$-]+$[$-$+$-]$[$+$-]$[$-$[-]]",
		temp0,temp1,
		x,temp1,x,
		y,temp1,temp0,y,
		temp0,y,temp0,
		temp1,x,temp1);
}

//x[-y-x]+y[x-y[-]] does not preserve x and y
void equals2(int x,int y){
	bfalgo("$[-$-$]+$[$-$[-]]",x,y,x,y,x,y);
}
/*
Attribution: Jeffry Johnston

The algorithm returns either 0 (false) or 1 (true).
temp0[-]
temp1[-]
x[temp1+x-]
y[temp1-temp0+y-]
temp0[y+temp0-]
temp1[x+temp1[-]] */
//x = x != y
void unequals(int x,int y){
	int temp0 = currentTempRegs[0];
	int temp1 = currentTempRegs[1];
	bfalgo("$[-]$[-]$[$+$-]$[$-$+$-]$[$+$-]$[$+$[-]]",
		temp0,temp1,
		x,temp1,x,
		y,temp1,temp0,y,
		temp0,y,temp0,
		temp1,x,temp1);
	codeDebugMessage("unequals");
}
/*Attribution: Yuval Meshorer

Sets x to be 1 if x == y, 0 otherwise.
x[
 y-x-]
y[[-]
 x+y]*/
//x = x != y
void unequals2(int x, int y){
	bfalgo("$[$-$-]$[[-]$+$]",x,y,x,y,x,y);
}


/*
Attribution: User:ais523

temp0[-]
temp1[-] >[-]+ >[-] <<
y[temp0+ temp1+ y-]
temp1[y+ temp1-]
x[temp1+ x-]
temp1[>-]> [< x+ temp0[-] temp1>->]<+<
temp0[temp1- [>-]> [< x+ temp0[-]+ temp1>->]<+< temp0-]
The temporaries and x are left at 0; y is set to y-x. 
*/
//z = x > y


void greater(int x, int y, int z){
	int temp0 = currentTempRegs[0];
	int temp1 = currentTempRegs[1];
	bfalgo("$[-]$[-]$[-]$[ $+$[- $[-] $+ $]$[- $+ $]$[- $+ $]$- $- ]",
		temp0,temp1,z,x,temp0,y,temp0,temp1,y,temp0,z,temp0,temp1,y,temp1,y,x);
	//generated line
	codeDebugMessage("greater");
}
/*
Attribution: Ian Kelly

x and y are unsigned. 
temp1 is the first of three consecutive temporary cells. 
The algorithm returns either 0 (false) or 1 (true).

temp0[-]
temp1[-] >[-]+ >[-] <<
y[temp0+ temp1+ y-]
temp1[y+ temp1-]
x[temp1+ x-]
temp1[>-]> [< x+ temp0[-] temp1>->]<+<
temp0[temp1- [>-]> [< x+ temp0[-]+ temp1>->]<+< temp0-]*/

//x = x <= y
void lessequal(int x, int y){
	int temp0 = currentTempRegs[0];
	int temp1 = pushCells(3);
	bfalgo("$[-]$[-] >[-]+ >[-] <<$[$+ $+ $-]$[$+ $-]$[$+ $-]$[>-]> [< $+ $[-] $>->]<+< $[$- [>-]> [< $+ $[-]+ $>->]<+< $-]",
		temp0,temp1,y,temp0,temp1,y,temp1,y,temp1,x,temp1,x,temp1,x,temp0,temp1,temp0,temp1,x,temp0,temp1,temp0);
	//generated line
	codeDebugMessage("less or equal than");

	//popCells(3);
}
/* Attribution: Ian Kelly
x and y are unsigned. 
temp1 is the first of three consecutive temporary cells. 
The algorithm returns either 0 (false) or 1 (true).
temp0[-]
temp1[-] >[-]+ >[-] <<
y[temp0+ temp1+ y-]
temp1[y+ temp1-]
x[temp1+ x-]
temp1[>-]> [< x+ temp0[-] temp1>->]<+<
temp0[temp1- [>-]> [< x+ temp0[-]+ temp1>->]<+< temp0-] */
void less(int x, int y){
	int temp0 = currentTempRegs[0];
	int temp1 = pushCells(3);
	bfalgo("$[-]$[-] >[-]+ >[-] <<$[$+ $+ $-]$[$+ $-]$[$+ $-]$[>-]> [< $+ $[-] $>->]<+<$[$- [>-]> [< $+ $[-]+ $>->]<+< $-]",
		temp0,temp1,y,temp0,temp1,y,temp1,y,temp1,x,temp1,x,temp1,x,temp0,temp1,temp0,temp1,x,temp0,temp1,temp0);
	codeDebugMessage("less");
}
/*Attribution: Jeffry Johnston

The algorithm returns either 0 (false) or 1 (true).
temp0[-]
temp1[-]
x[temp1+x-]
temp1[
 temp1[-]
 y[temp1+temp0+y-]
 temp0[y+temp0-]
 temp1[x+temp1[-]]
]*/
//x = x and y
void and(int x, int y){
	int temp0 = currentTempRegs[0];
	int temp1 = currentTempRegs[1];
	bfalgo("$[-]$[-]$[$+$-]$[$[-]$[$+$+$-]$[$+$-]$[$+$[-]]]",
		temp0,temp1,x,temp1,x,temp1,temp1,y,temp1,temp0,y,temp0,y,temp0,temp1,x,temp1);
	//generated line
}
/* Attribution: Sunjay Varma

Consumes x and y (leaves them as zero at the end of the algorithm) and stores the result in z. For short-circuit evaluation, don't evaluate x or y until just before they are used.

The algorithm returns either 0 (false) or 1 (true).
z[-]
x[
 y[z+y-]
 x-
]
y[-]
*/
//z = x and y
void and2(int x, int y, int z){
	bfalgo("$[-]$[$[$+$-]$-]$[-]",z,x,y,z,y,x,y);
	//generated line
}
/*
z = x or y (boolean, logical)
Attribution: Sunjay Varma

Consumes x and y (leaves them as zero at the end of the algorithm) and stores the result in z. For short-circuit evaluation, don't evaluate x or y until just before they are used.

If you don't care about short-circuit evaluation, temp0 can be removed completely. If temp0 is removed and both x and y are 1, z will be 2, not 1. This is usually not a problem since it is still non-zero, but you should keep that in mind.

The algorithm returns either 0 (false) or 1 (true).
z[-]
temp0[-]+
x[
 z+
 temp0-
 x-
]
temp0[-
 y[
  z+
  y-
 ]
]
y[-]*/
void or(int x, int y, int z){
	int temp0 = currentTempRegs[0];
	bfalgo("$[-]$[-]+$[$+$-$-]$[-$[$+$-]]$[-]",z,temp0,x,z,temp0,x,temp0,y,z,y,y);
}

/*
Attribution: Yuval Meshorer

Consumes x and y, does not use a temporary cell. Makes z 1 (true) or 0 (false) if either x or y are one.
z[-]
x[y+x-]
y[[-]
z+y]*/
void or2(int x,int y, int z){
	bfalgo("$[-]$[$+$-]$[[-]$+$]",z,x,y,x,y,z,y);
}
/*
Attribution: Yuval Meshorer

Consumes x and y. Makes z 1 (true) or 0 (false) if x does not equal y. Finishes at y.
 z[-]
x[y-
 x-]
y[z+
 y[-]] */
void xor(int x, int y, int z){
	bfalgo("$[-]$[$-$-]$[$+$[-]]",z,x,y,x,y,z,y);
}  

/*read into cell x
by Urban Müller (1993)

Value is input into the current cell, uses three more cells to the right. End of the number is a newline, or eof. All other character are treated the same as digits. Works correctly with bignum cells.

   [-]>[-]+    // Clear sum
   [[-]                // Begin loop on first temp
   >[-],               // Clear the inp buffer to detect leave on eof and input
       [
           +[                          // Check for minus one on eof
               -----------[            // Check for newline
                   >[-]++++++[<------>-]       // Subtract 38 to get the char in zero to nine
                   <--<<[->>++++++++++<<]      // Multiply the existing value by ten
                   >>[-<<+>>]          // and add in the new char
               <+>]
           ]
       ]
   <]
   <
   // Current cell is the number input*/



void read(int x){
	pushCells(10);
	bfalgo("$[-]>[-]+[[-]>[-],[+[-----------[>[-]++++++[<------>-]<--<<[->>++++++++++<<]>>[-<<+>>]<+>]]]<]<",
		currentAllocationIndex-4);
	codeZero(x);
	incrementXbyY2(x,currentAllocationIndex-4);
	codeZero(currentAllocationIndex-4);
	popCells(10);
}

static void codeCellValuePrint(int start, int end){
	pushCells(10);
	incrementXbyY2(currentAllocationIndex-5,start);
	bfalgo("$[$+$+$-]$",currentAllocationIndex-5,currentAllocationIndex-6,currentAllocationIndex-7,currentAllocationIndex-5,currentAllocationIndex-6);
	codeStr(">>++++++++++<<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>>[-]>>>++++++++++<[->-[>+>>]>[+[-<+>]>+>>]<<<<<]>[-]>>[>++++++[-<++++++++>]<.<<+>+>[-]]<[<[->-<]++++++[->++++++++<]>.[-]]<<++++++[-<++++++++>]<.[-]<<[-<+>]<");
	bfalgo("$[$+$-]$",currentAllocationIndex-7,currentAllocationIndex-5,currentAllocationIndex-7,currentAllocationIndex-5);
	bfalgo("$[-]$[-]$[-]",currentAllocationIndex-5,currentAllocationIndex-6,currentAllocationIndex-7);
	popCells(10);
}

static void codeCellValuePrintAnySize(int start, int end){
	printf("priting cell %d\n", start);
	pushCells(10);
	incrementXbyY2(currentAllocationIndex-5,start);
	bfalgo("$[$+$+$-]$",currentAllocationIndex-5,currentAllocationIndex-6,currentAllocationIndex-7,currentAllocationIndex-5,currentAllocationIndex-6);
	codeStr("[>>+>+<<<-]>>>[<<<+>>>-]<<+>[<->[>++++++++++<[->-[>+>>]>[+[-<+>]>+>>]<<<<<]>[-]++++++++[<++++++>-]>[<<+>>-]>[<<+>>-]<<]>]<[->>++++++++[<++++++>-]]<[.[-]<]<");
	bfalgo("$[$+$-]$",currentAllocationIndex-7,currentAllocationIndex-5,currentAllocationIndex-7,currentAllocationIndex-5);
	bfalgo("$[-]$[-]$[-]",currentAllocationIndex-5,currentAllocationIndex-6,currentAllocationIndex-7);
	popCells(10);
}


void setCodeOutput(FILE* out) {
	setOutput(out);
	output = out;
} 

static void defaultOutput() {
	output = stdout;
}


static Parameter* currentParameters = NULL;









static void codeSignature() {
	//char version[] = "v_alpha_0_1";
	fprintf(output, "Generated by HAC (Headache Awesome Compiler) \n" );
	//fprintf(output, "HAC version %s\n",version);
	//fprintf(output, "[More information at %s] \n",link);

}

void codeTree() {
	if(output==NULL) {
		defaultOutput();
	}
	currentStringConstant = 0;
	printf("generating code for tree\n");
	codeSignature();
	codeDefList(globalTree->next);
} 
void codeDefVar(DefVar* dv) {
	dv->start_cell = currentAllocationIndex += cellsForType(dv->t);
	printf("var address: %d\n",dv->start_cell);	

}
void codeDefFunc(DefFunc* df) {


	if(checkCompilerFunctions(df->id) >= 0){
		codeCompilerFunctions(df->id);
	}
	if(df->b) {
		currentFunctionTIndex = 0;
		currentParameters = df->params;
		declareTop = 0;
		currentFuncHasReturn = 0;
		currentBrIndex = 0;
		codeBlock(df->b);
	}
}
void codeDefVarList(DefVarL* dvl) {

	DefVarL* p = dvl;
	while(p) {
		codeDefVar(p->dv);
		p=p->next;
	}
}
void codeDefList(Def* d) {
	if(!d)
		return;
	//printf("coding DefList\n");
	switch (d->tag) {
		case DVar:
			codeDefVarList(d->u.v);
		break;
		case DFunc:
			if(strcmp(d->u.f->id,"main")==0) //only code main 
				codeDefFunc(d->u.f); //definition of main leaves implicit main call
		break;
	}
	codeDefList(d->next);

}

void codeType(Type* t);
void codeParams(Parameter* params) {
	// if(!params)
	// 	return;
	// if(params->next) {
	// 	params->start_cell = currentAllocationIndex+=2;
	// 	codeParams(params->next);
	// }
}
void codeForAllocParams(Parameter* params) {
	if(!params)
		return;
	
	Parameter* p = params;
	while(p) {
		p->start_cell = pushCells(cellsForType(p->t));
		p = p->next;
	}
	
}
void codeForDeAllocParams(Parameter* params) {
	if(!params)
		return;
	
	Parameter* p = params;
	while(p) {
		popCells(cellsForType(p->t));
		p = p->next;
	}
	

}

int codeCond(Exp* e) {
	int i1;
	int temp = currentTempRegs[4];
	i1 = codeExp(e);
	codeZero(temp);
	incrementXbyY2(temp,i1);
	//unequals(i1,temp0);
	
	codeDebugMessage("CodeCond");
	return temp;
}
void codeCommandList(CommandL* cl) {
	codeDebugMessage("CL");
	if(!cl)
		return;
	CommandL* c = cl;
	//printf("CommandL\n");
	int i1,i2;
	//int b1,b2,b3;
	int temp0,temp1;
	while(c) {
		//printf("cl\n");
		switch(c->tag) {
			case CWhile:
				codeDebugMessage("begin while");
				temp0 = currentAllocationIndex;
			 	i1 = codeCond(c->condExp);
				bfalgo("$[\n",i1);
					codeCommandList(c->cmdIf);
				i2 = codeCond(c->condExp);
				bfalgo("$\n]",i2);
				
				codeDebugMessage("end while");
			break;
			case CFor:
				codeDebugMessage("begin for");
				codeCommandList(c->cFor.begin);
				temp0 = currentAllocationIndex;
			 	i1 = codeCond(c->cFor.cmpExp);
				bfalgo("$[\n",i1);
					codeCommandList(c->cFor.block);
					codeCommandList(c->cFor.end);
				i2 = codeCond(c->cFor.cmpExp);
				bfalgo("$\n]",i2);
				codeDebugMessage("end for");
			break;
			case CIf:
				temp0 = pushCells(1);
				codeZero(temp0);
				i1 = codeCond(c->condExp);
				bfalgo("$[",i1);
				codeCommandList(c->cmdIf);
				bfalgo("$]$",temp0,i1);


				// leaveScope();
			break;
			case CIfElse:
				i1 = codeCond(c->condExp);
				temp0 = pushCells(1);
				temp1 = pushCells(1);
				bfalgo("$[-]+$[-]$[",temp0,temp1,i1);
 				codeCommandList(c->cmdIf);
 				bfalgo("$-$[$+$-]]$[$+$-]$[",temp0,i1,temp1,i1,temp1,i1,temp1,temp0);
 				codeCommandList(c->cmdElse);
 				bfalgo("$-]",temp0);
 
			break;
			case CReturn:	
				//printf("cret\n");
				currentFuncHasReturn = 1;
				if(c->retExp == NULL) {
					fprintf(output, "ret void\n");
					codeDebugMessage("No Return");
				}
				else {
					i1 = codeExp(c->retExp);
					codeZero(currentTempRegs[0]);
					incrementXbyY2(currentTempRegs[0],i1);
					codeDebugMessage("Return");
				}
			break;
			case CAssign:
				 i1 = codeExp(c->expRight);
				 
				 /* lil Trick to add shorts and ints */
				 //printType(c->expLeft->type,0);
				 if(c->expLeft->type->b == WShort){
				 	//printf("forceExpand %d\n",forceExpand);
				 	forceExpand = forceExpand > 1 ? forceExpand : 1;
				 }
				 else if(c->expLeft->type->b == WInt){
				 	//printf("forceExpand %d\n",forceExpand);
				 	forceExpand = forceExpand > 2 ? forceExpand : 2;
				 }
				 else {
				 	//printf("don't forceExpand %d\n",forceExpand);
				 }
				 //printExp(c->expRight,0);
				 i2 = codeExp(c->expLeft);
				 //printExp(c->expRight,0);
				 temp0 = pushCells(1);
				 codeZero(temp0);
				 incrementXbyY2(temp0,i1);
				 moveXToY(i1,i2);
				 moveXToY(temp0,i1);
				 popCells(1);
				 //printf("%d %d",i1,i2);
				 codeDebugMessage("Assign");
			break;
			case CBlock:
				//printf("cblock\n");
				codeBlock((Block*)c->block );
				// leaveScope();
			break;
			case CCall:
				//printf("ccall\n");
				codeExp(c->expRight);
				codeDebugMessage("Call");
			break;
			case CPrint:
				i1 =  codeExp(c->printExp);
				if(c->printExp->type == NULL) {
					fprintf(output, ";printing void expression is unavaible\n" );
				}
				else if(c->printExp->type->tag == base) {
					switch(c->printExp->type->b) {
						case WInt:
						//fprintf(output, ">>++++++++++<<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>>[-]>>>++++++++++<[->-[>+>>]>[+[-<+>]>+>>]<<<<<]>[-]>>[>++++++[-<++++++++>]<.<<+>+>[-]]<[<[->-<]++++++[->++++++++<]>.[-]]<<++++++[-<++++++++>]<.[-]<<[-<+>]<");
						codeCellValuePrintAnySize(i1,i1);
						codeDebugMessage("print int");
						break;
						case WShort:
						//fprintf(output, ">>++++++++++<<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>>[-]>>>++++++++++<[->-[>+>>]>[+[-<+>]>+>>]<<<<<]>[-]>>[>++++++[-<++++++++>]<.<<+>+>[-]]<[<[->-<]++++++[->++++++++<]>.[-]]<<++++++[-<++++++++>]<.[-]<<[-<+>]<");
						codeCellValuePrintAnySize(i1,i1);
						codeDebugMessage("print Short");
						break;
						case WByte:
						//fprintf(output, ">>++++++++++<<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>>[-]>>>++++++++++<[->-[>+>>]>[+[-<+>]>+>>]<<<<<]>[-]>>[>++++++[-<++++++++>]<.<<+>+>[-]]<[<[->-<]++++++[->++++++++<]>.[-]]<<++++++[-<++++++++>]<.[-]<<[-<+>]");

						codeCellValuePrint(i1,i1);
						codeDebugMessage("print byte");

						break;
						case WFloat:
						codeDebugMessage("Not implemented");
						break;
						case WBit:
						temp0 = pushCells(1);
						temp1 = pushCells(1);
						bfalgo("$[-]+$[-]$[",temp0,temp1,i1);
		 				codePrint("true");
		 				bfalgo("$-$[$+$-]]$[$+$-]$[",temp0,i1,temp1,i1,temp1,i1,temp1,temp0);
		 				codePrint("false");
		 				bfalgo("$-]",temp0);
						codeDebugMessage("print bit");
						break;
						case WChar:
						bfalgo("$.",i1);
						codeDebugMessage("print char");
						break;
					}
				}
				else if(c->printExp->type->of->tag == base &&
					c->printExp->type->of->b == WByte) {
					codePrint(c->printExp->c->u.str);
					codeDebugMessage("Printing string");
				}
				else {
					fprintf(output, ";printing non string array is unavaible\n" );
				}
				// if (!checkPrintability(c->printExp)) {
				// 	typeError("Expression is not printable");
				// }
			break;
			case CRead:
				i1 =  codeExp(c->printExp);
				if(c->printExp->type->b == WChar) { 
					bfalgo("$,",i1);
				} else {
					read(i1);
				}
				codeDebugMessage("Read");
				 if(c->printExp->type->b == WShort){
				 	//printf("forceExpand %d\n",forceExpand);
				 	forceExpand = forceExpand > 1 ? forceExpand : 1;
				 }
				 else if(c->printExp->type->b == WInt){
				 	//printf("forceExpand %d\n",forceExpand);
				 	forceExpand = forceExpand > 2 ? forceExpand : 2;
				 }
			break; 
			case CDebug:
				codeStr("@");
				codeDebugMessage("Print All Memory");
			break; 
			case COperator:
				codeExp(c->oprExp);
				codeDebugMessage("Operator");
			break; 
		}
		c = c->next;
	}
}
int sizeOfDvl(DefVarL* dvl ){
	DefVarL* p=dvl;
	int sum = 0;
	while(p){
		sum += cellsForType(p->dv->t); 
		p=p->next;
	}
	return sum;

}
void codeBlock(Block* b) {
	// if(!b->limits) {
	// 	b->limits = (CellUsage*)malloc(sizeof(CellUsage));
	// 	b->limits->start = currentAllocationIndex++;
	// }
	codeDefVarList(b->dvl);
	codeCommandList(b->cl);

	//codeGoTo(b->limits->start);
}
int codeBinExp(Exp* e ,int* f) {
	int te1,te2; 
	te1 = codeExp(e->bin.e1 );
	//printExp(e->bin.e1,0);
	te2 = codeExp(e->bin.e2 );
	//printExp(e->bin.e2,0);
	//printf("\n%d,%d\n",te1,te2 );
	currentAllocationIndex += cellsForType(e->type);
	incrementXbyY(currentAllocationIndex,te1);
	incrementXbyY(currentAllocationIndex,te2);
	return currentAllocationIndex;
}
int codeCallExp(Exp* e) {

	DefFunc* df = (DefFunc*)e->call.def;
	ExpList *q = e->call.expList;
	Parameter *p = df->params;
	//calculate size
	//codeForAllocParams(df->params);
	while(q) {
		int temp = codeExp(q->e);
		//codeStr("@");
		p->start_cell = pushCells(cellsForType(p->t));
		incrementXbyY2 (p->start_cell,temp);
		//codeStr("@");
		printf("param %s at %d\n",p->id,p->start_cell );
		p = p->next;
		q = q->next;
	}

	codeDefFunc((DefFunc*)e->call.def);
	//codeForDeAllocParams(df->params);
	return currentTempRegs[0];		
}


int codeExpPrim(Exp* e) {
	//char* tStr = stringForType(e->type);
	if(e->c->tag == KStr) {


		return currentFunctionTIndex;
	}
	
	//char* cStr = stringForConstant(e->c);
	
	if(e->c->tag == KFloat) {
		
	} else {
		if(e->c->start_cell == 0) {
			e->c->start_cell = pushCells(1);
			printf("kint at %d\n",currentAllocationIndex);
			char c = '+';
			int number = e->c->u.i;
			codeZero(e->c->start_cell);
			for(int i=0; i<number;i++) {
				fprintf(output, "%c", c);
			}
		}
		codeGoTo(e->c->start_cell);
		
		//codeGoTo(preserve);
		codeDebugMessage("KInt");
	}
	//int index = currentAllocationIndex;
	return e->c->start_cell;
}
int getAddressOfVar(Var* id) {
	return -1;
}
int codeExpVar(Exp* e) {
	currentFunctionTIndex++;
	if(e->var->declaration == NULL)
	{
		//printf(";params\n");
		Parameter* p = currentParameters;
		int t=0;
		while(p) {
			if(strcmp(e->var->id,p->id)==0)
				break;
			p=p->next;
			t++;
		}
		return p->start_cell;

	}
	else {
		return e->var->declaration->start_cell;
		
	}
	return currentFunctionTIndex;
}
int codeExpUnary(Exp* e) {
	int i1,i2;
	i1 = codeExp(e->unary.e);
	switch(e->unary.op) {
		case MINUS:
			i2 = pushCells(cellsForType(e->type));
			codeZero(i2);
			incrementXbyY2(i2,i1);
			sigswitchX(i2);
			return i2;
		break;
		case NOT:
			i2 = pushCells(cellsForType(e->type));
			codeZero(i2);
			incrementXbyY2(i2,i1);
			logicalnot(i2);
			return i2;
		break; 
	}
}
int codeExpCast(Exp* e) {
	int i1 = codeExp(e->cast.e);
	currentFunctionTIndex++;
	//printf("%di1\n",i1);
	codeDebugMessage("Cast");

		if(e->type->b == WByte) {
			// fprintf(output, "%%t%d = trunc i32 %%t%d to i8\n",
			// currentFunctionTIndex,
			// i1 );
		}
		else if(e->cast.e->type->b == WByte) {
			// fprintf(output, "%%t%d = sext i8 %%t%d to i32\n",
			// currentFunctionTIndex,
			// i1 );
		}
		else if(e->cast.e->type->b == e->type->b) {
			fprintf(output, ";cast useless\n");
			return i1;
		}
		else {
			fprintf(output, ";cast not implemented\n");
			return -1;
		}
	return i1;
}
char* adressOfParameter(const char* id) {
	Parameter* p = currentParameters;
		int t=0;
		while(p) {
			if(strcmp(id,p->id)==0)
				break;
			p=p->next;
			t++;
		}
		char * str = (char*)malloc(t/10+3);
		sprintf(str,"%%t%d",t);
		return str;
		
}




int sizeOfType(Type* t) {
	if(t->tag == base) {
		switch(t->b) {
			case WInt:
				return sizeof(int);
			break;
			case WShort:
				return sizeof(short);
			break;
			case WFloat:
				return sizeof(float);
			break;
			case WByte:
				return sizeof(char);
			break;
			case WBit:
				return sizeof(char);
			case WChar:
				return sizeof(char);
			break;
		}
	}
	else {
		return sizeof(int*); //pointer size
	}
}

void codeLabel(int label) {
	fprintf(output, "b%d:\n",label);
}
void codeBranches(int cond, int lt,int lf) {
	fprintf(output, "br i1 %%t%d, label %%b%d, label %%b%d\n", 
			cond,
			lt,
			lf);
}

int codeCondToValue(int b1,int b2,int b3) {
	// fprintf(output, "b%d:\n", b1);
	// fprintf(output, "br label %%b%d\n",
	// 	b3);
	// fprintf(output, "b%d:\n", b2);
	// fprintf(output, "br label %%b%d\n",
	// 	b3);
	// fprintf(output, "b%d:\n", b3);
	// currentFunctionTIndex++;
	// fprintf(output, "%%t%d = phi i32 [ 1, %%b%d ], [0, %%b%d]\n",
	// currentFunctionTIndex,
	// b1,
	// b2 );
	return currentFunctionTIndex;
}
int codeSimpleCompare(Exp* e) {
	int i1,i2;
	int t1,t2,t3;
	i1 = codeExp(e->cmp.e1);
	i2 = codeExp(e->cmp.e2);
	switch(e->cmp.op){
		case GT:
			t1 = pushCells(cellsForType(e->type));
			t2 = pushCells(cellsForType(e->type));
			t3 = pushCells(cellsForType(e->type));
			//preserve x and y
			codeZero(t2);
			incrementXbyY2(t2,i1);
			codeZero(t3);
			incrementXbyY2(t3,i2);
			//greater algorithm
			codeZero(t1);
			//incrementXbyY2(t1,i1); this shouldn't be necessary
			greater(i1,i2,t1); //z = x > y => greater(x,y,z);
			//revert x and y
			incrementXbyY2(i1,t2); //x is left to zero
			codeZero(i2);
			incrementXbyY2(i2,t3);
			//popCells(cellsForType(e->type)*2);
			return t1;
		break;
		case LS: //a <= x <=> x-a > 0
			t1 = pushCells(cellsForType(e->cmp.e2->type));
			codeZero(t1);
			incrementXbyY2(t1,i1);
			less(t1,i2);
			return t1;
		break;
		case LSE: //a <= x <=> x-a > 0
			t1 = pushCells(cellsForType(e->cmp.e2->type));
			codeZero(t1);
			incrementXbyY2(t1,i1);
			lessequal(t1,i2);
			return t1;
		break;
		case EqEq: //a == x <=> x-a == 0
			t1 = pushCells(cellsForType(e->cmp.e2->type));
			codeZero(t1);
			incrementXbyY2(t1,i1);
			equals(t1,i2);
			printf("i1:%d i2:%d t1:%d\n",i1,i2,t1);
			codeDebugMessage("equal than");
			return t1;
		break;
		default:
			codeStr("Not implemented");
			return -1;
		break;
	}

}
int codeExpCompare(Exp* e) {
	int i1,i2;
	int t1,t2,t3;
	//yep, no short circuit now
	i1 = codeExp(e->cmp.e1);
	i2 = codeExp(e->cmp.e2);
	// there are ways to implement short circuit by sharding the algorithms
	// and evaluating exps only when is absolutely necessary, but that's for later.
	switch(e->cmp.op) {

		case OR: 
			t1 = pushCells(cellsForType(e->type));
			t2 = pushCells(cellsForType(e->type));
			t3 = pushCells(cellsForType(e->type));
			//preserve x and y
			codeZero(t2);
			incrementXbyY2(t2,i1);
			codeZero(t3);
			incrementXbyY2(t3,i2);
			//or algorithm
			codeZero(t1);
			or2(i1,i2,t1); //z = x > y => or(x,y,z);
			//revert x and y
			incrementXbyY2(i1,t2); //x is left to zero
			codeZero(i2);
			incrementXbyY2(i2,t3);
			return t1;
		break;
		case AND:
			t1 = pushCells(cellsForType(e->cmp.e2->type));
			codeZero(t1);
			incrementXbyY2(t1,i1);
			and(t1,i2);
			return t1;
		break;
		default:
			return codeSimpleCompare(e);
		break;
	}
}

int codeExp(Exp* e) {
	int result =-1;
	if(!e)
		return -1;
	//e = optimizeExp(e);
	//printExp(e,10);
	switch(e->tag) {
		int te1,te2;
		case ExpAdd:
				//result = codeBinExp(e,"add nsw");
				te1 = codeExp(e->bin.e1 );
				te2 = codeExp(e->bin.e2 );
				currentAllocationIndex = pushCells(cellsForType(e->type));
				codeZero(currentAllocationIndex);
				incrementXbyY2(currentAllocationIndex,te1);
				incrementXbyY2(currentAllocationIndex,te2);
				codeDebugMessage("Add");
				result = currentAllocationIndex;
		break;
		case ExpSub:
				te1 = codeExp(e->bin.e1 );
				te2 = codeExp(e->bin.e2 );
				currentAllocationIndex = pushCells(cellsForType(e->type));
				codeZero(currentAllocationIndex);
				incrementXbyY(currentAllocationIndex,te1);
				decrementXbyY(currentAllocationIndex,te2);
				codeDebugMessage("Sub");
				result = currentAllocationIndex;
		break;
		case ExpMul:
				te1 = codeExp(e->bin.e1 );
				te2 = codeExp(e->bin.e2 );
				currentAllocationIndex += cellsForType(e->type);
				codeZero(currentAllocationIndex);
				incrementXbyY(currentAllocationIndex,te1);
				multiplyXbyY(currentAllocationIndex,te2);
				codeDebugMessage("Mult");
				result = currentAllocationIndex;
		break;
		case ExpDiv:
				te1 = codeExp(e->bin.e1 );
				te2 = codeExp(e->bin.e2 );
				currentAllocationIndex += cellsForType(e->type);
				codeZero(currentAllocationIndex);
				incrementXbyY(currentAllocationIndex,te1);
				divideXbyY(currentAllocationIndex,te2);
				codeDebugMessage("Div");
				result = currentAllocationIndex;
		break;
		case ExpCall:
			result = codeCallExp(e);
		break;
		case ExpVar:
			result = codeExpVar(e);
		break;
		case ExpUnary:
			result = codeExpUnary(e);
		break;
		case ExpPrim:
			result = codeExpPrim(e);	
		break;
		case ExpNew:
			result = -1; //codeExpNew(e);
		break;
		case ExpCmp:
			result = codeExpCompare(e);
		break;
		case ExpAccess:
			result = -1; //codeExpAccess(e);
			
		break;
		case ExpCast:
			result = codeExpCast(e);
			
		break;
		case ExpOperator:
			result = codeExpOperator(e);
			
		break;
	}

	return result;
}
int codeExpOperator(Exp* e) {
	int result = codeExp(e->opr.e);
	printf("amount %d\n",e->opr.amount);
	switch(e->opr.op) {
		case INC:
			codeConstantIncrement(result,e->opr.amount);
		break;
		case DEC:
			codeConstantDecrement(result,e->opr.amount);
		break; 
	}
	return currentFunctionTIndex;
}
