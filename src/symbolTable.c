#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbolTable.h"

#if !defined(grammar_h)
	#include "grammar.h"
	#define grammar_h
#endif
#if !defined(compilerFunctions_h)
	#include "compilerFunctions.h"
	#define compilerFunctions_h
#endif
#if !defined(optimizer_h)
	#include "optimizer.h"
	#define optimizer_h
#endif

typedef struct SymbolListNode
{
	char* symbol;
	struct SymbolListNode* next;
} SymbolListNode; 

typedef struct SymbolListStack {
	SymbolListNode **list;
	int size;
	int current;
} SymbolListStack;

typedef struct Symbol {
	const char* id;
	Type* type;
	void* declaration;

} Symbol;

//static SymbolListStack symbolTable;

static Symbol variables[1000]; 
static int scopes[100]; 

static int scopesTop;
static int variablesTop;

static DefFunc* currentFunction = NULL;

static int flagFunctionHasReturn = 0;

//static int currentAllocationIndex = 4; // four registers
//static char* memory[30000]; //debugs and controlsfree memory




int warningCount=0;

int totalMemoryUsage=0;


void printSymbol(Symbol s);
Type* getTypeOfExp(Exp* e);
Type* typeOfConstant(Constant* c);
int typeEquals(Type* t1, Type* t2);
int typesBothIntegers(Type* t1,Type* t2);
void performCastToType(Type* lt,Exp** right);
int checkPrintability(Exp* e);
int checkReadability(Exp* e);
int checkTypeLogic(Exp* e);




DefFunc* findFuncInTree(const char* funcId);
void typeError(const char* message) {
	printf("Typing error: %s\n",message);
	exit(01);
}

void raiseError(const char* message,int line) {
	printf("Error: %s\n# near line %d\n",message,line);
	exit(01);
}
void raiseWarning(const char* message,int line) {
	printf("Warning: %s\n# near line %d",message,line);
	warningCount++;
}
void generateStardardDeclares(progNode* prog) {
	findFuncInTree("");
}

void incrementMemoryUsage(int bytes){
	totalMemoryUsage+=bytes;
}


Parameter* findParamsOfFunc(const char* funcId) {
	Def* dfl = globalTree->next;
	while(dfl) {
		if(dfl->tag == DFunc) {
			DefFunc* df = dfl->u.f;
			if(strcmp(funcId,df->id)==0) {
				return df->params;
			}
		}
		dfl = dfl->next;
	}
	return NULL;

}
DefFunc* findFuncInTree(const char* funcId) {
	Def* dfl = globalTree->next;
	while(dfl) {
		if(dfl->tag == DFunc) {
			DefFunc* df = dfl->u.f;
			if(strcmp(funcId,df->id)==0) {
				return df;
			}
		}
		dfl = dfl->next;
	}
	return NULL;

}

void printSymbol(Symbol s) {
	printType(s.type,0);
	printf("symbol %s \n",s.id);
}
void debugScopes() {
	for(int j = scopesTop-1;j>=0;j--) {
		printf(":::scope %d:::",j);
		for(int i = variablesTop-1;i>=0;i--) {
			printSymbol(variables[i]);	
		}
		printf("\n:::end %d:::\n",j);
	}
}

void initSymbolTable() {
	scopesTop=0;
	variablesTop=0;
	scopes[0] = 0;
	currentFunction = NULL;
}

void enterScope() {
	//printf("enterScope %d\n",scopesTop+1);
	scopes[scopesTop] = variablesTop; 
	scopesTop++;
	//scopes[scopesTop] = variablesTop; 
	//debugScopes();
}
void leaveScope() {
	//printf("leaveScope %d\n",scopesTop);
	scopesTop--;
	variablesTop = scopes[scopesTop];
	//debugScopes();
}
int find(const char * symbol) {
	int i;
	for(i=variablesTop-1;i>=0;i--) {
		if(strcmp(variables[i].id,symbol)==0) {
			return i;
		}
	}
	return -1;

}
int findCurrentScope(const char * symbol) {
	int i;

	for(i=variablesTop-1;i>=scopes[scopesTop];i--) {
		if(strcmp(variables[i].id,symbol)==0) {
			return i;
		}
	}
	return -1;

}
void insert(const char* symbolID,Type* type,void* d) {
	//printf("insert %s \n",symbolID);
	if(findCurrentScope(symbolID)>=0) {
		printf("--%s--\n", symbolID);
		typeError("Symbol was already declared in this scope");
	}
	variables[variablesTop].id = symbolID;
	variables[variablesTop].type = type;
	variables[variablesTop].declaration = d;
	variablesTop++;
	incrementMemoryUsage(cellsForType(type));

	//printf("variablesTop %d\n",variablesTop );
}

