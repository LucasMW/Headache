/* The grammar for monga*/
/* grammar.y */

%error-verbose /* instruct bison to generate verbose error messages*/

%{
#include "lex.h"
int yyerror(const char *s);
int yylex(void);
#include <stdio.h>
#include <stdlib.h>
#define YYDEBUG 1
extern FILE *yyin;
#if !defined(tree_h)
  #include "tree.h"
  #define tree_h
   #endif
#if !defined(compilerFunctions_h)
  #include "compilerFunctions.h"
  #define compilerFunctions_h
   #endif
%}


%union{
  int		int_val;
  char*	str_val;
  double double_val;
  progNode* prog;
  Def* def;
  DefVar* dVar;
  DefFunc* dFunc;
  Parameter* param;
  Exp* exp;
  Type* type;
  NameL* namelist;
  CommandL* cmd;
  Block* block;
  DefVarL* dvl;
  ExpList* el;
  Constant* cons;


}

%start program

%token <int_val> TK_GE
%token <int_val> TK_LE
%token <int_val> TK_INT
%token <int_val> TK_WSHORT
%token <int_val> TK_WBYTE
%token <int_val> TK_FLOAT
%token <int_val> TK_WAS
%token <int_val> TK_WCHAR
%token <int_val> TK_WELSE
%token <int_val> TK_WFLOAT
%token <int_val> TK_WIF
%token <int_val> TK_WINT
%token <int_val> TK_WNEW
%token <int_val> TK_WRETURN
%token <int_val> TK_WVOID
%token <int_val> TK_WWHILE
%token <int_val> TK_AND
%token <int_val> TK_OR
%token <int_val> TK_EQEQ
%token <str_val> TK_VAR
%token <str_val> TK_STR
%token <str_val> TK_INC
%token <str_val> TK_DEC



%type<prog> program  
%type <cmd> command command1  commandList commandList2 commandIF commandWhile commandPrint commandDebug
%type <block> block
%type <param> parameters parameter 
%type <def> definitionList  definition 
%type <dVar> defVar 
%type <dFunc> defFunc
%type <exp> expUnary expVar  expOr expLogic expCmp  expMul expAdd expCall expCast expNew exp primary
%type <type> type;
%type <namelist> idList idList2 nameList
%type <int_val> baseType 
%type <str_val> ID
%type <dvl> defVarList defVarList2
%type <el> expList expList2
%type <cons> constant;

%right "if" TK_WELSE // Same precedence, but "shift" wins.
%%
program : definitionList  {
  $$ = (progNode*)malloc(sizeof(progNode));
  $$->next = $1;
  globalTree = $$;
}
;

definitionList: {
        $$ = NULL; //printf(">>>>Null\n");
}
            | definition definitionList {
              $$ = $1;
              $$->next = $2;
            }

;
definition : defVar {
  $$ = (Def*)malloc(sizeof(Def));
  $$->u.v = $1;
  $$->tag = DVar;
  $$->next = NULL;
  //printDefVar($$->u.v);
}
| defFunc {
  $$ = (Def*)malloc(sizeof(Def));
  $$->u.f = $1;
  $$-> tag = DFunc;
  $$->next = NULL;
}
;

defFunc : type ID '(' parameters ')' block {//printf("typed %s ",$2);
          $$ = (DefFunc*)malloc(sizeof(DefFunc));
          $$->id = $2;
          $$->retType = $1;
          $$->params = $4;
          $$->b = $6;

        }
        | TK_WVOID ID '(' parameters ')' block {//printf("untyped %s ",$2);
        $$ = (DefFunc*)malloc(sizeof(DefFunc));
        $$->id = $2;
        $$->retType = NULL;
        $$->params = $4;
        $$->b = $6;
      }
        | type ID '(' parameters ')' ';' {

          $$ = (DefFunc*)malloc(sizeof(DefFunc));
          $$->id = $2;
          $$->retType = $1;
          $$->params = $4;
          $$->b = NULL;
        }
        | TK_WVOID ID '(' parameters ')' ';' {
          $$ = (DefFunc*)malloc(sizeof(DefFunc));
          $$->id = $2;
          $$->retType = NULL;
          $$->params = $4;
          $$->b = NULL;
        } 

