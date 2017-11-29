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

void run(char* path){
	FILE* input = fopen(path, "rt");
	char c;
	if(!input){
		input = stdin;
	}
	while(fscanf(input,"%c",&c) != EOF){
			printf("%s", expand8to16NoCopy(c));
	}
}
int main (int argc, char** argv){
	//expects ./expander filepath 
	//expects ./expander -p "bfprogram" 
	if (argc == 3){
		if(strcmp("-p",argv[1])==0) {
			for(char* p = argv[2];*p;p++){
				printf("%s\n", expand8to16NoCopy(*p));
			}
		} else if(strcmp("-i",argv[1])==0) {
			run(NULL);
		} else{
			cliError(argv[0]);
		}
	}
	else if(argc == 2){
		run(argv[1]);
	}
	else{
		cliError(argv[0]);
	}
}
#endif