void typeTree(progNode* p)
{
	flagDebug = 0;
	typeDefList(p->next);
	//printf("Using %d bytes\n", totalMemoryUsage);
}
void typeDefList(Def* d)
{
	Def* df = d;
	while(df!=NULL) {
		switch(df->tag) {
			case DVar:
				// ndf = expandDefVar(d);
				// df = ndf;
				typeDefVarList(df->u.v);
			break;
			case DFunc:
				typeDefFunc(df->u.f);
			break;
		}
		df = df->next;
	}
}
void typeNameList(NameL* nl, Type* t,DefVar* dv) {
	if(!nl)
		return;
	NameL* p = nl;
	do  {
		insert(p->name,t,dv);
		p = p->next;
		//incrementMemoryUsage(cellsForType(t));

	} while(p);
}
void typeDefVar(DefVar* dv){
	
	if(!dv)
		return;
	dv->scope = scopesTop;
	//typeNameList(dv->nl,dv->t,dv);
	insert(dv->id,dv->t,dv);
}
void typeDefFunc(DefFunc* df)
{
	if(!df)
		return;
	// printType(df->retType );
	currentFunction = df;
	flagFunctionHasReturn = 0;
	insert(df->id,df->retType,df);
	enterScope();
	typeParams(df->params );
	typeBlock(df->b );
	leaveScope();
	if(df->b && 
		df->retType && 
		flagFunctionHasReturn == 0) {
		typeError("Non void function must return a value");
	}
	flagFunctionHasReturn = 0;
	currentFunction = NULL;
	//printf("end df %s\n",df->id);
}


