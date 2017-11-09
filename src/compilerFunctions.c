
#include "compilerFunctions.h"
#include "string.h"
#if !defined(codeEss_h)
	#include "codeEss.h"
	#define tree_h
#endif

int checkCompilerFunctions(const char* str){
	for(int i=0; i < compilerFunctionsCount; i++){
		if(strcmp(compilerFunctions[i],str)==0){
			return i;
		}
	}
	return -1;
}
void codeCompilerFunctions(const char* str){
	int index = checkCompilerFunctions(str);
	switch(index){
		case 0:
			hacPrintMemory();
		break;
		default:
		break;
	}
}
void hacPrintMemory(){
	codeStr("@");
}