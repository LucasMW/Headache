// data Var = VarID ID
// 		| VarIndexed Exp Exp
// 		| ...
#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#if !defined(symbolTable_h)
	#include "symbolTable.h"
	#define symbolTable_h
#endif
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

progNode* globalTree;


DefVarL* DefVarToDevL(DefVar* dv);
DefVarL* NameLToDevL(NameL* nl, Type* t, int scope );
void DefVarLFix(DefVarL** dvlRef);

void DefVarLFix(DefVarL** dvlRef){
	(*dvlRef)->next = DefVarToDevL((*dvlRef)->dv);
}

DefVarL* DefVarToDevL(DefVar* dv){
	DefVarL* dvl =  (DefVarL*)malloc(sizeof(DefVarL));
	dvl->next = NameLToDevL(dv->nl, dv->t,dv->scope);
	return dvl;
}

DefVarL* NameLToDevL(NameL* nl, Type* t, int scope ){
	if(!nl)
		return NULL;
	DefVarL* dvl = (DefVarL*)malloc(sizeof(DefVarL));
	DefVarL* currentDvl = dvl;
	NameL* p = nl;
	do  {
		DefVarL* ndvl = (DefVarL*)malloc(sizeof(DefVarL));
		DefVar* dv = (DefVar*)malloc(sizeof(DefVar));
		dv->scope = scope;
		dv->t = t;
		dv->id = malloc(strlen(p->name)+1);
		strcpy((char*)dv->id,p->name);
		dv->start_cell = 0;
		dv->limits = NULL;
		ndvl->dv = dv;
		currentDvl->next = ndvl;
		currentDvl = ndvl;

		p = p->next;
	} while(p);
	currentDvl->next = NULL;

	return dvl;
}

static void printDepthLevel(const char* str,int x) {
	int i =0;
	printf("\n|");
	for(i=0;i<x;i++) {
		printf("  |");
	}
	printf("-%s",str);
	//printf("$%d$",depth_level);
}
void printTree() {
	printf("Tree");
	if(globalTree->next == NULL) {
		printf("is Null\n");
	}
	else {
		printf("\nroot\n|");
		printDefList(globalTree->next,0);

	}
	printf("\n");
}
void printDefList(Def* d,int x)
{
	Def* df = d;
	printDepthLevel("DEF LIST",x);
	while(df!=NULL) {
		switch(df->tag) {
			case DVar:
				//printf("defVar \t");
				printDefVar(df->u.v,x+1);
			break;
			case DFunc:
				//printf("defFunc \t");
				printDefFunc(df->u.f,x+1);
			break;
		}
		df = df->next;
		
	}
}

void printNameList(NameL* nl,int x) {
	printDepthLevel("nl",x);
	if(!nl)
		return;
	NameL* p = nl;
	do  {
		printDepthLevel(p->name,x+1);
		p = p->next;
	} while(p);
}
void printDefVar(DefVar* dv,int x){
	
	if(!dv)
		return;
	printDepthLevel("defVar",x);
	printType(dv->t,x+1);
	//printNameList(dv->nl,x+1);
	printDepthLevel(dv->id,x+1);
}
void printDefFunc(DefFunc* df,int x)
{
	if(!df)
		return;
	printDepthLevel("DefFunc",x);
	printDepthLevel(df->id,x+1);
	printType(df->retType,x+1);
	printParams(df->params,x+1);
	printBlock(df->b,x+1);
}
void printType(Type* t,int x) {
	
	if(!t) {
		printDepthLevel("void ",x);
		return;
	}
	switch(t->tag) {
		case base:
			//printf("b: %d ",t->b);
			switch(t->b) {
				case WInt:
				printDepthLevel("int ",x);
				break;
				case WFloat:
				printDepthLevel("float ",x);
				break;
				case WByte:
				printDepthLevel("byte ",x);
				break;
			}
			
		break;
		case array:
			printDepthLevel("array of ",x);
			printType(t->of,x+1);
		break;
	}
}