void typeParams(Parameter* params )
{
	if(!params) {
		return;
	}
	Parameter* p = params;
	while(p) {
		insert(p->id,p->t,NULL);
		p = p->next; 
	}
}
void typeBlock(Block* b ) {
	
	if(!b)
		return;
	// ndvl = expandDefVarL(b->dvl);
	// b->dvl = ndvl;
	typeDefVarList(b->dvl );
	typeCommandList(b->cl );
}
void typeDefVarList(DefVarL* dvl ) {
	if(!dvl)
		return;
	DefVarL* d = dvl;
	while(d){
		typeDefVar(d->dv );
		d = d->next;
	}
}
int checkTypeReturn(Exp* retExp, DefFunc* df) {
	Type* t = NULL;
	if(retExp)
		t = retExp->type; 
	Type* ft = df->retType;
	return typeEquals(t,ft);
}
int checkCallability(Exp* callExp) {
	//printf("wsw\n");
	if(!callExp) {
		return 0; //cannot call null
	}
	Type* t = callExp->type;
	DefFunc* df = findFuncInTree(callExp->call.id);
	if(!df)
		return 0; //must be a function
	Type* ft = df->retType;
	return typeEquals(t,ft);
}
int checkIncDecremantability(Exp* oprExp){
	if(!oprExp){
		return 0;
	}
	if(oprExp->opr.e->tag == ExpPrim){
		return 0;
	}
	return 1;
}
void typeCommandList(CommandL* cl ) {
	//printf("commandList %p \n", (void*)cl);
	if(!cl)
		return;
	CommandL* c = cl;
	while(c) {
		switch(c->tag) {
			case CWhile:
				
				typeExp(c->condExp );
				if(!checkTypeLogic(c->condExp)) {
					raiseError("expression not suitble for condition",
						c->condExp->dbg_line);
				}
				enterScope();
				typeCommandList(c->cmdIf );
				leaveScope();
			break;
			case CFor:
				typeCommandList(c->cFor.begin);

				typeExp(c->cFor.cmpExp);
				if(!checkTypeLogic(c->cFor.cmpExp)) {
					raiseError("expression not suitble for condition",
						c->cFor.cmpExp->dbg_line);
				}
				enterScope();
				typeCommandList(c->cFor.block);
				typeCommandList(c->cFor.end);
				leaveScope();
			break;
			case CIf:
				typeExp(c->condExp );
				if(!checkTypeLogic(c->condExp)) {
					raiseError("expression not suitble for condition",
						c->condExp->dbg_line);
				}
				enterScope();
				typeCommandList(c->cmdIf );
				leaveScope();
			break;
			case CIfElse:
				typeExp(c->condExp );
				if(!checkTypeLogic(c->condExp)) {
					raiseError("expression not suitble for condition",
						c->condExp->dbg_line);
				}
				enterScope();
				typeCommandList(c->cmdIf );
				leaveScope();
				enterScope();
				typeCommandList(c->cmdElse );
				leaveScope();
			break;
			case CReturn:
				flagFunctionHasReturn = 1;
				typeExp(c->retExp );
				if(!currentFunction) {
					raiseError("Return not allowed outbounds",
						c->retExp->dbg_line);
				}
				if(!checkTypeReturn(c->retExp,currentFunction)) {
					raiseError("Return and func types mismatch",c->retExp->dbg_line);
				}
				if(c->next){
					raiseError("Return is not last statement of function",c->retExp->dbg_line);
				}
			break;
			case CAssign:
				
				typeExp(c->expLeft);
				typeExp(c->expRight);
				if(!typeEquals(c->expLeft->type,c->expRight->type)) {
					//printf("not equal types\n");
					if(typesBothIntegers(c->expLeft->type, c->expRight->type)) {
						//printf("auto cast\n");
					performCastToType(
						c->expLeft->type,
						&(c->expRight));
					}
					else {
						raiseError("Assigment left type differs from right type",c->expLeft->dbg_line);
					}
				}
				
			break;
			case CBlock:
				enterScope();
				typeBlock((Block*)c->block );
				leaveScope();
			break;
			case CCall:
				typeExp(c->expRight );
				if(!checkCallability(c->expRight)) {
					raiseError("Expression is not callable",c->expRight->dbg_line);
				}
			break;
			case CPrint:
				typeExp(c->printExp);
				if (!checkPrintability(c->printExp)) {
					raiseError("Expression is not printable",c->printExp->dbg_line);
				}
			break;
			case CRead:
				typeExp(c->printExp);
				if (!checkReadability(c->printExp)) {
					raiseError("Expression is not readable",c->printExp->dbg_line);
				}
			break;
			case CDebug:
				flagDebug = 1;
			break;
			case COperator:
				typeExp(c->oprExp);
				if (!checkIncDecremantability(c->oprExp)) {
					raiseError("Expression is suitable for increment/decrement",c->oprExp->dbg_line);
				}
			break;
		}
		c = c->next;
	}
}
void performCastToType(Type* lt,Exp** right) {
	Exp* eCast = (Exp*)malloc(sizeof(Exp));
	eCast->tag= ExpCast;
	eCast->cast.e = *right;
	eCast->cast.type = lt;
	eCast->type = lt;
	eCast->dbg_line = (*right)->dbg_line;
	*right = eCast;
}
int typesBothIntegers(Type* t1,Type* t2) {
	return t1->tag == base && t2->tag == base; // there is only byte, short and int for now
}
int typeEquals(Type* t1, Type* t2) {
	if(t1 == NULL || t2 == NULL) {
		return t2 == NULL && t1 == NULL;
	}
	if(t1->tag == base && t2->tag == base) {
		if(t1->b == t2->b) {
			return 1;
		}
		return 0;
	}
	if(t1->tag == array && t2->tag == array) {
		return typeEquals(t1->of,t2->of);
	}
	return 0;
}
/* 
Em qualquer expressão, 
uma variável char tem seu valor 
automaticamente promovido para int.
*/



/* Um argumento vira uma definição de variável */

