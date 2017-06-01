%option noinput
%option nounput
%option noyywrap
%{
   #if !defined(lex_h)
	#include "lex.h"
	#define lex_h
   #endif
   #if !defined(tree_h)
	#include "tree.h"
	#define tree_h
   #endif
   #if !defined(grammar_h)
	#include "grammar.h"
	#define grammar_h
   #endif
   

	static char* makeNewStr()
	{
		char* newStr = (char*)malloc(yyleng+1);
		strcpy(newStr,yytext);
		newStr[yyleng] = '\0';
		return newStr;
	}
	// transforms scapes of yytext to ascii characters
	static char * translatescape()
	{
		char flag = 0;
		char* newStr = (char*)malloc(yyleng);
		int j = 0;
		for(int i = 1; i < yyleng-1; i++) //limits to discard quotes  
		{
			if(flag == 1) //means last character was '\\'
			{
				switch(yytext[i])
				{
					case 'n':
					newStr[j++]='\n';
					break;
					case 't':
					newStr[j++]='\t';
					break;
					case '\"':
					newStr[j++]='\"';
					break;
					case '\\': //convenience
					newStr[j++]='\\';
					break;
				}
				flag = 0; 
			}
			else
			{
				if(yytext[i] == '\\' )
					flag = 1;
				else
					newStr[j++] = yytext[i];
			}
		}
		newStr[j]= '\0';
		return newStr;
	}
%}

%x IN_COMMENT 
%%


">"		{ printf("%c", '>');}
"<"		{ printf("%c", '<');}
"["		{ printf("%c", '[');}
"]"		{ printf("%c", ']');}
"+"		{ printf("%c", '+');}
"-"		{ printf("%c", '-');}
","		{ printf("%c", ',');}
"."		{ printf("%c", '.');}

([a-z]|[A-Z])([a-z]|[A-Z]|[0-9])* { yylval.str_val = makeNewStr();
									return TK_VAR;}

<INITIAL>{ //state machine based on the example of flex manual: http://flex.str_valourceforge.net/manual/How-can-I-match-C_002dstyle-comments_003f.html
     "/*"      BEGIN(IN_COMMENT); 
     }
     <IN_COMMENT>{
     "*/"      BEGIN(INITIAL); // finished comment; Return to the original state
     [^*\n]+   // eat comment in chunks
     "*"       // eat the lone star
     \n        yy_lines++; //comments can be mult-line
     <<EOF>>	   lexError("Unfinished Comment",1); //reached EOF 
     }

\\\\*\n {
			yy_lines++;}

[\n] 	{  	//count lines;
			yy_lines++; }
[ ]|[\t] {//ignore spaces and tabs
	 }
. { return yytext[0];}