;

parameters :   {
          $$ = NULL;
}
        | parameter  { 
          $$=$1;
          //printf("jone\n");
        }
        | parameter ',' parameters { 
          $$ =  $1;
          $$->next = $3;
        }

parameter : type ID {
    $$ = (Parameter*)malloc(sizeof(Parameter));
    $$->t = $1;
    //printType($1);
    $$->id = $2;
    $$->next = NULL;
    //printParams($$);
}
;
command : command1 {
  $$=$1;
}
;

defVar : type nameList ';' {  //correct
  //printf("defVar\n");
  $$ = (DefVar*)malloc(sizeof(DefVar));
  $$->t = $1;
  $$->nl = $2;
  $$->scope = VGlobal;
  $$->id = $2->name;
  
}
;

nameList: ID idList {
  
   $$ = (NameL*)malloc(sizeof(NameL));
   $$->name = $1;
   $$->next = $2;
   //printf(" namelist: ");
   //printNameList($$);
   //printf("\n");
}

idList: {$$ = NULL;}
    |idList2 { $$ = $1; //printf("ONE ID");
  }
idList2: ID { 
    $$ = (NameL*)malloc(sizeof(NameL));
    $$->name = $1;
    $$->next = NULL;
             //printf("<nl null>");
}
    | ',' ID idList {
      $$ = (NameL*)malloc(sizeof(NameL));
      $$->name = $2;
      $$->next = $3;
    }

block : '{'  defVarList   commandList  '}'
{
  $$ = (Block*) malloc (sizeof(Block));
  $$->dvl = $2;
  $$->cl = $3;
};


defVarList : {
  $$ = NULL;
}
| defVarList2 {
  $$ = $1;
}

defVarList2: defVar defVarList {
  $$ = (DefVarL*)malloc(sizeof(DefVarL));
  $$->dv = $1;
  $$->next = $2;
}

commandList: {
  $$=NULL;
}
| commandList2 {
  $$ = $1;
}

commandList2: command commandList {
  $$= $1;
  $$->next = $2;
}

commandPrint: '@' exp ';' {
  $$ = (CommandL*)malloc(sizeof(CommandL));
  $$->tag = CPrint;
  $$->printExp = $2;
}
;
commandDebug: '%' exp ';' {
  $$ = (CommandL*)malloc(sizeof(CommandL));
  $$->tag = CDebug;
  $$->printExp = $2;
}
;
commandIF: TK_WIF '(' exp ')' command %prec "if" {  
   $$ = (CommandL*)malloc(sizeof(CommandL));
          $$->tag = CIf;
          $$->condExp = $3;
          $$->cmdIf = $5;
} |  TK_WIF '(' exp ')' command TK_WELSE command %prec "else" {
          $$ = (CommandL*)malloc(sizeof(CommandL));
          $$->tag = CIfElse;
          $$->condExp = $3;
          $$->cmdIf = $5;
          $$->cmdElse = $7;
}
;

commandWhile: TK_WWHILE '(' exp ')' command %prec "if" {
          $$ = (CommandL*)malloc(sizeof(CommandL));
          $$->tag = CWhile;
          $$->condExp = $3;
          $$->cmdIf = $5;
}
;