static DefVar* paramToDefVar(Parameter* param){
	DefVar* dv = (DefVar*)malloc(sizeof(DefVar));
	dv->id = param->id;
	dv->t = param->t;
	//dv->nl = (NameL*)malloc(sizeof(NameL));
	//dv->nl->name = param->id;
	//dv->nl->next = NULL;
	return dv;
}


/* Transforma argumentos em variáveis
*/
void adaptFuncParamsToVars(DefFunc* df){
	
	DefVarL* param_dvl = (DefVarL*)malloc(sizeof(DefVarL));
	param_dvl->next = df->b->dvl;
	
	DefVarL* dvl = param_dvl;
	for(Parameter* p = df->params; p ; p = p->next){
		DefVarL* ndvl = (DefVarL*)malloc(sizeof(DefVarL));
		ndvl->dv = paramToDefVar(p);
		ndvl->next = dvl;
		dvl = ndvl;
	}
	

}


void promoteType(Exp ** eptr) {
	Exp* e = *eptr;
	if(e->type->tag == base && e->type->b == WByte) {
		Type* t = (Type*)malloc(sizeof(Type));
		t->tag = base;
		t->b = WInt;
		performCastToType(t,eptr);
	}
}

BType BTypeOfArith(Exp* e1,Exp *e2) {
	if(e1->type->b == WFloat || e2->type->b == WFloat)
		return WFloat;
	else if(e1->type->b == WShort || e2->type->b == WShort)
		return WShort;
	else if(e1->type->b == WInt || e2->type->b == WInt)
		return WInt;
	return WByte;
}
Type* arithType(Exp* e) {
	Type* t = (Type*)malloc(sizeof(Type));
	t->tag = base;
	t->b = BTypeOfArith(e->bin.e1,e->bin.e2);
	return t;
}

Type* unaryType(Exp* e) {
	return e->unary.e->type;
}

Type* CmpType(Exp* e) {
	Type* t = (Type*)malloc(sizeof(Type));
	t->tag = base;
	t->b = WInt;
	return t;
}

Type* typeOfCall(Exp* e) {
	if(!e)
		return NULL;
	int index = find(e->call.id);
	if(checkCompilerFunctions(e->call.id) >= 0) {
		return NULL;
	}
	if(index < 0) {
		printf("--func %s--\n",e->call.id);
		raiseError("No such func in scope",e->dbg_line);
	}
	return variables[index].type;
}

Type* typeOfNew(Exp* e) {
	Type* t = (Type*)malloc(sizeof(Type));
	t->tag = array;
	t->of = e->eNew.t;
	return t;
}

Type* typeOfAccess(Exp* e) {
	int index = find(e->call.id);
	if(checkCompilerFunctions(e->call.id) >= 0) {
		return NULL;
	}
	if(index < 0) {
		printf("--func %s--\n",e->call.id);
		raiseError("No such func in scope",e->dbg_line);
	}
	return variables[index].type;
}

int checkTypeIndex(Exp* e) {
	return e->type->tag == base && e->type->b == WInt;
}

int checkTypeArtih(Exp* left,Exp *right) {
	// printExp(left,0);
	// printExp(right,0);
	if(left->type->tag == base && right->type->tag == base) {
		if(typeEquals(left->type,right->type)) {
			return 1;
		}
		if(left->type->b == WByte || right->type->b == WByte) {
			if(left->type->b == WInt || right->type->b == WInt) {
				return 1;
			}
		}
	}
	return 0;
}



int chekTypeMinusUnary(Exp* e) {
	Type* t = e->type;
	if(!t)
		return 0;
	return t->tag == base;
}

//receives exp inside unary
int checkTypeUnary(Exp* e) {
	Type* t = e->type;
	if(!t)
		return 0;
	return t->tag == base;
}

int checkTypeLogic(Exp* e) {
	if(e->type->tag == base)
		return 1;
	return 0;
}

int checkTypeCast(Exp* e) {
	//printExp(e,5);
	if(e->type->tag == base) {
		return e->cast.e->type->tag == base;
	}
	return 0;
}