void printParams(Parameter* params,int x)
{
	//printf("params\n");
	if(!params) {
		printDepthLevel("None",x);
		return;
	}
	Parameter* p = params;
	while(p) {
		printType(p->t,x);
		if(p->id)
			printDepthLevel(p->id,x+1);
		p = p->next; 
	}
}
void printBlock(Block* b,int x) {
	if(!b)
		return;
	printDepthLevel("block{}",x);
	
	printDefVarList(b->dvl,x+1);
	printCommandList(b->cl,x+1);
}
void printDefVarList(DefVarL* dvl,int x) {
	printDepthLevel("DefVarL",x);
	if(!dvl)
		return;
	DefVarLFix(&dvl);
	DefVarL* d = dvl;
	while(d){
		printDefVar(d->dv,x+1);
		d = d->next;
	}
}
void printCommandList(CommandL* cl,int x) {
	if(!cl)
		return;
	CommandL* c = cl;
	while(c) {
		switch(c->tag) {
			case CWhile:
				printDepthLevel("While",x);
				printExp(c->condExp,x+1);
				printCommandList(c->cmdIf,x+1);
			break;
			case CIf:
				printDepthLevel("If",x);
				printExp(c->condExp,x+1);
				printCommandList(c->cmdIf,x+1);
			break;
			case CIfElse:
				printDepthLevel("if/else",x);
				printExp(c->condExp,x+1);
				printCommandList(c->cmdIf,x+1);
				printCommandList(c->cmdElse,x+1);
			break;
			case CReturn:
				printDepthLevel("return ",x);
				printExp(c->retExp,x+1);
			break;
			case CAssign:
				printDepthLevel("Assign",x);
				printExp(c->expLeft,x+1);
				printExp(c->expRight,x+1);
			break;
			case CBlock:
				printDepthLevel("block ",x);
				printBlock((Block*)c->block,x+1);
			break;
			case CCall:
				printDepthLevel("call",x);
				printExp(c->expRight,x+1);
			break;
			case CPrint:
				printDepthLevel("print",x);
				printExp(c->printExp,x+1);
			break;
			case CDebug:
				printDepthLevel("debug",x);
				printExp(c->printExp,x+1);
			break;

		}
		c = c->next;
	}
}
void printExp(Exp* e,int x) {
	if(!e)
		return;

	printType(e->type,x);
	switch(e->tag) {
		case ExpAdd: 
			//printf("expAdd ");
			printDepthLevel("+",x);
			printExp(e->bin.e1,x+1);
			printExp(e->bin.e2,x+1);
		break;
		case ExpSub:
			printDepthLevel("-",x);
			printExp(e->bin.e1,x+1);
			printExp(e->bin.e2,x+1);
		break;
		case ExpMul:
			printDepthLevel("*",x);
			printExp(e->bin.e1,x+1);
			printExp(e->bin.e2,x+1);
		break;
		case ExpDiv:
			printDepthLevel("/",x);
			printExp(e->bin.e1,x+1);
			printExp(e->bin.e2,x+1);
		break;
		case ExpCall:
			printDepthLevel("call()",x);
			printDepthLevel(e->call.id,x+1);
			printExpList(e->call.expList,x+1);
		break;
		case ExpVar:
			printVar(e->var,x);
		break;
		case ExpUnary:
			switch(e->unary.op) {
				case MINUS:
					printDepthLevel("-(unary)",x);
				break;
				case NOT:
					printDepthLevel("!",x);
				break; 
			}
			printExp(e->unary.e,x+1);
		break;
		case ExpPrim:
			printDepthLevel("Prim",x);
			printConstant(e->c,x+1);
		break;
		case ExpNew:
			printDepthLevel("New",x);
			printType(e->eNew.t,x+1);
			printExp(e->eNew.e,x+1);
		break;
		case ExpCmp:
			switch(e->cmp.op) {
				case GT:
					printDepthLevel(">",x);
				break;
				case GTE:
					printDepthLevel(">=",x);
				break;
				case LS:
					printDepthLevel("<",x);
				break;
				case LSE:
					printDepthLevel("<=",x);
				break;
				case AND:
					printDepthLevel("&&",x);
				break;
				case OR:
					printDepthLevel("||",x);
				break;
				case EqEq:
					printDepthLevel("==",x);
				break;
			}
			printExp(e->cmp.e1,x+1);
			
			printExp(e->cmp.e2,x+1);
		break;
		case ExpAccess:
			printDepthLevel("[]",x);
			printExp(e->access.varExp,x+1);
			printExp(e->access.indExp,x+1);
			printType(e->type,x+1);printf("<-");
		break;
		case ExpCast:
			printDepthLevel("cast",x);
			printExp(e->cast.e,x+1);
			printType(e->cast.type,x+1); 
		break;
	}
}
void printExpList(ExpList* el,int x) {
	if(!el)
		return;
	ExpList *p = el;
	printDepthLevel("expList",x);
	while(p) {
		printExp(p->e,x+1);
		p = p->next;
	}

}
void printVar(Var* v,int x) {
	if(!v)
		return;
	printDepthLevel("Var",x);
	printDepthLevel(v->id,x+1);
}
void printConstant(Constant* c,int x) {
	char str[40] = "no string given";
	if(!c)
		return;
	switch(c->tag) {
		case KInt:
			sprintf(str, "%d", c->u.i);
			printDepthLevel(str,x);
		break;
		case KFloat:
			sprintf(str, "%lf", c->u.d);
			printDepthLevel(str,x);
		break;
		case KStr:
			sprintf(str, "\"%.*s\"", 37,c->u.str); //first 37 characters of string
			printDepthLevel(str,x);
		break;
	}
}

int getNumberOfCells(CellUsage cu){
	return cu.end - cu.start+1;
}

int getCellSize(){
	return 1;
}

int getMemoryUsageInBytes(CellUsage cu){
	return getNumberOfCells(cu) * getCellSize();
}

CellUsage* makeCellUsage(int start, int end){
	CellUsage* cu = (CellUsage*)malloc(sizeof(CellUsage));
	cu->start = start;
	cu->end = end;
	return cu;
}


Constant* makeConstant(constantType t)
{
	Constant* k = (Constant*)malloc(sizeof(Constant));
	k->tag = t;
	return k;
}