command1: TK_WRETURN  ';' {
   $$ = (CommandL*)malloc(sizeof(CommandL));
   $$->tag = CReturn;
   $$->retExp = NULL;
}
        | TK_WRETURN exp ';' {
           $$ = (CommandL*)malloc(sizeof(CommandL));
           $$->tag = CReturn;
           $$->retExp = $2;
        }
        | expCall ';' {
          $$ = (CommandL*)malloc(sizeof(CommandL));
          $$->tag = CCall;
          $$->expRight = $1;
        }
        | block {
          $$ = (CommandL*)malloc(sizeof(CommandL));
          $$->tag = CBlock;
          $$->block = $1; //abuse
        }
        | commandIF {
          $$=$1;
        }
        | expVar '=' exp ';' {
          $$ = (CommandL*)malloc(sizeof(CommandL));
          $$->tag = CAssign;
          $$->expLeft = $1;
          $$->expRight = $3;
        }
        | commandWhile {
          $$=$1;
        } 
        | commandPrint {
          $$ = $1;
        }
        | commandDebug {
          $$ = $1;
        }
;

//exps
exp: expNew {
  $$ = $1;
  $$->dbg_line = yy_lines;
} 
 | expCast {
      $$=$1;
      $$->dbg_line = yy_lines;
    }
;


expCast: exp TK_WAS type {
  $$ = (Exp*)malloc(sizeof(Exp));
  $$->tag = ExpCast;
  $$->cast.type = $3;
  $$->type = $3;
  $$->cast.e = $1;

}

expCall: TK_VAR '(' expList ')' {
  $$ = (Exp*)malloc(sizeof(Exp));
  $$->tag = ExpCall;
  $$->call.id = $1;
  $$->call.expList = $3;
  $$->dbg_line = yy_lines;
}
;
expList: {
  $$=NULL;
}
      | expList2 {
        $$=$1;
      }
;
expList2: exp {
  $$ = (ExpList*)malloc(sizeof(ExpList));
  $$->e = $1;
  $$->next = NULL;
}
      | exp ',' expList {
        $$ = (ExpList*)malloc(sizeof(ExpList));
        $$->e =$1;
        $$->next = $3;
      }
;


expNew: TK_WNEW type '[' exp ']' {
      $$ = (Exp*)malloc(sizeof(Exp));
      $$->tag = ExpNew;
      $$->eNew.t = $2;
      $$->eNew.e = $4;
}
      | expLogic {
        $$ = $1;
      }
;


expLogic : expLogic TK_AND expCmp {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpCmp; //&&
        $$->cmp.e1 = $1;
        $$->cmp.e2 = $3;
        $$->cmp.op = AND;
      }
      | expOr {
        $$=$1;
      }
;    
expOr : expLogic TK_OR expCmp {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpCmp; //||
        $$->cmp.e1 = $1;
        $$->cmp.e2 = $3;
        $$->cmp.op = OR;
      }
      | expCmp {
        $$ = $1;
      }
;

expCmp: expCmp TK_EQEQ expAdd {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpCmp;
        $$->cmp.e1 = $1;
        $$->cmp.e2 = $3;
        $$->cmp.op = EqEq;
      }
      | expCmp TK_GE expAdd {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpCmp; //>=
        $$->cmp.e1 = $1;
        $$->cmp.e2 = $3;
        $$->cmp.op = GTE;
      }
      | expCmp TK_LE expAdd {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpCmp; //<=
        $$->cmp.e1 = $1;
        $$->cmp.e2 = $3;
        $$->cmp.op = LSE;
      }
      | expCmp '>' expAdd {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpCmp; //>
        $$->cmp.e1 = $1;
        $$->cmp.e2 = $3;
        $$->cmp.op = GT;
      }
      | expCmp '<' expAdd {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpCmp; //<
        $$->cmp.e1 = $1;
        $$->cmp.e2 = $3;
        $$->cmp.op = LS;

      }
      | expAdd {
        $$=$1;
      }
;

//arith
expAdd: expAdd '+' expMul { 
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpAdd; //<
        $$->bin.e1 = $1;
        $$->bin.e2 = $3;
        $$->dbg_line = yy_lines;
}
      | expAdd '-' expMul { 
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpSub;
        $$->bin.e1 = $1;
        $$->bin.e2 = $3;
        $$->dbg_line = yy_lines;
      }
      | expMul {
        $$=$1;
      }
