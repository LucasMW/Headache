#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#if !defined(symbolTable_h)
	#include "symbolTable.h"
	#define symbolTable_h
#endif
#include "optimizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int optimizationLevel;


void setOptimizationLevel(int level){
	//printf("optimizationLevel =  %d\n",level );
	optimizationLevel = level;
}

void optimizeDefList(Def* d);
void optimizeNameList(NameL* nl);
void optimizeDefVar(DefVar* dv);
void optimizeDefFunc(DefFunc* df);
void optimizeType(Type* t);
void optimizeParams(Parameter* params);
void optimizeBlock(Block* b);
void optimizeDefVarList(DefVarL* dvl);
void optimizeCommandList(CommandL* cl); 
void optimizeCommandPrint(CommandL* cl);
void optimizeExpPrint(Exp* e);
void optimizeExpList(ExpList* el);
Exp* optimizeExp(Exp* e);

void optimizeTree() {
	if(globalTree->next == NULL) {
	}
	else {
		optimizeDefList(globalTree->next);
	}
}
void optimizeDefList(Def* d)
{
	Def* df = d;
	while(df!=NULL) {
		switch(df->tag) {
			case DVar:
				optimizeDefVarList(df->u.v);
			break;
			case DFunc:
				optimizeDefFunc(df->u.f);
			break;
		}
		df = df->next;
		
	}
}

void optimizeNameList(NameL* nl) {
	if(!nl)
		return;
	NameL* p = nl;
	do  {
		p = p->next;
	} while(p);
}
void optimizeDefVar(DefVar* dv){	
	if(!dv)
		return;
	optimizeType(dv->t);
}
void optimizeDefFunc(DefFunc* df)
{
	if(!df)
		return;

	optimizeType(df->retType);
	optimizeParams(df->params);
	optimizeBlock(df->b);
}
void optimizeType(Type* t) {
}

void optimizeParams(Parameter* params)
{
	if(!params) {
		return;
	}
	Parameter* p = params;
	while(p) {
		p = p->next; 
	}
}
void optimizeBlock(Block* b) {
	if(!b)
		return;
	optimizeDefVarList(b->dvl);
	optimizeCommandList(b->cl);
}
void optimizeDefVarList(DefVarL* dvl) {
	if(!dvl)
		return;
	DefVarL* d = dvl;
	while(d){
		optimizeDefVar(d->dv);
		d = d->next;
	}
}
void optimizeCommandList(CommandL* cl) {
	if(!cl)
		return;
	CommandL* c = cl;
	while(c) {
		switch(c->tag) {
			case CWhile:
				optimizeExp(c->condExp);
				optimizeCommandList(c->cmdIf);
			break;
			case CFor:
				optimizeExp(c->cFor.cmpExp);
				optimizeCommandList(c->cFor.begin);
				optimizeCommandList(c->cFor.end);
				optimizeCommandList(c->cFor.block);
			break;
			case CIf:
				optimizeExp(c->condExp);
				optimizeCommandList(c->cmdIf);
			break;
			case CIfElse:
				optimizeExp(c->condExp);
				optimizeCommandList(c->cmdIf);
				optimizeCommandList(c->cmdElse);
			break;
			case CReturn:
				c->retExp = optimizeExp(c->retExp);
			break;
			case CAssign:
				optimizeExp(c->expLeft);
				c->expRight = optimizeExp(c->expRight);
			break;
			case CBlock:
				optimizeBlock((Block*)c->block);
			break;
			case CCall:
				optimizeExp(c->expRight);
			break;
			case CPrint:
				c->printExp = optimizeExp(c->printExp);
			if(optimizationLevel >= 1) {
				optimizeExpPrint(c->printExp);
				optimizeCommandPrint(c);
			}

			break;
			case CRead:
				c->printExp = optimizeExp(c->printExp);
			break;
			case CDebug:
				optimizeExp(c->printExp);
			break;
			case COperator:
				optimizeExp(c->oprExp);
			break;


		}
		c = c->next;
	}
}

