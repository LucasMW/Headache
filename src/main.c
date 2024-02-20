#include <stdio.h>
#include <stdlib.h>
#if !defined(lex_h)
	#include "lex.h"
	#define lex_h
#endif
#if !defined(bison_h)
	#include "bison.h"
	#define bison_h
#endif
#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#if !defined(grammar_h)
	#include "grammar.h"
	#define grammar_h
#endif
#if !defined(lextest_h)
	#include "lextest.h"
	#define lextest_h
#endif
#if !defined(highlight_h)
	#include "highlight.h"
	#define highlight_h
#endif
#if !defined(symbolTable_h)
	#include "symbolTable.h"
	#define symbolTable_h
#endif
#if !defined(codeGen_h)
	#include "codeGen.h"
	#define codeGen_h
#endif
#if !defined(testbfi_h)
	#include "testbfi.h"
	#define testbfi_h
#endif
#if !defined(expander_h)
	#include "expander.h"
	#define expander_h
#endif
#if !defined(optimizer_h)
	#include "optimizer.h"
	#define optimizer_h
#endif
#if !defined(codellvm_h)
	#include "codellvm.h"
	#define codellvm_h
#endif

#include <string.h>
#include <assert.h>
Seminfo_t seminfo;
int yy_lines=1; //save one for EOF

extern FILE* yyin;

#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif
#ifndef YY_TYPEDEF_YY_SIZE_T
#define YY_TYPEDEF_YY_SIZE_T
typedef size_t yy_size_t;
#endif
extern YY_BUFFER_STATE yy_scan_string (char *base);


char forceExpand=0;
void lexError(const char* message, int ret)
{
	printf("Lexical error detected in source:\n");
	fprintf(stderr,"ERROR: %s\t\t#line %d\n", message, yy_lines);
	exit( ret ? ret : 1);
}

static char* hacOptions[] = {
	"-check",
	"-syntax",
	"-lex",
	"-tree",
	"-noChecks",
	"-noBin",
	"-noCode"
};

static char hacOptionsCount = 7;


static char* optimizationOptions[] = {
	"-O0",
	"-O1",
	"-O2"
};

static char optimizationOptionsCount = 3;

static char* breakingOptions[] = {
	"--help",
	"--version",
	"--maga",
};
static char breakingOptionsCount = 3;

static char hacVersion[] = "v0.80.0b (LLVM)";

static int isOption(const char* candidate){
	for (int i=0;i<hacOptionsCount;i++){
		if(strcmp(candidate,hacOptions[i])==0){
			return 1;
		}
	}
	return 0;
}
static int isBreakingOption(const char* candidate){
	for (int i=0;i<breakingOptionsCount;i++){
		if(strcmp(candidate,breakingOptions[i])==0){
			return 1;
		}
	}
	return 0;
}
/* detects -O0 -O1 -O2 and eliminates it from argv
	sets zero optimization as default */
