
#include <stdio.h>

#if !defined(codeEss_h)
	#include "codeEss.h"
	#define codeEss_h
#endif

extern FILE* output;

void codeZero(int x) {
	codeGoTo(x); codeStr("[-]");
}

void addXbyY(int x,int y){
	int temp0 = x-1;
	fprintf(output, "%d[-]%d[%d+%d+%d-]%d[%d+%d-]\n",temp0,y,x,temp0,y,temp0,y,temp0);
}
/*	temp0[-]
		y[x+temp0+y-]
		temp0[y+temp0-]	*/
void incrementXbyY(int x,int y) {
	printf("add %d to %d\n", x,y);
	int temp0 = x-1;
	codeGoTo(temp0); codeStr("[-]");
	codeGoTo(y); codeStr("["); codeGoTo(x); codeStr("+"); codeGoTo(temp0); codeStr("+"); codeGoTo(y); codeStr("-]");
	codeGoTo(temp0); codeStr("[");  codeGoTo(y); codeStr("+"); codeGoTo(temp0); codeStr("-]");
}

/*
temp0[-]
y[x-temp0+y-]
temp0[y+temp0-]
*/
void decrementXbyY(int x,int y) {
	printf("sub %d to %d\n", x,y);
	int temp0 = x-1;
	codeGoTo(temp0); codeStr("[-]");
	codeGoTo(y); codeStr("["); codeGoTo(x); codeStr("-"); codeGoTo(temp0); codeStr("+"); codeGoTo(y); codeStr("-]");
	codeGoTo(temp0); codeStr("[");  codeGoTo(y); codeStr("+"); codeGoTo(temp0); codeStr("-]");
}
	/*	temp0[-]
	x[-]
	y[x+temp0+y-]
	temp0[y+temp0-]	*/
void setXtoY(int x, int y) {
	printf("set %d to %d\n",x,y );
	int temp0 = x-1;
	printf("temp %d\n",temp0 );
	codeGoTo(temp0); codeStr("[-]");
	codeGoTo(x); codeStr("[-]");
	codeGoTo(y); codeStr("["); codeGoTo(x); codeStr("+"); codeGoTo(temp0); codeStr("+"); codeGoTo(y); codeStr("-]");
	codeGoTo(temp0); codeStr("["); codeGoTo(y); codeStr("+"); codeGoTo(temp0); codeStr("-]");
}
	/* temp0[-]
	temp1[-]
	x[temp1+x-]
	temp1[
	 y[x+temp0+y-]temp0[y+temp0-]
	temp1-] */
void multiplyXbyY(int x, int y){
	int temp0 = x-1;
	int temp1 = y-1;
	codeZero(temp0);
	codeZero(temp1);
	codeGoTo(x); codeStr("["); codeGoTo(temp1); codeStr("+"); codeGoTo(x); codeStr("-]");
	codeGoTo(temp1); codeStr("[");
}
/*
temp0[-]
temp1[-]
temp2[-]
temp3[-]
x[temp0+x-]
temp0[
 y[temp1+temp2+y-]
 temp2[y+temp2-]
 temp1[
  temp2+
  temp0-[temp2[-]temp3+temp0-]
  temp3[temp0+temp3-]
  temp2[
   temp1-
   [x-temp1[-]]+
  temp2-]
 temp1-]
 x+
temp0]
*/
void divideXbyY(int x,int y){
	int temp0;
	int temp1;
	int temp2;
	int temp3;
	int order[] = {temp0,temp1,temp2,temp3,x,temp0,x,
		temp0,y,temp1,temp2,y,temp2,y,temp2,temp1,temp2,
		temp0,temp2,temp3,temp0};
	//fprintf(output, "%s[-]%s[-]%s[-]%s[-]x[%s+x-]%s[y[%s+%s+y-]%s[y+%s-]%s[%s+%s-[%s[-]%s+%s-]%s[%s+%s-]%s[%s-[x-%s[-]]+%s-]%s-]x+%s]\n", );
}
/*temp0[-]
x[temp0+x-]
y[x+y-]
temp0[y+temp0-] */
void swapXbyY(int x, int y){
	int temp0=x-1;
	codeGoTo(temp0);codeStr("[-]");
	codeGoTo(x);codeStr("[");codeGoTo(temp0);codeStr("+");codeGoTo(x);codeStr("-]");
	codeGoTo(y);codeStr("[");codeGoTo(x);codeStr("+");codeGoTo(y);codeStr("-]");
	codeGoTo(temp0); codeStr("["); codeGoTo(y); codeStr("+"); codeGoTo(temp0); codeStr("-]");
}