void optimizeCommandPrint(CommandL* cl){
	if(cl->tag == CPrint && cl->printExp->tag == ExpPrim && cl->printExp->c->tag == KStr){
		printf("will opt 1\n");
		if(cl->next != NULL && cl->next->tag == CPrint && cl->next->printExp->tag == ExpPrim && cl->next->printExp->c->tag == KStr){
			printf("will opt 2\n");
			// if((cl->printExp->c == NULL) || (cl->next->printExp->c == NULL)){
			// 	printf("Fuck off 1\n");
			// 	return;
			// }
			// if((cl->printExp->c->u.str == NULL) || (cl->next->printExp->c->u.str == NULL)){
			// 	printExp(cl->next->printExp,3);
			// 	printf("Fuck off 2\n");
			// 	return;
			// }
			// if(cl->printExp->tag == ExpVar ||  cl->next->printExp->tag == ExpVar){
			// 	printf("Fuck off 3\n");
			// 	return;
			// }
			//printCommandList(cl,1);
			cl->next->printExp = optimizeExp(cl->next->printExp);
			
			printf("will opt 3\n");
			optimizeExpPrint(cl->next->printExp);
			printf("will opt 4\n");
			optimizeCommandPrint(cl->next); //this should be possible, but it was crashing
			printf("will opt 5\n");

			int x = strlen(cl->printExp->c->u.str);
			printf("will opt 5.1 %d\n",x);

			printExp(cl->next->printExp,20);
			int y = strlen(cl->next->printExp->c->u.str);
			printf("will opt 5.2 (%d)\n",y);

			char* newStr = malloc(strlen(cl->printExp->c->u.str) + strlen(cl->next->printExp->c->u.str)+1);
			printf("will opt 6 b\n");
			sprintf(newStr,"%s%s",cl->printExp->c->u.str,cl->next->printExp->c->u.str);
			//free(cl->printExp->c->u.str);
			//free(cl->next->printExp->c->u.str);
			printf("will opt 7\n");
			cl->printExp->c->u.str = newStr;
			printf("will opt 8\n");
			cl->next = cl->next->next;
			printf("will opt 9\n");
			// free(cl->next->printExp->c);
			// free(cl->next->printExp);
			// free(cl->next);
		}
	}
}



void optimizeExpPrint(Exp* e){
	if(!e)
		return;
	if(e->tag == ExpPrim) {
		if(e->c->tag == KInt){
			printf("xxxx\n");
			printExp(e,10);
			printf("xxxx\n");
			int x = e->c->u.i;
			char* nStr = malloc(x%10+2);
			sprintf(nStr,"%d",x);
			free(e->c);
			free(e->type);
			Constant* c = (Constant*)malloc(sizeof(Constant));
			c->tag = KStr;
			c->u.str = nStr;
			e->c = c;
			e->type = (Type*)malloc(sizeof(Type));
	    	e->type->tag = array; 
	    	e->type->of = (Type*)malloc(sizeof(Type));
	    	e->type->of->tag = base;
	    	e->type->of->b = WByte;
		}
	}
}

void optimizeExpList(ExpList* el) {
	if(!el)
		return;
	ExpList *p = el;
	while(p) {
		optimizeExp(p->e);
		p = p->next;
	}
}
void optimizeVar(Var* v) {
	if(!v)
		return;
}
void optimizeConstant(Constant* c) {
	if(!c)
		return;
	switch(c->tag) {
		case KInt:
		break;
		case KFloat:
		break;
		case KStr:
		break;
		case KBit:
		break;

	}
}

Exp* optimizeExpLogic(Exp* e){
	Exp* newExp = e;
	//Exp* e1 = optimizeExp(e->bin.e1);
	//Exp* e2 = optimizeExp(e->bin.e2);
	//printf("Optimized exp\n");
	//printExp(newExp,2);
	return newExp;	
}

