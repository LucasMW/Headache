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


static const int registerCount = 4;
int currentTempRegs[registerCount] = {0,1,2,3};
static int currentAllocationIndex = registerCount;
static int currentCell = 0;

static char* memory[30000]; //debugs and controls free memory

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


static void bfalgo(char* str, ...){

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
	for(int i=0;i<x;i++){
		memory[currentAllocationIndex+i] = 1;
	}
	currentAllocationIndex+=x;
	return currentAllocationIndex;
}
static int popCells(int x){
	for(int i=0;i<x;i++){
		memory[currentAllocationIndex-i] = 0;
	}
	currentAllocationIndex-=x;
	return currentAllocationIndex;
}

/*looks for not used temporary
chunks is needed memory size

*/

int findTempMemory(int chunks){
	return 0;
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
	int temp0 = currentTempRegs[2];
	bfalgo("$[-] $[$+$+$-] $[$+$-]",temp0,y,x,temp0,y,
		temp0,y,temp0);
}
static void incrementXbyY(int x,int y) {
	//printf("add %d to %d\n", x,y);
	int temp0 = currentTempRegs[2];
	codeGoTo(temp0); codeStr("[-]");
	codeGoTo(y); codeStr("["); codeGoTo(x); codeStr("+"); 
	codeGoTo(temp0); codeStr("+"); codeGoTo(y); codeStr("-]");
	codeGoTo(temp0); codeStr("[");  codeGoTo(y); codeStr("+"); 
	codeGoTo(temp0); codeStr("-]");
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
static void setXtoY(int x, int y) {
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
/*temp0[-]
temp1[-]
x[temp1+x-]
y[temp1-temp0+y-]
temp0[y+temp0-]
temp1[x+temp1[-]] */
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

char* stringForType(Type* t) {
	char* tStr = NULL;
	if(t == NULL)
		return "void";
	switch(t->tag) {
		case base:
			switch(t->b) {
				case WInt:
					return "i32";
				break;
				case WFloat:
					return "float";
				break;
				case WByte:
					return "i8";
				break;
				case WBit:
					return "i1";
				break;
			}
		break;
		case array:
			
			tStr = stringForType(t->of);
			int tlen = strlen(tStr);
			char* new = malloc(tlen+2);
			strcpy(new,tStr);
			//free(tStr); // no need anymore
			new[tlen] = '*';
			new[tlen+1] = '\0';
			return new;

		break;
	}
	return tStr;
}

static char* stringForDefaultValue(Type* t) {
	if(!t) {
		printf(";That's probably an error\n");
		return "void";
	}
	if(t->tag == base) {
		if(t->b == WInt || t->b == WByte || t->b == WShort) {
			return "0";
		}
		else {
			return "0.0";
		}
	} else {
		return "null";
	}

}


static void pushStringToDeclare(char* str) {
	//printf("%s\n",str );
	char* nstr = malloc(strlen(str)+1);
	strcpy(nstr,str);
	nstr[strlen(str)] = '\0';
	stringsToDeclare[declareTop] = nstr ;
	declareTop++;
}
// static void declateStringsToDeclare() {
// 	int x = currentStringConstant - declareTop;
// 	for(int i = 0;i<declareTop;i++,x++) {
// 		//printf("ihihiz\n");
// 		int len = strlen(stringsToDeclare[i])+1;
// 		fprintf(output, "@.cstr.%d = private unnamed_addr constant [%d x i8] c\"%s\\00\"\n",
// 		x+1,
// 		len,
// 		 stringsToDeclare[i]);
// 		free(stringsToDeclare[i]);
// 		stringsToDeclare[i] = NULL;
// 	}
// 	declareTop = 0;
// }

char* stringForVarAddress(const char* name,int scope) {
	char string[50] = "no string yet";
	if(strlen(name) >= 50) {
		printf("SevereError. var name is to big\n");
	}
	if(scope == 0) {
		sprintf(string,"@g%s",name);
	}
	else {
		sprintf(string,"%%l%d%s",scope,name);
	}
	
	char* str = (char*)malloc(strlen(string)+1);
	strcpy(str,string);
	str[strlen(string)] = '\0';
	return str;
}
static void codeSignature() {
	//char version[] = "v_alpha_0_1";
	fprintf(output, "Generated by HAC (HeadAche Comp) \n" );
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
	char* typeStr = stringForType(df->retType);
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
/* 
 %t315 = getelementptr i32* @g25, i32 0
 ~= i315 = &g25[0];
*/
// void getelementptr(char* str) {
// 	fprintf(output, "%s\n", );
// }
char* adressOfLeftAssign(Exp* e) {
	//printf("addr left assig\n");
	if(e->tag == ExpVar) {
		//printf("e->var.id %s\n",e->var->id);
		
		if (e->var->declaration == NULL){
			Parameter* p = currentParameters;
			int t=0;
			while(p) {
				if(strcmp(e->var->id,p->id)==0)
					break;
				p=p->next;
				t++;
			}
			char * str = (char*)malloc(t/10+3);
			sprintf(str,"%%t%d",t);
			return str;
		}
		else {
			int scope = e->var->declaration->scope;
			char* varAddr = stringForVarAddress(e->var->id,scope);
			return varAddr;
		}
	}
	else if(e->tag == ExpAccess) {
		int i1 = codeExpAccess(e)-1;  //received getElemPtr
		char * str = (char*)malloc(i1/10+3);
		sprintf(str,"%%t%d",i1);
		return str;
	}
	else {
		fprintf(output, ";SevereError\n");
	}
	return NULL;
}
int codeCond(Exp* e) {
	int i1;
	int temp = currentTempRegs[1];
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
				 /* gambiarra para short e int sem esforço*/
				 printType(c->expLeft->type,0);
				 if(c->expLeft->type->b == WShort){
				 	printf("forceExpand %d\n",forceExpand);
				 	forceExpand = forceExpand > 1 ? forceExpand : 1;
				 }
				 else if(c->expLeft->type->b == WInt){
				 	printf("forceExpand %d\n",forceExpand);
				 	forceExpand = forceExpand > 2 ? forceExpand : 2;
				 }
				 else {
				 	printf("don't forceExpand %d\n",forceExpand);
				 }
				 //printExp(c->expRight,0);
				 i2 = codeExp(c->expLeft);
				 //printExp(c->expRight,0);
				 moveXToY(i1,i2);
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

						codeCellValuePrintAnySize (i1,i1);
						codeDebugMessage("print byte");

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
	
	int toCall = -1;
	int size=0;
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

char* stringForConstant(Constant* c) {
	//char str[40] = "no string given";
	char* str;
	double nd;
	int exponent = 0;
	if(!c)
		return NULL;
	switch(c->tag) {
		case KInt:
			str = (char*)malloc(40);
			sprintf(str, "%d", c->u.i);
		break;
		case KFloat:
			nd = frexp(c->u.d, &exponent);
			str = "No floats";//hexaStringForFloat(c->u.d);
		break;
		case KStr:
			str = (char*)c->u.str;
		break;
	}
	return &str[0];
}
int codeExpPrim(Exp* e) {
	currentFunctionTIndex++;
	//char* tStr = stringForType(e->type);
	if(e->c->tag == KStr) {
		currentStringConstant++;
		char* cStr = stringForConstant(e->c);
		
		pushStringToDeclare(cStr);

		return currentFunctionTIndex;
	}
	
	//char* cStr = stringForConstant(e->c);
	
	if(e->c->tag == KFloat) {
		
	} else {
		if(e->c->start_cell == 0) {
			e->c->start_cell = currentAllocationIndex += 2;
			//printf("kint at %d\n",currentAllocationIndex);
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
	char* tStr = stringForType(e->type);
	int i1,i2;
	i1 = codeExp(e->unary.e);
	currentFunctionTIndex++;
	switch(e->unary.op) {
		case MINUS:
			if(e->type->b == WFloat) {
				fprintf(output, "%%t%d = fsub %s 0.0, %%t%d\n",
				 currentFunctionTIndex,
				 tStr,
				 i1);
				
			}
			else {
				fprintf(output, "%%t%d = sub nsw %s 0, %%t%d\n",
				 currentFunctionTIndex,
				 tStr,
				 i1);
			}
		break;
		case NOT:
			if(e->type->b == WFloat) {
				fprintf(output, "%%t%d = fcmp oeq float %%t%d, 0.0\n",
				 currentFunctionTIndex,
				 i1);
			i2 = currentFunctionTIndex++;
  			fprintf(output, "%%t%d = uitofp i1 %%t%d to float\n",
				currentFunctionTIndex,
				i2);
			}
			else {
				fprintf(output, "%%t%d = icmp eq %s %%t%d, 0\n",
				 currentFunctionTIndex,
				 tStr,
				 i1);
				i2 = currentFunctionTIndex++;
				fprintf(output, "%%t%d = zext i1 %%t%d to %s\n",
				currentFunctionTIndex,
				i2,
				tStr );
			}
		break; 
	}
	return currentFunctionTIndex;
}
int codeExpCast(Exp* e) {
	int i1 = codeExp(e->cast.e);
	char* orTStr = stringForType(e->cast.e->type);
	char* toTStr = stringForType(e->type);
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
char* addressOfVector(Exp* e) {
	//printf("addressOfVector\n");
	if(e->tag == ExpAccess) {
		//printf("gacr\n");
		return addressOfVector(e->access.varExp);
	}
	else if(e->tag == ExpVar) {
		//printf("gVar\n");

		if(e->var->declaration == NULL)
		{
			return adressOfParameter(e->var->id);
		}
		return adressOfLeftAssign(e);
	}
	else {
		return "%%SevereError";
	}
}
int codeGetElemPtr(Type* type,int arrayTemp,int indexTemp) {
	currentFunctionTIndex++;
	char* tStr = stringForType(type);
	fprintf(output, "%%t%d = getelementptr %s, %s* %%t%d, i64 %%t%d\n",
	currentFunctionTIndex,
	tStr,
	tStr,
	arrayTemp,
	indexTemp);
	return currentFunctionTIndex;
}
int codeAccessElemPtr(Exp* e) {
	//fprintf(output,";getelementptr\n");
	int i1 = codeExp(e->access.indExp);
	currentFunctionTIndex++;
	fprintf(output, "%%t%d = sext i32 %%t%d to i64\n",
			currentFunctionTIndex,
			i1 );
	
	char* tStr = stringForType(e->type);
	int index = currentFunctionTIndex++;
	char* str = addressOfVector(e->access.varExp);
	
	fprintf(output, "%%t%d = load %s*, %s** %s\n",
	currentFunctionTIndex,
	tStr,
	tStr,
	str );
	//fprintf(output,"; %s mark1\n",stringForType(e->type));

	int startArrayAddress = currentFunctionTIndex++;
	fprintf(output, "%%t%d = getelementptr %s, %s* %%t%d, i64 %%t%d\n",
	currentFunctionTIndex,
	tStr,
	tStr,
	startArrayAddress,
	index);
	return currentFunctionTIndex;
}
int codeExpAccess(Exp* e) {
	//fprintf(output,";Exp Access\n");

	int i1,i2;
	char* tStr;
	int access;
	switch (e->tag) {
		case ExpAccess :
		i1 = codeExp(e->access.indExp);
		currentFunctionTIndex++;
		fprintf(output, "%%t%d = sext i32 %%t%d to i64\n",
			currentFunctionTIndex,
			i1 );
		tStr = stringForType(e->type);
		int index = currentFunctionTIndex++;
		access = codeExp(e->access.varExp);
		currentFunctionTIndex++;

		i2 =  codeGetElemPtr(e->type,
			access,
			index);
		// char* str = stringForVarAddress(e->access.varExp);
		currentFunctionTIndex++;
		fprintf(output, "%%t%d = load %s, %s* %%t%d\n",
		currentFunctionTIndex,
		tStr,
		tStr,
		i2 );
		
		return currentFunctionTIndex;
		break;
		default:
		//fprintf(output, ";default reached\n" );
		tStr = stringForType(e->type);
		char* str = addressOfVector(e);
		currentFunctionTIndex++;
		fprintf(output, "%%t%d = load %s, %s* %s\n",
		currentFunctionTIndex,
		tStr,
		tStr,
		str );
		return currentFunctionTIndex;
		break;
	}
	return currentFunctionTIndex;	
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
			break;
		}
	}
	else {
		return sizeof(int*); //pointer size
	}
}
int codeExpNew(Exp* e) {
	int i1 = codeExp(e->eNew.e);
	char * tStr = stringForType(e->type);
	currentFunctionTIndex++;
	fprintf(output, "%%t%d = mul i32 %%t%d, %d\n",
		currentFunctionTIndex, 
		i1,
		sizeOfType(e->type));
	i1 = currentFunctionTIndex++;
	fprintf(output, " %%t%d = tail call i8* @malloc(i32 %%t%d)\n",
	currentFunctionTIndex,
	i1);
	int i2 = currentFunctionTIndex++;
	fprintf(output, "%%t%d = bitcast i8* %%t%d to %s\n",
	currentFunctionTIndex,
	i2,
	tStr );
	return currentFunctionTIndex;
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
int codeSimpleCompare(Exp* e,const char* oprStr ) {
	int i1,i2;
	i1 = codeExp(e->cmp.e1);
	i2 = codeExp(e->cmp.e2);
	currentFunctionTIndex++;
	// fprintf(output, "%%t%d = icmp %s i32 %%t%d, %%t%d\n",
	// 	currentFunctionTIndex,
	// 	oprStr,
	// 	i1,
	// 	i2 );
	return currentFunctionTIndex;

}
int codeExpCompare(Exp* e) {
	int i1,i2;
	int b1 = currentBrIndex++;
	int b2 = currentBrIndex++;
	int b3 = currentBrIndex++;
	int ln;
	switch(e->cmp.op) {
		case GT:
			codeSimpleCompare(e,"sgt");
		break;
		case GTE:
			codeSimpleCompare(e,"sge");
		break;
		case LS:
			codeSimpleCompare(e,"slt");
		break;
		case LSE:
			codeSimpleCompare(e,"sle");
		break;
		case EqEq:
			codeSimpleCompare(e,"eq");
		break;
		case OR:
			// i1 = codeExp(e->cmp.e1);
			// currentFunctionTIndex++;
			// fprintf(output, "%%t%d = icmp ne i32 %%t%d, 0\n",
			// currentFunctionTIndex,
			// i1);
			// ln = currentBrIndex++;
			// codeBranches(currentFunctionTIndex,b1,ln); 
			// codeLabel(ln);
			// i2 = codeExp(e->cmp.e2);
			// currentFunctionTIndex++;
			// fprintf(output, "%%t%d = icmp ne i32 %%t%d, 0\n",
			// currentFunctionTIndex,
			// i2);
			// codeBranches(currentFunctionTIndex,b1,b2);
			return codeCondToValue(b1,b2,b3);
		break;
		case AND:
			// i1 = codeExp(e->cmp.e1);
			// currentFunctionTIndex++;
			// fprintf(output, "%%t%d = icmp ne i32 %%t%d, 0\n",
			// currentFunctionTIndex,
			// i1);
			// ln = currentBrIndex++;
			// codeBranches(currentFunctionTIndex,ln,b2); 
			// codeLabel(ln);
			// i2 = codeExp(e->cmp.e2);
			// currentFunctionTIndex++;
			// fprintf(output, "%%t%d = icmp ne i32 %%t%d, 0\n",
			// currentFunctionTIndex,
			// i2);
			// codeBranches(currentFunctionTIndex,b1,b2);
			return codeCondToValue(b1,b2,b3);
		break;
	}
	//codeBranches(currentFunctionTIndex,b1,b2);
	return codeCondToValue(b1,b2,b3);
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
				currentAllocationIndex += cellsForType(e->type);
				incrementXbyY2(currentAllocationIndex,te1);
				incrementXbyY2(currentAllocationIndex,te2);
				codeDebugMessage("Add");
				result = currentAllocationIndex;
		break;
		case ExpSub:
				te1 = codeExp(e->bin.e1 );
				te2 = codeExp(e->bin.e2 );
				currentAllocationIndex += cellsForType(e->type);
				incrementXbyY(currentAllocationIndex,te1);
				decrementXbyY(currentAllocationIndex,te2);
				codeDebugMessage("Sub");
				result = currentAllocationIndex;
		break;
		case ExpMul:
				te1 = codeExp(e->bin.e1 );
				te2 = codeExp(e->bin.e2 );
				currentAllocationIndex += cellsForType(e->type);
				incrementXbyY(currentAllocationIndex,te1);
				multiplyXbyY(currentAllocationIndex,te2);
				codeDebugMessage("Mult");
				result = currentAllocationIndex;
		break;
		case ExpDiv:
				te1 = codeExp(e->bin.e1 );
				te2 = codeExp(e->bin.e2 );
				currentAllocationIndex += cellsForType(e->type);
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
			result = codeExpNew(e);
		break;
		case ExpCmp:
			result = codeExpCompare(e);
		break;
		case ExpAccess:
			result = codeExpAccess(e);
			
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
void codeExpList(ExpList* el) {
	char * tStr;
	if(!el)
		return;
	ExpList *p = el;
	while(p) {
		int index = codeExp(p->e);
		tStr = stringForType(p->e->type);
		fprintf(output, "%s %%t%d",tStr,index);
		if(p->next)
			fprintf(output, ", ");
		p = p->next;
	}
	return;

}