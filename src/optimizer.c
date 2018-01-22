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
			case COperator:
				optimizeExp(c->oprExp);
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
		if(e->tag == ExpAdd)
			r = (char)e1->c->u.i + (char)e2->c->u.i;
		else if(e->tag == ExpSub)
			r = (char)e1->c->u.i - (char)e2->c->u.i;
		else if(e->tag == ExpMul)
			r = (char)e1->c->u.i * (char)e2->c->u.i;
		else if(e->tag == ExpDiv) {
			//Dealing with 0
			if(e2->c->u.i == 0 && e1->c->u.i == 0){
				r = 0;
			}
			else if(e2->c->u.i == 0){
				raiseWarning("cannot optimize",e->dbg_line);
				return e; //abort optimization. Cannot optimize this
			} 
			else {
				r = (char)e1->c->u.i / (char)e2->c->u.i; 
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
		newExp->c->u.i = r;
		newExp->c->start_cell = 0;

		
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