static int handleOptimization(int* refargc, char** argv){
	char flag=0;
	int j;
	int level=0;
	const int argc = *refargc;
	for(int i=1;i<argc;i++) {
		for(j=0;j<optimizationOptionsCount;j++) {
			if(strcmp(argv[i],optimizationOptions[j])==0) {
				level = j;
				flag = 1;
			} else if((strcmp(argv[i],"-O") == 0 && strlen(argv[i])>2)){
				int x = atoi(argv[i]+2);
				if(x > 0 && x < 3){
					level = x;
				} else {
					level = 0;
				}
				flag = 1;
			}
		}
		if(flag == 1) {
			// must shift all of them
			for(int k = i;k<argc; k++){
				if(k < argc-1) 
					argv[k] = argv[k+1];
			}
			*refargc= argc - 1;
			return level;
		}
		flag = 0;
	}
	return level;
}
static char* handleClangOptions(int argc,char** argv) {
	char* str = (char*)malloc(50*argc); //more than enough
	char flag = 0;
	for(int i=1;i<argc;i++) {
		for(int j=0;j<hacOptionsCount;j++) {
			if(strcmp(argv[i],hacOptions[j])==0) {
				flag = 0;
				break;
			}
			flag = 1;
		}
		if(flag == 1) {
			assert(strlen(argv[i])<50);
			strcat(str," ");
			strcat(str,argv[i]);
		}
		flag = 0;
	}
	return str;

}
static char* doubleNullTerminatedString(const char* string){
	int len = strlen(string);
	char* newStr = malloc(len+2);
	strcpy(newStr,string);
	newStr[len]= '\0';
	newStr[len+1]= '\0';
	return newStr;
}
static int guessBufferSize(char* str) {
	int buffSize = 1000+strlen(str)*10*(1+forceExpand*forceExpand);
	return buffSize;
}
char* compileHa2bf(const char* program, int level, int bufferSize){	
	char* buffer = calloc(sizeof(char),bufferSize);
	char* programToScan = doubleNullTerminatedString(program);
	yy_scan_string(programToScan);
	yyparse();			
	checkAndFixesTypesInTree();
	setOptimizationLevel(level);
	optimizeTree();
	FILE* f = fmemopen(buffer, bufferSize, "wb");
	setCodeOutput(f);
	codeTree();
	fclose(f);
	char* old = buffer;
	if(forceExpand){
		buffer = expandedString(old,forceExpand == 1 ? 0 : 2);
		free(old);
		return buffer;
	}
	return buffer;
}
char* compileHa2bfAutoBuffer(const char* program, int level){
	return compileHa2bf(program,level,guessBufferSize(program));
}
char* compileHa2LLVM(const char* program, int level, int bufferSize){
	char* buffer = calloc(sizeof(char),bufferSize);
	char* programToScan = doubleNullTerminatedString(program);
	yy_scan_string(programToScan);
	yyparse();			
	checkAndFixesTypesInTree();
	setOptimizationLevel(level);
	optimizeTree();
	FILE* f = fmemopen(buffer, bufferSize, "wb");
	setCodeOutputLLVM(f);
	codeTreeLLVM();
	fclose(f);
	return buffer;
}
char* compileHa2LLVMAutoBuffer(const char* program, int level){
	return compileHa2LLVM(program,level,guessBufferSize(program));
}
void compileHa2File(const char* input, const char* output, int level){
	yyin = fopen(input,"r");
	yyparse();			
	checkAndFixesTypesInTree();
	setOptimizationLevel(level);
	optimizeTree();
	FILE* f = fopen(output,"w");
	setCodeOutput(f);
	codeTree();
	fclose(f);
}
void compileHa2FileLLVM(const char* input, const char* output, int level){
	yyin = fopen(input,"r");
	yyparse();			
	checkAndFixesTypesInTree();
	setOptimizationLevel(level);
	optimizeTree();
	FILE* f = fopen(output,"w");
	setCodeOutput(f);
	codeTreeLLVM();
	fclose(f);
}