;
expMul: expMul '*' expUnary {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpMul; //<
        $$->bin.e1 = $1;
        $$->bin.e2 = $3;
        $$->dbg_line = yy_lines;
}
      | expMul '/' expUnary {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpDiv; //<
        $$->bin.e1 = $1;
        $$->bin.e2 = $3;
        $$->dbg_line = yy_lines;
      }
      | expUnary {
        $$=$1;
      }

expUnary: '!' expVar {  
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpUnary;
        $$->unary.op = NOT;
        $$->unary.e = $2;
        $$->dbg_line = yy_lines;
         } //!(exp) = 
      | '-' expVar {  //-(exp) = (0 - exp)
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpUnary;
        $$->unary.op = MINUS;
        $$->unary.e = $2;
        $$->dbg_line = yy_lines;
      }
      | expVar  {
        $$=$1;
      }
;

expVar: expVar '[' exp ']' {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpAccess; 
        $$->access.varExp = $1;
        $$->access.indExp = $3;
        $$->dbg_line = yy_lines;


}
      | ID {
        $$ = (Exp*)malloc(sizeof(Exp));
        $$->tag = ExpVar; 
        $$->var = (Var*)malloc(sizeof(Var));
        $$->var->id = $1;
        $$->var->declaration = NULL;
        $$->dbg_line = yy_lines;
      }
      | primary {
        $$ = $1;
      }
      | expCall {
        $$=$1;
      }

primary: constant {
  $$ = (Exp*)malloc(sizeof(Exp));
  $$->tag = ExpPrim;
  $$->c = $1;
  switch($1->tag) {
    case KInt:
    $$->type = (Type*)malloc(sizeof(Type));
    $$->type->tag = base; 
    $$->type->b = WInt;
    break;
    case KFloat:
    $$->type = (Type*)malloc(sizeof(Type));
    $$->type->tag = base; 
    $$->type->b = WFloat;
    break;
    case KStr:
    $$->type = (Type*)malloc(sizeof(Type));
    $$->type->tag = base; 
    $$->type->of = (Type*)malloc(sizeof(Type));
    $$->type->of->tag = base;
    $$->type->of->b = WByte;
    break;
  }
  //printConstant($$->c);
}
      | '(' exp ')' {
        $$ = $2; 
      }
;
constant: TK_INT  {     
        $$ = (Constant*)malloc(sizeof(Constant));
        $$->tag = KInt;
        $$->u.i = yylval.int_val;
        //printf("%d\n", $$->u.i);
      }
      | TK_FLOAT  {
        $$ = (Constant*)malloc(sizeof(Constant));
        $$->tag = KFloat;
        $$->u.d = yylval.double_val;
        //printf("%lf\n", $$->u.d);
      }
      | TK_STR    {//$$=(char*)$1;
        $$ = (Constant*)malloc(sizeof(Constant));
        $$->tag = KStr;
        $$->u.str = yylval.str_val ;
        //printf("%s\n", $$->u.str);
      }
;
ID: TK_VAR { $$=yylval.str_val;
  //printf("id:%s\n",$$);
}
;
type : baseType { $$ = (Type*)malloc(sizeof(Type));
                  $$->tag = base; 
                  //printf("base %d\n",$1); 
                 $$->b = $1; }
    | type '[' ']' {
      $$ = (Type*)malloc(sizeof(Type)); 
      $$->tag =array;
      $$->of = $1;
    }
;
baseType : TK_WINT { $$ = WInt;}
| TK_WBYTE  { $$ = WByte;}
| TK_WFLOAT {$$ = WFloat;}
;
%%

int yyerror(const char* s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  printf("Syntax Error at token \"%s\" at line %d \n",yytext,yy_lines);
  //printf("yyval.str_val = %s\n",yylval.str_val);
  exit(1);
}



