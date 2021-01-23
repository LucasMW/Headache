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
#include <string.h>

typedef enum terminalColors { black, red, green, orange, blue, magenta, cyan, lightgray, fallback } terminalColors; 
	

void setColor( terminalColors color) {
	switch (color) {
		case black:
			printf("\033[34m");
		break;
		case red:
			printf("\033[31m");
		break;
		case green:
			printf("\033[32m");
		break;
		case orange:
			printf("\033[33m");
		break;
		case blue:
			printf("\033[34m");
		break;
		case magenta:
			printf("\033[35m");
		break;
		case cyan:
			printf("\033[36m");
		break;
		case lightgray:
			printf("\033[37m");
		break;
		case fallback:
			printf("\033[39m");
		break;
		default:
			printf("\033[39m");
		break;
	}
}
void highlight ()
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
				setColor(fallback);
			break;
			case TK_GE:
				setColor(fallback);
			break;
			case '<':
				setColor(fallback);
			break;
			case TK_LE:
				setColor(fallback);
			break;
			case '@':
				setColor(magenta);
			break;
			case '%':
				setColor(magenta);
			break;
			case '#':
				setColor(magenta);
			break;
			case '=':
				setColor(fallback);
			break;
			case ';':
				setColor(fallback);
			break;
			case ',':
				setColor(fallback);
			break;
			case '+':
				setColor(fallback);
			break;
			case TK_INC:
				setColor(fallback);
			break;
			case '-':
				setColor(fallback);
			break;
			case TK_DEC:
				setColor(fallback);
			break;
			case '*':
				setColor(fallback);
			break;
			case '/':
				setColor(fallback);
			break;
			case '!':
				setColor(fallback);
			break;
			case '{':
				setColor(fallback);
			break;
			case '}':
				setColor(fallback);
			break;
			case '[':
				setColor(fallback);
			break;
			case ']':
				setColor(fallback);
			break;
			case '(':
				setColor(fallback);
			break;
			case ')':
				setColor(fallback);
			break;
			case TK_EQEQ:
				setColor(fallback);
				printf("==");
			break;
			case TK_OR:
				setColor(fallback);
			break;
			case TK_AND:
				setColor(fallback);
			break;
			case TK_INT:
				setColor(orange);
			break;
			case TK_WINT:
				setColor(blue);
			break;
			case TK_WSHORT:
				setColor(blue);
			break;
			case TK_WBYTE:
				setColor(blue);
			break;
			case TK_WCHAR:
				setColor(blue);
			break;
			case TK_WFLOAT:
				setColor(blue);
			break;
			case TK_WIF:
				setColor(magenta);
			break;
			case TK_WELSE:
				setColor(magenta);
			break;
			case TK_WNEW:
				setColor(magenta);
			break;
			case TK_WRETURN:
				setColor(magenta);
			break;
			case TK_WVOID:
				setColor(blue);
				printf("void");
			break;
			case TK_WWHILE:
				setColor(magenta);
				printf("while\n");
			break;
			case TK_WFOR:
				setColor(magenta);
				printf("for");
			break;
			case TK_WAS:
				setColor(magenta);
				printf("as");
			break;
			case TK_STR:
				setColor(orange);
				printf("\"%s\"",yylval.str_val);
			break;
			case TK_VAR:
				setColor(lightgray);
				printf("%s",yylval.str_val);
			break;
			default: 
			printf("%c", control);
			break;

		}
		printf("%c", control);
	}
	printf("FINISHED READING FILE\n");
	printf("Proccessed %d tokens in %d lines\n",tokens,yy_lines);
}