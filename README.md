# Headache
Programming Language that compiles to 8-Bit Brainfuck.

## NOT OFFICIALLY SUPPORTED BRANCH


## Build

Requires MinGW to build

Just type 'make'.

## Install

Just type 'make install'

then include 
		
	$HOME/.Headache/ 

in your PATH variable.

this will enable hac, bfi and expander in your shell 

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

## Tests

Type 'make test' to run the default tests

Other options:

make testlexical
	
	Runs only the lexical analyser tests

make testsyntax
	
	Runs only the syntax analyser tests

make testchecks
	
	Runs only the typing and error checking related tests

make testrunnable
	
	Runs only the tests which tests if programs are compiled and run succesfully.




