#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif

void checkAndFixesTypesInTree();
void typeTree(progNode* p); 
void typeDefVar(DefVar* dv);
void typeDefFunc(DefFunc* df);
void typeDefVarList(DefVarL* dvl);
void typeDefList(Def* d);
void typeNameList(NameL* nl,Type* t,DefVar* dv);
void typeType(Type* t);
void typeParams(Parameter* params);
void typeCommandList(CommandL* cl);
void typeBlock(Block* b);
void typeExp(Exp* e);
void typeVar(Var* v);
void typeConstant(Constant* c);
void typeExpList(ExpList* el);
int cellsForType(Type* t);
void raiseWarning(const char* message,int line);
void raiseError(const char* message,int line);
static char flagDebug;