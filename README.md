# Headache
Programming Language that compiles to 8-Bit Brainfuck.

## Build

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

## Resources

wiki: https://github.com/LucasMW/Headache/wiki


## Support Headache

Help Headache launch on Homebrew. This repository needs 50 stargazers to become a brew package. 

### Donation Addresses:

Support Headache with cryptocurrency:

BTC: 1NcdcAjkKj9ywiCfinwFi1W3QfsvFSFsAH

Doge: D5wEBGcwLeBSQvwnBckco1EvQQcuXCBvRj

LTC: Ldj6ax1CzdYFRbSiZjpCvcPoxQ2BvX42Kb

ZEC: t1PsMPGuoZfD5YwAgmyd3j2cb8siysxR9fE

Dash: Xt7qYtLiLx6mHR1fUeQVYiHgEbX7YNugM2

BCH: 1NCYSnFb44ybHtzwG51cUYWLa67PZdsREG

XMR: 4CTKuktQ8eQFriMu3xDALEawuu9iWQhFYffgffqgC3RHCWgcLGTG9oyNWgvLiAoDPpYUvsJmRkTAudGSXNqnRFsuT9LNU3Up81F2x3fzkj





