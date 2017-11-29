#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#include "optimizer.h"
#include <stdlib.h>
#include <stdio.h>

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
	
	if(!t) {
		return;
	}
	switch(t->tag) {
		case base:
			switch(t->b) {
				case WInt:
				break;
				case WFloat:
				break;
				case WByte:

				break;
			}
			
		break;
		case array:
		break;
	}
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
			break;
			case CDebug:
				optimizeExp(c->printExp);
			break;

		}
		c = c->next;
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
	}
}

Exp* optimizeExpLogic(Exp* e){
	Exp* newExp = e;
	Exp* e1 = optimizeExp(e->bin.e1);
	Exp* e2 = optimizeExp(e->bin.e2);
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
		if(e->tag == ExpAdd)
			r = e1->c->u.d + e2->c->u.d;
		else if(e->tag == ExpSub)
			r = e1->c->u.d - e2->c->u.d;
		else if(e->tag == ExpMul)
			r = e1->c->u.d * e2->c->u.d;
		else if(e->tag == ExpDiv)
			r = e1->c->u.d / e2->c->u.d; //might have different results due to 0/0
		else {
			printf("SEVERE ERROR optimizer\n");
			printf("aborting optimization\n");
			return e;
		}
		//printf("r is %d\n", r);
		newExp=(Exp*)malloc(sizeof(Exp));
		newExp->tag = ExpPrim;
		newExp->c = (Constant*)malloc(sizeof(Constant));
		newExp->c->u.d = r;
		newExp->type = e->type;
	} else if(e1->tag == ExpPrim) { // translate to simple increments or decrements

	} else if(e2->tag == ExpPrim) {

	}
	//printf("Optimized exp\n");
	//printExp(newExp,2);
	return newExp;
}
Exp* optimizeExp(Exp* e) {
	
	if(!e)
		return e;
	//printExp(e,0);
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
			newExp->cast.e = optimizeExp(e->cast.e);
		break;
	}
	//printExp(newExp,0);
	return newExp;
}