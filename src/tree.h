typedef enum constantType { KFloat, KInt, KStr } constantType;
typedef struct Constant
{
	constantType tag;
	union {
		const char* str;
		int i;
		double d;
	} u;

} Constant;




typedef enum defType { DFunc, DVar} defType;

typedef enum BType { WInt, WByte ,WFloat} BType;
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
} Parameter;




typedef enum CTypes { CWhile, CIf, CIfElse, CReturn, CAssign, CBlock, CCall,CPrint } CTypes;



typedef struct NameL
{
	const char* name;
	struct NameL* next;
} NameL;

typedef enum VScope { VGlobal,VLocal } VScope;

typedef struct DefVar
{
	Type* t;
	const char* id; //not used yet
	NameL* nl;
	int scope; //think is enough
	int start_cell;

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
	ExpCast
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
		Constant* c;
		Var *var;
		struct {
			struct Exp *e;
			Type* t;
		} eNew;
		struct {
			const char* id;
			void* expList;
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
} Exp;

typedef struct ExpList
{
	Exp* e;
	struct ExpList* next; 
} ExpList ;


typedef struct CommandL
{
	CTypes tag;
	struct CommandL* next;
	Exp* condExp;
	struct CommandL* cmdIf;
	struct CommandL* cmdElse;
	Exp* expLeft;
	Exp* expRight;
	Exp* retExp;
	Exp* printExp;
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
	
} Block;
typedef struct DefFunc
{
	const char* id;
	Type* retType;
	Parameter* params;
	Block* b;


} DefFunc;
typedef struct Def {
	defType tag;
	union {
		DefFunc* f;
		DefVar* v;
	} u;
	struct Def* next;
} Def;


typedef struct progNode {
	Def* next;
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


Constant* makeConstant(constantType t);
void notConst();
void minusConst();
void makeID();