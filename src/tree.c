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



// dvl->e->null
void DefVarLinkEnd(DefVarL* dvl, DefVarL* e){
	DefVarL* p = dvl;
	while(p->next){
		p=p->next;
	}
	p->next = e;
}
//e->dvl->null
void DefVarLinkBegin(DefVarL* dvl, DefVarL* e){
	DefVarL* p = e;
	while(p->next){
		p=p->next;
	}
	p->next = dvl;
}

void DefVarLFix(DefVarL** dvlRef);

void DefVarLFix(DefVarL** dvlRef){
	//printf("Fixing\n");
	if(!(*dvlRef))
		return;


	// DefVarL* oldNext = (*dvlRef)->next; // nextLine definitions
	
	// DefVarL* internal = DefVarToDevL((*dvlRef)->dv); //result from expanded

	// DefVarLinkEnd(internal,oldNext);

	// (*dvlRef)->dv = internal->dv;
	// (*dvlRef)->next = internal->next;
}

DefVarL* DefVarToDevL(DefVar* dv){
	// if(!dv)
	// 	return NULL;
	 DefVarL* dvl =  (DefVarL*)malloc(sizeof(DefVarL));
	// dvl->next = NameLToDevL(dv->nl, dv->t,dv->scope);
	// //printf("\ninternal\n");
	// //printDefVarList(dvl->next,0);
	// //printf("\nevery\n");
	// //printDefVarList(dvl,0);
	// free(dv->nl);
	// dv->nl = NULL;
	return dvl;
}