int checkTypeExpList(ExpList* el,Parameter* params) {
	if(el == NULL || params == NULL)
		return !el && !params;
	ExpList *p = el;
	//printExpList(el,0);
	//printParams(params,0);
	//puts("");
	while(p) {
		//printf("a\n");
		if(!typeEquals(params->t,p->e->type)) {
			printf("Not equal typing");
			return 0;
		}
		p = p->next;
		params = params->next;
		if(!params && p) {
			return 0;
		}
		incrementMemoryUsage(2);
	}
	return params == NULL;
}
int checkTypeCallParamsArgs(Exp* e) {
	//printf("checkTypeCallParamsArgs\n");
	if(!e)
		return 0;
	int index = find(e->call.id);
	if(index<0)
		return 0;
	Parameter* params = findParamsOfFunc(e->call.id);
	return checkTypeExpList(e->call.expList,params);
}

int checkPrintability(Exp* e) {
	if(!e)
		return 0;
	if(e->type == NULL)
		return 0;
	if(e->type->tag == base) {
		return 1;
	}
	if(e->type->tag == array) { //can print strings
		if(e->type->of->tag == base 
			&& e->type->of->b ==WByte) {
			return 1;
		}
	}
	return 0;
}
int checkReadability(Exp* e) {
	if(!e)
		return 0;
	if(e->type == NULL)
		return 0;
	
	if(e->tag == ExpVar){
		if(e->type->tag == base) {
			return 1;
		}
		if(e->type->tag == array) { //can print strings
			if(e->type->of->tag == base 
				&& e->type->of->b ==WByte) {
				return 1;
			}
		}
	}
	return 0;
}

int checkAccess(Exp* e) {
	Exp* v = e->access.varExp;
	if(v->tag == ExpAccess || v->tag == ExpVar || v->tag == ExpCall) {
		return 1;
	}
	return 0;
}

int checkAccessType(Exp* e) {
	Exp* v = e->access.varExp;
	if(v->type->tag == array) {
		return 1;
	}
	return 0;
}

