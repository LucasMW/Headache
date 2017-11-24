#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#include "optimizer.h"
#include <stdlib.h>
#include <stdio.h>

Exp* optimizeExpBin(Exp* e){
	Exp* newExp = e;
	Exp* e1 = optimizeExp(e->bin.e1);
	Exp* e2 = optimizeExp(e->bin.e2);
	if(e1->tag == ExpPrim && e2->tag == ExpPrim) {
		int r;
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
			printf("aborting optimizer\n");
			return e;
		}
		newExp = (Exp*)malloc(sizeof(Exp));
		newExp->tag = ExpPrim;
		newExp->c->u.d = r;
	} else if(e1->tag == ExpPrim) { // translate to simple increments or decrements

	} else if(e2->tag == ExpPrim) {

	}
	return newExp;
}
Exp* optimizeExp(Exp* e) {
	
	if(!e)
		return e;
	
	Exp* newExp = e; //case nothing happens, should return the same
	// Exp* e1;
	// Exp* e2;

	switch(e->tag) {
		case ExpAdd:
		case ExpSub:
		case ExpMul:
		case ExpDiv:
			newExp = optimizeExpBin(e);
		break;
		case ExpCall:
			
		break;
		case ExpVar:
			
		break;
		case ExpUnary:
			
		break;
		case ExpPrim:
				
		break;
		case ExpNew:
			
		break;
		case ExpCmp:
			
		break;
		case ExpAccess:
			
		break;
		case ExpCast:
			
		break;
	}

	return newExp;
}