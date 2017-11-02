# Headache
Programming Language that compiles to 8-Bit Brainfuck.

## Build

Just use type 'make'.

## Usage

./hac file.ha

it generates a brainfuck file named "a.bf" and then executes it.

Other options:

./hac -lex
	Runs lexical analyser

./hac -syntax
	Runs syntax analyser

./hac -tree
	Displays AST

./hac -check
	Checks for errors and warnings whithout compiling