#ifndef HACLIB
int main (int argc, char** argv){
	char noTree = 1;
	char noChecks=0;
	char noCode = 0;
	char noBin = 0;
	char noDebug = 0;

	char* option = NULL;
	char* fileName;
	int level = handleOptimization(&argc,argv);
	if(argc >= 3)
	{	
		fileName = argv[2];
		option = argv[1];
		yyin = fopen(fileName,"r");
	}  
	else if(argc >= 2)
	{
		if(isOption(argv[1]))
		{
			option = argv[1];
		} 
		else if (isBreakingOption(argv[1])) 
		{ 
			if(strcmp("--help",argv[1])==0)
			{
				printf("Usage: %s [options] file.ha [optimization]\n",argv[0] );
				printf("Available options: \n");
				for(int i=0;i<hacOptionsCount;i++) {
					printf("\t%s\n",hacOptions[i]);
				}
				printf("Available optimizations: -O0, -O1, -O2. (Default is -O0)\n");
				printf("If no file is provided, it shall read from stdin\n");
				return 0;
			}
			else if(strcmp("--version",argv[1])==0)
			{
				printf("HAC (Headache Awesome Compiler). Version: %s\n",hacVersion );
				return 0;
			}
			else if(strcmp("--maga",argv[1])==0)
			{
				system("curl https://www.buffettworld.com/images/news_trump.jpg > trump.jpg");
				system("open trump.jpg");
				
				
			}
		} 
		else 
		{
			fileName = argv[1];
			option = NULL; 
			
			yyin = fopen(fileName,"r");
		}

	}
	if(yyin == NULL && argc > 1 && !option)
	{
		printf("ERROR: no such file \'%s\'\n", (argc > 2) ? argv[2] : argv[1]);
		return 1;
	} 
	if(argc == 1 || (argc == 2 && option) ) 
	{
		printf("HAC: Running interactive mode\n");
	}
	if(option) {
		if(strcmp("-check",option)==0)
		{
			noTree =1;
			noCode =1;
			noBin = 1;
			noDebug =1;
		}
		else if(strcmp("-syntax",option)==0)
		{
			noTree = 1;
			noChecks = 1;
			noCode = 1;
			noBin = 1;
			noDebug =1;
		}
		else if(strcmp("-lex",option)==0)
		{
			testLex();
			return 0;
		}
		else if(strcmp("-highlight",option)==0){
			highlight();
			return 0;
		}
		else if(strcmp("-tree",option)==0)
		{
			noTree = 0;
		}
		else if(strcmp("-noChecks",option)==0)
		{
			noChecks = 1;
			noCode =1;
			noBin = 1;
			noDebug =1;
		}
		else if(strcmp("-noCode",option)==0)
		{
			noCode =1;
			noBin =1;
			noDebug =1;
		}
		else if(strcmp("-noBin",option)==0)
		{
			noBin =1;
			noDebug =1;
		}
		else if(strcmp("-noDebug",option)==0)
		{
			noDebug =1;
		}
	}
	

	yyparse();
	printf("Syntax OK\n");

	if(!noChecks) {
		checkAndFixesTypesInTree();
		printf("Typing OK\n");
	}
	if(level > 0){
		setOptimizationLevel(level);
		optimizeTree();
	}
	if(!noTree)
	{
		printTree();
	}
	char * bf_name = "a.bf";
	char * bin_name = "a.ll";
	if(!noCode)
	{	FILE* bf_location = fopen(bf_name,"wt");
		setCodeOutput(bf_location);
		forceExpand = 0;
		codeTree();
		fclose(bf_location);
		//freeTree();
	}
	if(!noBin)
	{
		FILE* bin_location = fopen(bin_name,"wt");
		setCodeOutputLLVM(bin_location);
		codeTreeLLVM();
		fclose(bin_location);

		char* str = handleClangOptions(argc,argv);
		char* buff = (char*)malloc(
			strlen(str) +
			strlen("clang") +
			strlen(bin_name)+1);
		sprintf(buff,"clang %s %s",
			str,
			bin_name);
		int s = system(buff);
		if(s == 0)
			printf("\nLLVM binary generated\n"); 
		else 
			printf("something went wrong! clang returned %d\n", s);
	}
	if(forceExpand){
		printf("should forceExpand %d\n",forceExpand );
		for(int i=0;i<forceExpand;i++){
			char* bfprogram = readFile(bf_name);
			FILE* bf_location = fopen(bf_name,"wt");
			runStr(bf_location,bfprogram,0);
			fclose(bf_location);
			// system("expander a.bf > exa.bf");
			// system("mv exa.bf a.bf");
		}
	}
	if(!noDebug)
	{
		printf("Debugging\n");
		char* program = readFile(bf_name);
		printf("exec\n");
		int used = execute(program,30000,1);
		free(program);
		printf("printing memory\n");
		printAllWrittenMemory();
		printf("used %d cells (bytes) to run\n", used);
	}

	return 0;
}
#endif
