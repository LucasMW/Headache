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

void testLex ()
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
			case '%':
				printf("Percent\n");
			break;
			case '#':
				printf("Sharp\n");
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
			case TK_INC:
				printf("Increment\n");
			break;
			case '-':
				printf("Minus\n");
			break;
			case TK_DEC:
				printf("Decrement\n");
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
				printf("Int %d\n",yylval.int_val);
			break;
			case TK_WINT:
				printf("reserved word int\n");
			break;
			case TK_WSHORT:
				printf("reserved word short\n");
			break;
			case TK_WBYTE:
				printf("reserved word byte\n");
			break;
			case TK_WCHAR:
				printf("reserved word char\n");
			break;
			case TK_WFLOAT:
				printf("reserved word float\n");
			break;
			case TK_WIF:
				printf("reserved word if\n");
			break;
			case TK_WELSE:
				printf("reserved word else\n");
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
			case TK_WFOR:
				printf("reserved word for\n");
			break;
			case TK_WAS:
				printf("reserved word as\n");
			break;
			case TK_STR:
				printf("String: %s\n",yylval.str_val);
			break;
			case TK_VAR:
				printf("Var %s\n",yylval.str_val);
			break;

		}
	}
	printf("FINISHED READING FILE\n");
	printf("Proccessed %d tokens in %d lines\n",tokens,yy_lines);
}