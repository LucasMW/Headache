#include <stdio.h>
#include <stdlib.h>
#if !defined(lex_h)
	#include "lex.h"
	#define lex_h
#endif
#include <string.h>

Seminfo_t seminfo;
int yy_lines=1; //save one for EOF

void lexError(const char* message, int ret)
{
	printf("Lexical error detected in source:\n");
	fprintf(stderr,"ERROR: %s\t\t#line %d\n", message, yy_lines);
	exit( ret ? ret : 1);
}

int main (int argc, char** argv)
{
	int tokens=-1;
	int control=1; //Grants it enters the while
	while(control)
	{
		control = yylex();
		tokens++; //tokens shall only be incremented when yylex() returns
		switch(control)
		{
			case '>':
				printf("Greater\n");
			break;
			case TK_GE:
				printf("Greater or Equal\n");
			break;
			case '<':
				printf("Lesser\n");
			break;
			case TK_LE:
				printf("Lesser or Equal\n");
			break;
			case '@':
				printf("At\n");
			break;
			case '=':
				printf("Equal\n");
			break;
			case ';':
				printf("Semicolon\n");
			break;
			case ',':
				printf("Comma\n");
			break;
			case '+':
				printf("Plus\n");
			break;
			case '-':
				printf("Minus\n");
			break;
			case '*':
				printf("Asterisk\n");
			break;
			case '/':
				printf("Slash\n");
			break;
			case '!':
				printf("Exclamation\n");
			break;
			case '{':
				printf("Braces Open\n");
			break;
			case '}':
				printf("Braces Close\n");
			break;
			case '[':
				printf("Brackets Open\n");
			break;
			case ']':
				printf("Brackets Close\n");
			break;
			case '(':
				printf("Parenthesis Open\n");
			break;
			case ')':
				printf("Parenthesis Close\n");
			break;
			case TK_EQEQ:
				printf("Equal-Equal\n");
			break;
			case TK_OR:
				printf("OR\n");
			break;
			case TK_AND:
				printf("AND\n");
			break;
			case TK_INT:
				printf("Int %d\n",seminfo.i);
			break;
			case TK_FLOAT:
				printf("Float %g\n",seminfo.d);
			break;
			case TK_WCHAR:
				printf("reserved word char\n");
			break;
			case TK_WELSE:
				printf("reserved word else\n");
			break;
			case TK_WFLOAT:
				printf("reserved word float\n");
			break;
			case TK_WIF:
				printf("reserved word if\n");
			break;
			case TK_WINT:
				printf("reserved word int\n");
			break;
			case TK_WNEW:
				printf("reserved word new\n");
			break;
			case TK_WRETURN:
				printf("reserved word return\n");
			break;
			case TK_WVOID:
				printf("reserved word void\n");
			break;
			case TK_WWHILE:
				printf("reserved word while\n");
			break;
			case TK_STR:
				printf("String: %s\n",seminfo.s);
			break;
			case TK_VAR:
				printf("Var %s\n",seminfo.s);
			break;

		}
	}
	printf("FINISHED READING FILE\n");
	printf("Proccessed %d tokens in %d lines\n",tokens,yy_lines);
}

