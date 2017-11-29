#include <stdio.h>

/* Suggestion of software aproach to handle shorts */

short makeShort(char a,char b){
	short s;
	s = a*256+b; 
	return s;
}
void makeSS(short s, char* a, char* b){
	*b = (char)a;
	*a = (char)(s/256);
}



short add(char a, char b, char c, char d){
	char t;
	if(a >= 0 && c >= 0) {
		t = b+d;
		printf("t %d\n", t);
		//overflow
		if(t<b){
			a = a+1;
		}
		b = t; 
		t = a + c;
		if(t < a && t < c){

		}
	}
	return makeShort(a,b);
}
short sub(char a, char b, char c, char d){
	char t;
	if(a >= 0 && c >= 0) {
		t = b+d;
		printf("t %d\n", t);
		//overflow
		if(t<b){
			a = a+1;
		}
		b = t; 
		t = a + c;
		if(t < a && t < c){

		}
	}
	return makeShort(a,b);
}
short mult(char a, char b, char c, char d){
	char t;
	if(a >= 0 && c >= 0) {
		t = b+d;
		printf("t %d\n", t);
		//overflow
		if(t<b){
			a = a+1;
		}
		b = t; 
		t = a + c;
		if(t < a && t < c){

		}
	}
	return makeShort(a,b);
}
short div(char a, char b, char c, char d){
	char t;
	if(a >= 0 && c >= 0) {
		t = b+d;
		printf("t %d\n", t);
		//overflow
		if(t<b){
			a = a+1;
		}
		b = t; 
		t = a + c;
		if(t < a && t < c){

		}
	}
	return makeShort(a,b);
}
char equals(char a, char b, char c, char d){
	char t;
	if(a >= 0 && c >= 0) {
		t = b+d;
		printf("t %d\n", t);
		//overflow
		if(t<b){
			a = a+1;
		}
		b = t; 
		t = a + c;
		if(t < a && t < c){

		}
	}
	return makeShort(a,b);
}




int main(void){
	short x = 10*256+1;
	short y = add(10,0,0,1);
	printf("%d %d\n",x,y);
	return 0;
}