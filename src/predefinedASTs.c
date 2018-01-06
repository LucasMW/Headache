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

(void*) printBitAST(){
	$$ = (CommandL*)malloc(sizeof(CommandL));
          $$->tag = CIfElse;
          $$->condExp = $3;
          $$->cmdIf = $5;
          $$->cmdElse = $7;

}