Exp* optimizeExpBin(Exp* e){
	//printf("binexp\n");
	Exp* newExp = e;
	Exp* e1 = optimizeExp(e->bin.e1);
	Exp* e2 = optimizeExp(e->bin.e2);
	if(e1->tag == ExpPrim && e2->tag == ExpPrim) {
		//printf("Both prim\n");
		char r; //same semantics as brainfuck cell
		short r2;
		int r4;
		if(e->tag == ExpAdd){
			r = (char)e1->c->u.i + (char)e2->c->u.i;
			r2 = (short)e1->c->u.i + (short)e2->c->u.i;
			r4 = e1->c->u.i + e2->c->u.i;
		}
		else if(e->tag == ExpSub){
			r = (char)e1->c->u.i - (char)e2->c->u.i;
			r2 = (short)e1->c->u.i - (short)e2->c->u.i;
			r4 = e1->c->u.i - e2->c->u.i;
		}
		else if(e->tag == ExpMul){
			r = (char)e1->c->u.i * (char)e2->c->u.i;
			r2 = (short)e1->c->u.i * (short)e2->c->u.i;
			r4 = e1->c->u.i * e2->c->u.i;
		}
		else if(e->tag == ExpDiv) {
			//Dealing with 0
			if(e2->c->u.i == 0 && e1->c->u.i == 0){
				r = 0;
				r2 = 0;
				r4 = 0;
			}
			else if(e2->c->u.i == 0){
				raiseWarning("cannot optimize",e->dbg_line);
				return e; //abort optimization. Cannot optimize this
			} 
			else {
				r = (char)e1->c->u.i / (char)e2->c->u.i;
				r2 = (short)e1->c->u.i / (short)e2->c->u.i;
				r4 = e1->c->u.i / e2->c->u.i;  
			}
			
		}
		else {
			printf("SEVERE ERROR optimizer\n");
			printf("aborting optimization\n");
			return e;
		}
		//printf("\nr is %d\n", r);
		newExp=(Exp*)malloc(sizeof(Exp));
		newExp->tag = ExpPrim;
		newExp->c = (Constant*)malloc(sizeof(Constant));
		newExp->c->tag = KInt;
		switch(e->type->b){
			case WByte:
				newExp->c->u.i = r;
			break;
			case WShort: 
				newExp->c->u.i = r2;
			break;
			case WInt:
				newExp->c->u.i = r4;
			break;
			case WFloat:
				raiseWarning("cannot optimize",e->dbg_line);
			break;
			case WBit:
				raiseWarning("cannot optimize",e->dbg_line);
			break;
			case WChar:
				raiseWarning("cannot optimize",e->dbg_line);
			break;
		}
		newExp->c->start_cell = 0;
		printf("registers (%d,%d,%d); \nchosen value %d \n", newExp->c->u.i ,r,r2,r4);
		
		printf("types involved (%d,%d) => %d\n",e->type->b,e1->type->b,e2->type->b);		
	} else if(optimizationLevel >= 2) {
		//printf("optimizer inc dec\n");
		if(e1->tag == ExpPrim) { // translate to simple increments or decrements
			newExp = (Exp*)malloc(sizeof(Exp));
			newExp->tag = ExpOperator;
			newExp->opr.amount = e1->c->u.i;
			if(e->tag == ExpAdd){
				newExp->opr.op = INC;
			} else if(e->tag == ExpSub){
				newExp->opr.op = DEC;
			} else{
				return e;
			}
			newExp->opr.e = e2;
			newExp->start_cell = 0;


		} else if(e2->tag == ExpPrim) {
			newExp = (Exp*)malloc(sizeof(Exp));
			newExp->tag = ExpOperator;
			newExp->opr.amount = e2->c->u.i;
			if(e->tag == ExpAdd){
				newExp->opr.op = INC;
			} else if(e->tag == ExpSub){
				newExp->opr.op = DEC;
			} else {
				return e;
			}
			newExp->opr.e = e1;
			newExp->start_cell = 0;
			
		}

	}
	newExp->dbg_line = e->dbg_line;
	newExp->type = e->type; //self consistency 
	//printf("Optimized exp\n");
	//printExp(newExp,2);
	return newExp;
}
Exp* optimizeExp(Exp* e) {
	
	if(!e)
		return e;
	//printf("e->dbg_line %d\n",e->dbg_line);
	//printExp(e,10);
	Exp* newExp = e; //case nothing happens, should return the same
	// Exp* e1;
	// Exp* e2;

	switch(e->tag) {
		case ExpAdd:
		case ExpSub:
		case ExpMul:
		case ExpDiv:
			if(optimizationLevel >= 1) 
			{
				newExp = optimizeExpBin(e);
			}
		break;
		case ExpCall:
			//printf("call\n");
		break;
		case ExpVar:
			//printf("var\n");
		break;
		case ExpUnary:
			//printf("unary\n");
		break;
		case ExpPrim:
		break;
		case ExpNew:
			//printf("new\n");
		break;
		case ExpCmp:
			if(optimizationLevel >= 1) 
			{
				newExp = optimizeExpLogic(e); 
			}
			//printf("cmp\n");
		break;
		case ExpAccess:
			//printf("access\n");
		break;
		case ExpCast:
			if(optimizationLevel >= 1)
			{
				newExp->cast.e = optimizeExp(e->cast.e);
			}
		break;
		case ExpOperator:
			if(optimizationLevel >= 1)
			{
				newExp->opr.e = optimizeExp(e->opr.e);
			}
		break;
	}
	//printExp(newExp,0);
	return newExp;
}