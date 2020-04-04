
typedef struct CellUsage
{
	int start;
	int end;
} CellUsage;

int getNumberOfCells(CellUsage cu);
int getCellSize();
int getMemoryUsageInBytes(CellUsage cu);
CellUsage* makeCellUsage(int start, int end);




typedef enum constantType { KFloat, KInt, KStr, KBit } constantType;
typedef struct Constant
{
	constantType tag;
	union {
		const char* str;
		int i;
		double d;
	} u;
	CellUsage* limits;
	int start_cell;
} Constant;




typedef enum defType { DFunc, DVar} defType;

typedef enum BType { WInt, WByte ,WFloat, WShort, WBit, WChar} BType;
typedef enum Types { base, array} Types;

typedef struct Type
{
	Types tag;
	BType b;
	struct Type* of; //array of
	
} Type;

typedef struct Parameter
{
	Type* t;
	char* id;
	struct Parameter* next;
	int start_cell;
} Parameter;




typedef enum CTypes { CWhile, CIf, CIfElse, CReturn, CAssign, CBlock, CCall,CPrint, CDebug, COperator, CRead, CFor } CTypes;



typedef struct NameL
{
	const char* name;
	struct NameL* next;
	CellUsage limits;

} NameL;

typedef enum VScope { VGlobal,VLocal } VScope;

typedef struct DefVar
{
	Type* t;
	const char* id; //not used yet
	int scope; //think is enough
	int start_cell;
	CellUsage* limits;

} DefVar;

typedef struct DefVarL
{
	DefVar* dv;
	struct DefVarL* next;
} DefVarL;



typedef struct Var
{
	const char* id;
	Type* type;
	DefVar* declaration;
	int start_cell;
	//CellUsage* limits; //already in declaration.
} Var;

typedef enum ExpE {
	ExpAdd,
	ExpSub,
	ExpMul,
	ExpDiv,
	ExpUnary,
	ExpVar,
	ExpCall,
	ExpPrim,
	ExpCmp,
	ExpNew,
	ExpAccess,
	ExpCast,
	ExpOperator
} ExpE;



typedef struct Exp{
	ExpE tag;
	union {
		struct {
			struct Exp *e1, *e2;
		} bin;
		struct {
			struct Exp *e;
			enum {NOT, MINUS} op;
		} unary;
		struct {
			struct Exp *e;
			enum {INC, DEC} op;
			int amount;
		} opr;
		Constant* c;
		Var *var;
		struct {
			struct Exp *e;
			Type* t;
		} eNew;
		struct {
			const char* id;
			void* expList;
			void* def;
		} call;
		struct {
			struct Exp *e1, *e2;
			enum {EqEq, GTE, GT, LSE, LS, AND, OR } op;
		} cmp;
		struct {
			struct Exp *varExp, *indExp;
		} access;
		struct {
			struct Exp *e;
			Type* type;
		} cast;
		struct {
			struct Exp *e;
		} print;
	};
	Type* type;
	int dbg_line;
	int start_cell; //type and start cell are enough to infer size
} Exp;

typedef struct ExpList
{
	Exp* e;
	struct ExpList* next; 
	CellUsage* limits;
} ExpList ;


typedef struct CommandL
{
	CTypes tag;
	struct CommandL* next;
	Exp* condExp;
	struct CommandL* cmdIf;
	struct CommandL* cmdElse;
	struct {
			struct CommandL* begin;
			struct  Exp* cmpExp;
			struct CommandL* end;
			struct CommandL* block;
		} cFor;
	Exp* expLeft;
	Exp* expRight;
	Exp* retExp;
	Exp* printExp;
	Exp* oprExp;
	void* block;
} CommandL;



// typedef struct CommandL
// {
// 	CTypes tag;
// 	struct CommandL* next;

// } CommandL;
typedef struct Block
{	
	DefVarL* dvl;
	CommandL* cl;
	CellUsage* limits;
	
} Block;
typedef struct DefFunc
{
	const char* id;
	Type* retType;
	Parameter* params;
	Block* b;
	CellUsage* limits;

} DefFunc;
typedef struct Def {
	defType tag;
	union {
		DefFunc* f;
		DefVarL* v;
	} u;
	struct Def* next;
	CellUsage* limits;
} Def;


typedef struct progNode {
	Def* next;
	CellUsage* limits;
} progNode;

void printDefVar(DefVar* dv,int x);
void printDefFunc(DefFunc* df,int x);
extern progNode* globalTree;
void printTree();
void printDefVarList(DefVarL* dvl,int x);
void printDefList(Def* d,int x);
void printNameList(NameL* nl, int x);
void printType(Type* t,int x);
void printParams(Parameter* params, int x);
void printCommandList(CommandL* cl,int x);
void printBlock(Block* b,int x);
void printExp(Exp* e,int x);
void printVar(Var* v,int x);
void printConstant(Constant* c,int x);
void printExpList(ExpList* el,int x);
void freeTree();


/* Helping Parser */
void DefVarLinkEnd(DefVarL* dvl, DefVarL* e);
void DefVarLinkBegin(DefVarL* dvl, DefVarL* e);
DefVarL* NameLToDevL(NameL* nl, Type* t, int scope );
DefVarL* DefVarToDevL(DefVar* dv);
void DefVarLFix(DefVarL** dvlRef);
