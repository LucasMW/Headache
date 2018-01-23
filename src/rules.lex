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
"as"	{return TK_WAS;}
"bit"	{return TK_WBIT;}
"byte"	{return TK_WBYTE;}
"char"	{return TK_WCHAR;}
"else"	{return TK_WELSE; }

"if"	{return TK_WIF;}
"int"	{return TK_WINT;}
"new"	{return TK_WNEW;}
"return" {return TK_WRETURN;}
"short" {return TK_WSHORT;}
"void"	{return TK_WVOID;}
"while"	{return TK_WWHILE;}
"for"	{return TK_WFOR;}
">="	{return TK_GE;}
">"		{return '>';}
"<=" 	{return TK_LE;}
"<"		{return '<';}
";"		{return ';';}
"=="	{return TK_EQEQ;}
"="		{return '=';}
"{"		{return '{';}
"}"		{return '}';}
"["		{return '[';}
"]"		{return ']';}
"("		{return '(';}
")"		{return ')';}
"!"		{return '!';}
"&&"	{return TK_AND;}
"||"	{return TK_OR;}
"+"		{return '+';}
"++"	{return TK_INC;}
"+=" 	{return TK_PLUSEQ;}
"-="	{return TK_MINUSEQ;}
"--"	{return TK_DEC;}
"-"		{return '-';}
"*"		{return '*';}
"/"		{return '/';}
","		{return ',';}
"@"		{return '@';}
"print"		{return '@';}
"#"		{return '#';}
"read"		{return '#';}
"%"		{return '%';}

"0x"([0-9]|[A-F]|[a-f])+	{yylval.int_val = strtol(yytext, NULL, 0);
				return TK_INT;}
[0-9]+	{ yylval.int_val = atoi( yytext );
		return TK_INT;}


"true" { yylval.int_val = 1;
				return TK_INT;}
"false" { yylval.int_val = 0;
				return TK_INT;}

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

"//".*                                    { /* DO NOTHING */ }


\"(([\\][\"])|([^\"\n])+)*\" { yylval.str_val = translatescape(); //gives scape processed string
		 return TK_STR;}

\'.\' 	{yylval.int_val = *(yytext+1); //no multi-char character constants
		return TK_INT;}
\'"\\n"\' { yylval.int_val = '\n';
		return TK_INT;}
\'"\\t"\' { yylval.int_val = '\t';
		return TK_INT;}
\'"\\"\"\' { yylval.int_val = '\"';
		return TK_INT;}

\'.[^\']	{
		 lexError("Unfinished Character",2);}

\"[^"\""]*[\n]?	{ lexError("Unfinished String",3); 
			}	
\\\\*\n {	printf("comment");
			yy_lines++;}

[\n] 	{  	//count lines;
			yy_lines++; }
[ ]|[\t] {//ignore spaces and tabs
	 }

. { return yytext[0];}