DefVarL* NameLToDevL(NameL* nl, Type* t, int scope ){
	if(!nl)
		return NULL;
	DefVarL* dvl = (DefVarL*)malloc(sizeof(DefVarL));
	// DefVarL* currentDvl = dvl;
	// NameL* p = nl;
	// do  {
	// 	DefVarL* ndvl = (DefVarL*)malloc(sizeof(DefVarL));
	// 	DefVar* dv = (DefVar*)malloc(sizeof(DefVar));
	// 	dv->scope = scope;
	// 	dv->t = t;
	// 	dv->id = malloc(strlen(p->name)+1);
	// 	strcpy((char*)dv->id,p->name);
	// 	dv->nl = NULL;
	// 	dv->start_cell = 0;
	// 	dv->limits = NULL;
	// 	ndvl->dv = dv;
	// 	currentDvl->next = ndvl;
	// 	currentDvl = ndvl;

	// 	p = p->next;
	// } while(p);
	// currentDvl->next = NULL;
	//printf("first dvl:\n");
	//printDefVarList(dvl,0);
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
				printDefVarList(df->u.v,x+1);
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
				case WShort:
				printDepthLevel("short ",x);
				break;
				case WBit:
				printDepthLevel("bit ",x);
				break;
				case WChar:
				printDepthLevel("char ",x);
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
	//DefVarLFix(&(b->dvl));
	printDefVarList(b->dvl,x+1);
	printCommandList(b->cl,x+1);
}
void printDefVarList(DefVarL* dvl,int x) {
	printDepthLevel("DefVarL",x);
	if(!dvl)
		return;
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
			case CFor: 
				printDepthLevel("For",x);
				printCommandList(c->cFor.begin,x+1);
				printExp(c->cFor.cmpExp,x+1);
				printCommandList(c->cFor.block,x+1);
				printCommandList(c->cFor.end,x+1);
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
			case CRead:
				printDepthLevel("read",x);
				printExp(c->printExp,x+1);
			break;
			case CDebug:
				printDepthLevel("debug",x);
				printExp(c->printExp,x+1);
			break;
			case COperator:
				printDepthLevel("operator",x);
				printExp(c->oprExp,x+1);
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
		case ExpOperator:
			// Constant* c = (Constant*)malloc(sizeof(Constant));
			// c->tag = KInt;
			// c->u.i = e->opr.amount;
			switch(e->opr.op) {
				case INC:
					printDepthLevel("(++)",x);
					//printConstant(c,x+1);
				break;
				case DEC:
					printDepthLevel("(--)",x);
					//printConstant(c,x+1);
				break; 
			}
			//free(c); //created only to call printConstant
			printExp(e->opr.e,x+1);
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
		case KBit:
			if(c->u.i)
				sprintf(str, "true");
			else 
				sprintf(str, "false");
			printDepthLevel(str,x);
		break;
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

/* Free Section */

void freeDefFunc(DefFunc* df);
void freeDefVarList(DefVarL* dvl);
void freeDefList(Def* d);
void freeNameList(NameL* nl);
void freeType(Type* t);
void freeParams(Parameter* params);
void freeCommandList(CommandL* cl);
void freeBlock(Block* b);
void freeExp(Exp* e);
void freeVar(Var* v);
void freeConstant(Constant* c);
void freeExpList(ExpList* el);
void freeTree();

void freeTree() {
	if(globalTree->next == NULL) {
		return;
	}
	else {
		freeDefList(globalTree->next);
		free(globalTree->next);
	}
	
}
void freeDefList(Def* d)
{
	Def* df = d;
	while(df!=NULL) {
		switch(df->tag) {
			case DVar:
				//printf("defVar \t");
				freeDefVarList(df->u.v);
				free(df->u.v);
			break;
			case DFunc:
				//printf("defFunc \t");
				freeDefFunc(df->u.f);
				free(df->u.f);
			break;
		}
		df = df->next;
		
	}
}

void freeNameList(NameL* nl) {
	if(!nl)
		return;
	while(nl) {
		NameL* p = nl;
		while(p){
			p=p->next;
			free((void*)p->name);
		}
		free(p);
	}
	

}
void freeDefVar(DefVar* dv){
	
	if(!dv)
		return;
	freeType(dv->t);

}
void freeDefFunc(DefFunc* df)
{
	if(!df)
		return;
	freeType(df->retType);
	freeParams(df->params);
	freeBlock(df->b);
	free(df->retType);
	free(df->params);
	free(df->b);

}
void freeType(Type* t) {
	
	if(!t) {
		return;
	}
	switch(t->tag) {
		case base:
		break;
		case array:
			freeType(t->of);
		break;
	}
}

void freeParams(Parameter* params)
{
	if(!params) {
		return;
	}
	Parameter* p = params;
	while(p) {
		freeType(p->t);
		if(p->id)
			free((void*)p->id);
		p = p->next; 
	}
}
void freeBlock(Block* b) {
	if(!b)
		return;
	freeDefVarList(b->dvl);
	freeCommandList(b->cl);
}
void freeDefVarList(DefVarL* dvl) {
	if(!dvl)
		return;
	DefVarL* d = dvl;
	while(d){
		freeDefVar(d->dv);
		d = d->next;
	}
}
void freeCommandList(CommandL* cl) {
	if(!cl)
		return;
	CommandL* c = cl;
	while(c) {
		switch(c->tag) {
			case CWhile:
				freeExp(c->condExp);
				freeCommandList(c->cmdIf);
			break;
			case CFor:
				freeCommandList(c->cFor.begin);
				freeExp(c->cFor.cmpExp);
				freeCommandList(c->cFor.end);
				freeCommandList(c->cFor.block);
			break;
			case CIf:
				freeExp(c->condExp);
				freeCommandList(c->cmdIf);
			break;
			case CIfElse:
				freeExp(c->condExp);
				freeCommandList(c->cmdIf);
				freeCommandList(c->cmdElse);
			break;
			case CReturn:
				freeExp(c->retExp);
			break;
			case CAssign:
				freeExp(c->expLeft);
				freeExp(c->expRight);
			break;
			case CBlock:
				freeBlock((Block*)c->block);
			break;
			case CCall:
				freeExp(c->expRight);
			break;
			case CPrint:
				freeExp(c->printExp);
			break;
			case CRead:
				freeExp(c->printExp);
			break;
			case CDebug:
				freeExp(c->printExp);
			break;
			case COperator:
				freeExp(c->oprExp);
			break;

		}
		c = c->next;
	}
}
void freeExp(Exp* e) {
	if(!e)
		return;

	freeType(e->type);
	switch(e->tag) {
		case ExpAdd: 
			freeExp(e->bin.e1);
			freeExp(e->bin.e2);
		break;
		case ExpSub:
			freeExp(e->bin.e1);
			freeExp(e->bin.e2);
		break;
		case ExpMul:
			freeExp(e->bin.e1);
			freeExp(e->bin.e2);
		break;
		case ExpDiv:
			freeExp(e->bin.e1);
			freeExp(e->bin.e2);
		break;
		case ExpCall:
			freeExpList(e->call.expList);
		break;
		case ExpVar:
			freeVar(e->var);
		break;
		case ExpUnary:
			freeExp(e->unary.e);
		break;
		case ExpPrim:
			freeConstant(e->c);
		break;
		case ExpNew:
			freeType(e->eNew.t);
			freeExp(e->eNew.e);
		break;
		case ExpCmp:
			freeExp(e->cmp.e1);
			freeExp(e->cmp.e2);
		break;
		case ExpAccess:
			freeExp(e->access.varExp);
			freeExp(e->access.indExp);
			freeType(e->type);
		break;
		case ExpCast:
			freeExp(e->cast.e);
			freeType(e->cast.type); 
		break;
		case ExpOperator:
			freeExp(e->opr.e);
		break;
	}
}
void freeExpList(ExpList* el) {
	if(!el)
		return;
	ExpList *p = el;
	while(p) {
		freeExp(p->e);
		p = p->next;
	}

}
void freeVar(Var* v) {
	if(!v)
		return;
	free((void*)v->id);
}
void freeConstant(Constant* c) {
	if(!c)
		return;
	switch(c->tag) {
		case KBit:
		break;
		case KInt:
		break;
		case KFloat:
		break;
		case KStr:
			free((void*)c->u.str);
		break;
	}
}

