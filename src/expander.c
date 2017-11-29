//From https://esolangs.org/wiki/Brainfuck_bitwidth_conversions

#include <stdlib.h>

char* other(char b){
	char* s;
	switch(b){
		case '.':
			return ".";
		break;
		case ',':
			return ",";
		break;
		default:
			s = (char*)malloc(2);
			s[0]=b;
			s[1]='\0';
			return s;
		break;
	}
}
char * expand8to16NoCopy(char b){
	switch(b){
		case '>':
			return ">>>>";
		break;
		case '<':
			return "<<<<";
		break;
		case '+':
			return ">+<+[>-]>[->>+<]<<";
		break;
		case '-':
			return ">+<[>-]>[->>-<]<<-";
		break;
		case '[':
			return ">+<[>-]>[->+>[<-]<[<]>[-<+>]]<-[+<";
		break;
		case ']':
			return ">+<[>-]>[->+>[<-]<[<]>[-<+>]]<-]<";
		break;
	}
	return other(b);
}
char * expand8to16(char b){
	switch(b){
		case '>':
			return ">>>";
		break;
		case '<':
			return "<<<";
		break;
		case '+':
			return "+>+[<->[->>+<<]]>>[-<<+>>]<<<[->>+<<]";
		break;
		case '-':
			return "+>[<->[->>+<<]]>>[-<<+>>]<<<[->>-<<]>-<";
		break;
		case '[':
			return ">[<+>[->>+<<]]>>[-<<+>>]<[<<+>>[->+<]]>[-<+>]<<<[[-]";
		break;
		case ']':
			return ">[<+>[->>+<<]]>>[-<<+>>]<[<<+>>[->+<]]>[-<+>]<<<]";
		break;
	}
	return other(b);;
}
char * expand8to32(char b){
	switch(b){
		case '>':
			return ">>>>>";
		break;
		case '<':
			return "<<<<<";
		break;
		case '+':
			return "+>+[<->[->>>>+<<<<]]>>>>[-<<<<+>>>>]<<<<<[>>+[<<->>[->>>+<<<]]>>>[-<<<+>>>]<<<<<[>>>+[<<<->>>[->>+<<]]>>[-<<+>>]<<<<<[->>>>+<<<<]]]";
		break;
		case '-':
			return "+>[<->[->>>>+<<<<]]>>>>[-<<<<+>>>>]<<<<<[>>[<<->>[->>>+<<<]]>>>[-<<<+>>>]<<<<<[>>>[<<<->>>[->>+<<]]>>[-<<+>>]<<<<<[->>>>-<<<<]>>>-<<<]>>-<<]>-<";
		break;
		case '[':
			return ">[<+>[->>>>+<<<<]]>>>>[-<<<<+>>>>]<<<[<<+>>[->>>+<<<]]>>>[-<<<+>>>]<<[<<<+>>>[->>+<<]]>>[-<<+>>]<[<<<<+>>>>[->+<]]>[-<+>]<<<<<[[-]";
		break;
		case ']':
			return ">[<+>[->>>>+<<<<]]>>>>[-<<<<+>>>>]<<<[<<+>>[->>>+<<<]]>>>[-<<<+>>>]<<[<<<+>>>[->>+<<]]>>[-<<+>>]<[<<<<+>>>>[->+<]]>[-<+>]<<<<<]";
		break;
	}
	return other(b);
}
#ifdef STANDALONE
#include <stdio.h>
#include <string.h>

void cliError(char* prog){
	printf("Error!\nProgram usage is: \n%s ./expander sourcecode\n",prog);
	exit(-1);
}
char* expand(char c, int mode){
	switch (mode){
		case 0:
			return expand8to16NoCopy(c);
		break;
		case 1:
			return expand8to16(c);
		break;
		case 2:
			return expand8to32(c);
		break;
		default:
			return expand8to16NoCopy(c);
		break;
	}
}
void run(char* path,int mode){
	FILE* input = fopen(path, "rt");
	char c;
	if(!input){
		input = stdin;
	}
	while(fscanf(input,"%c",&c) != EOF){
			printf("%s", expand(c,mode));
	}
}
void runStr(char* str, int mode){
	for(char* p = str;*p;p++) {
		printf("%s\n", expand(*p,mode));
	}
}

int main (int argc, char** argv){
	//expects ./expander filepath [1]
	//expects ./expander -p "bfprogram" [1]
	char * program;
	int mode=0;
	char* option; 
	if(argc == 4){
		option = argv[1];
		program = argv[2];
		mode = atoi(argv[3]);
		runStr(program,mode);
	}
	if (argc == 3){
		if(strcmp("-p",argv[1])==0) {
			runStr(argv[2],0);
		} else if(strcmp("-i",argv[1])==0) {
			run(NULL,mode);
		} else{
			run(argv[1],atoi(argv[2]));
		}
	}
	else if(argc == 2){
		run(argv[1],mode);
	}
	else{
		cliError(argv[0]);
	}
}
#endif