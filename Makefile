CFLAGS = -Wall -std=c99
OUTFILE = comp
#always compiles when using just make
test/comp: src/main.c src/lex.c src/grammar.c
	cc $(CFLAGS) -o hac src/main.c src/lex.c src/grammar.c src/tree.c src/lextest.c src/symbolTable.c src/codeGen.c src/testbfi.c

bin/comp.js:
	emcc --pre-js stdin.js -Wall -o bin/comp.js src/main.c src/lex.c src/grammar.c src/tree.c src/lextest.c src/symbolTable.c src/codeGen.c
bin/comp.html:
	emcc -Wall  -o bin/comp.html src/main.c src/lex.c src/grammar.c src/tree.c src/lextest.c src/symbolTable.c src/codeGen.c

test: testlexical testsyntax testchecks testbin

bfi: src/testbfi.c
	cc $(CFLAGS) -DSTANDALONE src/testbfi.c -o bfi

testrunnable: hac bfi
	cat test/runnable/example.ha | ./hac
	./bfi a.bf
	cat test/runnable/add.ha | ./hac
	./bfi a.bf

testbin: comp
	cat test/test1.monga |./comp -noTree
	cat test/test2.monga |./comp -noTree
	cat test/test3.monga |./comp -noTree
	cat test/test4.monga |./comp -noTree
	cat test/test5.monga |./comp -noTree
	cat test/test6.monga |./comp -noTree -c  
	cat test/test7.monga |./comp -noTree
	cat test/test8.monga |./comp -noTree 
	cat test/test9.monga |./comp -noTree
	cat test/examples/program1.monga |./comp -noTree

testchecks: comp
	sh test/checks/script.sh
testsyntax: comp
	sh test/syntax/script.sh
testlexical: comp
	sh test/lexical/script.sh

testleaks: comp
	@rm -f val.out prog.out
	cat test/leaks/leak1.monga | valgrind --track-origins=yes ./comp > prog.out 2> val.out
	cat val.out | grep error
	tail val.out
	rm -f val.out prog.out
src/grammar.c: src/grammar.y
	bison -d src/grammar.y
	mv grammar.tab.c src/grammar.c
	mv grammar.tab.h src/grammar.h
src/lex.c: src/rules.lex src/grammar.c
	flex src/rules.lex
	mv lex.yy.c src/lex.c

clean: 
	rm -f src/lex.c
	rm -f src/grammar.c
	rm -r bin
	mkdir bin
	rm -r temp
	mkdir temp
	rm -rf *.dSYM
	rm -f test/*/*.output
	rm -f va.txt
	rm -f val.out
	rm -f grammar.output
	rm -f *.out
	rm -f *.ll
	rm -f test/*.out
	rm -f test/*.ll
	rm -rf *.o
	rm -f comp

#always generate zip
zip:
	rm -rf zipfolder
	zip -r zipfolder.zip src test README.txt Makefile
	mv zipfolder.zip ../mongaComp.zip

bin/comp: temp/codeGen.o temp/symbolTable.o temp/grammar.o temp/tree.o temp/main.o temp/lex.o temp/lextest.o
	ls temp
	cc -o bin/comp temp/*.o -O3 

temp/lextest.o: src/lextest.c
	cc -o temp/lextest.o -Wall -O3 -c src/lextest.c
temp/codeGen.o: src/codeGen.c
	cc -o temp/codeGen.o -Wall -O3 -c src/codeGen.c
temp/symbolTable.o: src/symbolTable.c
	cc -o temp/symbolTable.o -Wall -O3 -c src/symbolTable.c
temp/grammar.o: src/grammar.c
	cc -o temp/grammar.o -Wall -O3 -c src/grammar.c
temp/tree.o: src/tree.c
	cc -o temp/tree.o -Wall -O3 -c src/tree.c
temp/main.o:  src/main.c
	cc -o temp/main.o -Wall -O3 -c src/main.c
temp/lex.o: src/lex.c
	cc -o temp/lex.o -Wall -O3 -c src/lex.c