void typeExp(Exp* e ) {
	if(!e)
		return;

	switch(e->tag) {
		case ExpAdd: 
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)) {
				// printType(e->bin.e1->type,0);
				// printType(e->bin.e2->type,0);
				raiseError("Types in Add differs",
					e->dbg_line);
			}
			e->type = arithType(e);
		break;
		case ExpSub:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)){
				// printType(e->bin.e1->type,0);
				// printType(e->bin.e2->type,0);
				raiseError("Types in Sub differs",
					e->dbg_line);
			}
			e->type = arithType(e);
		break;
		case ExpMul:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)) {
				// printExp(e->bin.e1,0);
				// printExp(e->bin.e2,0);
				raiseError("Types in Mul differs.",
					e->dbg_line);
			}
			e->type = arithType(e);
		break;
		case ExpDiv:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)) {
				// printExp(e->bin.e1,0);
				// printExp(e->bin.e2,0);
				raiseError("Types in Div differs",e->dbg_line);
			}
			e->type = arithType(e);
		break;
		case ExpCall:
			typeExpList(e->call.expList);
			e->type = typeOfCall(e);
			e->call.def = (void*) findFuncInTree(e->call.id);
			if(!checkCallability(e)) {
					//printf("--%s--\n", e->call.id);
					raiseError("Expression is not callable",e->dbg_line);
				}
			if(!checkTypeCallParamsArgs(e)) {
				printf("--%s--\n", e->call.id);
				raiseError("Params typing differs from arguments in call",e->dbg_line);
			}
		break;
		case ExpVar:
			typeVar(e->var);
			e->type = e->var->type;
		break;
		case ExpUnary:
			typeExp(e->unary.e);
			if(e->unary.op == MINUS) {
				if(!chekTypeMinusUnary(e->unary.e))
				{
					raiseError("type of -(exp) not right",
						e->dbg_line);
				}
			}
			else {
				if(!checkTypeUnary(e->unary.e))
				{
					raiseError("type of !(exp) not right",
						e->dbg_line);
				}
			}	
			e->type = unaryType(e);
		break;
		case ExpPrim:
			e->type = typeOfConstant(e->c);
		break;
		case ExpNew:
			typeExp(e->eNew.e);
			if(!checkTypeIndex(e->eNew.e)) {
				raiseError("expression inside \"[]\" is not an int",e->dbg_line);
			}
			e->type = typeOfNew(e);
		break;
		case ExpCmp:
			typeExp(e->cmp.e1);
			typeExp(e->cmp.e2);
			//promoteType(&e->cmp.e1);
			//promoteType(&e->cmp.e2);
			switch(e->cmp.op) {
				default:
					if(!checkTypeLogic(e->cmp.e1) ||
					!checkTypeLogic(e->cmp.e2))  {
						raiseError("Types not suitble for logic",e->dbg_line);
					}
				break;
				case EqEq:
					if(!typeEquals(e->cmp.e1->type,e->cmp.e2->type)) {
						if(!typesBothIntegers(e->cmp.e1->type,e->cmp.e2->type)) {
							// printType(e->cmp.e1->type,0);
							// printType(e->cmp.e2->type,0);
							raiseError("Not comparable types in ==",e->dbg_line);
						}
					}
				break;

			}
			e->type = CmpType(e);
		break;
		case ExpAccess:
			typeExp(e->access.varExp);
			typeExp(e->access.indExp);
			if(!checkTypeIndex(e->access.indExp)) {
				raiseError("Index of array is not an int",e->dbg_line);
			}
			if(!checkAccess(e)) {
				raiseError("Access in something that is not an array, nor variable, nor function return",e->dbg_line);
			}
			if(!checkAccessType(e)) {
				raiseError("Access in something that does not have array typing",e->dbg_line);
			}

			e->type = e->access.varExp->type->of;
		break;
		case ExpCast:
			typeExp(e->cast.e);
			if(!checkTypeCast(e)) {
				// printType(e->type,0);
				// printType(e->cast.type,0);
				// printType(e->cast.e->type,0);
				raiseError("Cast not avaible for these types",e->dbg_line);
			}
			if(typeEquals(e->cast.type,e->cast.e->type)){
				raiseWarning("Cast to equal typing",e->dbg_line);
			}
			e->type = e->cast.type;
		break;
		case ExpOperator:
			typeExp(e->opr.e);
			e->type = e->opr.e->type;
		break;
		
	}
	//printExp(e,10);
}
Type* typeOfConstant(Constant* c) {
	if(!c)
		return NULL;
	Type* t = (Type*)malloc(sizeof(Type));
	t->tag = base;
	switch(c->tag) {
		case KInt:
			t->b = WByte;
			// if(c->u.i>255){
			// 	t->b = WShort;
			// }
			incrementMemoryUsage(2);
		break;
		case KFloat:
			t->b = WFloat;
		break;
		case KStr:
			t->tag = array;
			t->of = (Type*)malloc(sizeof(Type));
			t->of->tag = base;
			t->of->b = WByte;
		break;
		case KBit:
			t->b = WBit;
		break;
	}
		return t;
}
Type* getTypeOfExp(Exp* e) {

	if(e->tag == ExpPrim)
		return typeOfConstant(e->c);
	return NULL;
}


void typeExpList(ExpList* el ) {
	if(!el)
		return;
	ExpList *p = el;
	while(p) {
		typeExp(p->e);
		p = p->next;
	}
	return;
}

void typeVar(Var* v) {
	if(!v)
		return;
	int index = find(v->id);
	if(index < 0) {
		printf("--var %s--\n",v->id);
		typeError("No such var in scope");
	}
	v->type = variables[index].type;
	DefVar* dv = (DefVar*)variables[index].declaration;
	v->declaration = dv;
	if(findFuncInTree(v->id)) {
		typeError("Subject is a function, not a var");
	}
}
void checkAndFixesTypesInTree() 
{
	typeTree(globalTree);
}

int cellsForType(Type* t){
	if(!t)
		return -1;
	switch(t->tag) {
		case base:
			switch(t->b) {
				case WInt:
					return 8;
				break;
				case WFloat:
					return 4;
				break;
				case WShort:
					return 2;
				break;
				case WByte:
					return 1;
				break;
				case WBit:
					return 1;
				break;
				case WChar:
					return 1;
				break;
			}
		break;
		case array:
			return -1;
		break;
	}
}





