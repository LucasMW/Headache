
#include "compilerFunctions.h"
#include "string.h"
#if !defined(codeEss_h)
	#include "codeEss.h"
	#define tree_h
#endif

int checkCompilerFunctions(const char* str){
	for(int i=0; i < compilerFunctionsCount; i++){
		if(strcmp(compilerFunctions[i],str)==0){
			hac_print_memory();
		}
	}
}
void hac_print_memory(){
	codeStr("@");
}