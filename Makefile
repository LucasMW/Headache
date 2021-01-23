CFLAGS = -Wall -std=c99 -g -O3
OUTFILE = hac
SOURCES = src/main.c src/lex.c src/grammar.c src/tree.c src/lextest.c src/symbolTable.c src/codeGen.c src/testbfi.c src/compilerFunctions.c src/codeEss.c src/optimizer.c src/expander.c src/highlight.c
OBJS = temp/codeGen.o temp/symbolTable.o temp/grammar.o temp/tree.o temp/main.o temp/lex.o temp/lextest.o temp/testbfi.o temp/compilerFunctions.o temp/codeEss.o temp/optimizer.o temp/expander.o temp/highlight.o
#always compiles when using just make
test/hac: src/main.c src/lex.c src/grammar.c
	cc $(CFLAGS) -o hac $(SOURCES)
bin/hac.exe: $(SOURCES)
	i686-w64-mingw32-gcc $(CFLAGS) -o bin/hac.exe $(SOURCES)
bin/hac.linux: $(SOURCES)
	x86_64-linux-musl-cc -static $(CFLAGS) -o bin/hac.linux $(SOURCES)
bin/hac.js: src/main.c src/lex.c src/grammar.c
	emcc -Wall -o bin/hac.js $(SOURCES)
bin/hac.html: src/main.c src/lex.c src/grammar.c
	emcc -Wall -o bin/hac.html $(SOURCES)

all: test/hac bfi expander bin/hac bfalgoConverter

install: bin/hac expander bfi bfalgoConverter
	@rm -rf $(HOME)/.Headache/
	@echo "Installing Headache..."
	@./bin/hac --version
	@mkdir "$(HOME)/.Headache"
	@cp bin/hac "$(HOME)/.Headache/"
	@cp bfi "$(HOME)/.Headache/"
	@cp expander "$(HOME)/.Headache/"
	@cp bfalgoConverter "$(HOME)/.Headache/"
	@echo "Please add $(HOME)/.Headache/ to your PATH"

uninstall: 
	@rm -rf $(HOME)/.Headache/


test: testlexical testsyntax testtree testchecks

bfide-bfi: src/bfide/main.swift src/bfide/BrainfuckInterpreter.swift
	swiftc -o bfide-bfi src/bfide/*.swift

bfi: src/testbfi.c
	cc $(CFLAGS) -DSTANDALONE src/testbfi.c -o bfi

expander: src/expander.c
	cc $(CFLAGS) -DSTANDALONE src/expander.c -o expander

bfalgoConverter: src/bfalgoConverter.c
	cc $(CFLAGS) src/bfalgoConverter.c -o bfalgoConverter

bfi.exe: src/testbfi.c
	i686-w64-mingw32-gcc $(CFLAGS) -DSTANDALONE src/testbfi.c -o bfi.exe
bfi.linux: src/testbfi.c
	x86_64-linux-musl-cc $(CFLAGS) -DSTANDALONE src/testbfi.c -o bfi.linux
bfi.js: src/testbfi.c
	emcc $(CFLAGS) -DSTANDALONE src/testbfi.c -s FORCE_FILESYSTEM=1 -lnodefs.js -o bfi.js

expander.exe: src/expander.c
	i686-w64-mingw32-gcc $(CFLAGS) -DSTANDALONE src/expander.c -o expander.exe
expander.linux: src/expander.c
	x86_64-linux-musl-cc -static $(CFLAGS) -DSTANDALONE src/expander.c -o expander.linux

bfalgoConverter.exe: src/bfalgoConverter.c
	i686-w64-mingw32-gcc -static $(CFLAGS) src/bfalgoConverter.c -o bfalgoConverter.exe

bfalgoConverter.linux: src/bfalgoConverter.c
	x86_64-linux-musl-cc -static $(CFLAGS) src/bfalgoConverter.c -o bfalgoConverter.linux

windows.zip: bfi.exe expander.exe bfalgoConverter.exe bin/hac.exe
	rm -rf zipfolder.zip
	rm -f src/*.o
	mv bin/hac.exe hac.exe
	zip -r zipfolder.zip hac.exe expander.exe bfi.exe bfalgoConverter.exe
	mv zipfolder.zip hac-release-windows.zip

linux.zip: bfi.linux expander.linux bfalgoConverter.linux bin/hac.linux
	rm -rf zipfolder.zip
	rm -f src/*.o
	mv bin/hac.linux hac.linux
	zip -r zipfolder.zip hac.linux expander.linux bfi.linux bfalgoConverter.linux
	mv zipfolder.zip hac-release-linux.zip

testoptimize: hac bfi
	sh test/optimize/script.sh

testrunnable: hac bfi
	sh test/runnable/script.sh

testbf: bfi
	sh test/bf/script.sh

testbfide: bfide-bfi
	mv bfi bfi-x
	mv bfide-bfi bfi
	sh test/bf/script.sh
	mv bfi bfide-bfi
	mv bfi-x bfi

testbin: hac
	sh test/bin/script.sh

testtree: hac
	sh test/tree/script.sh
testchecks: hac
	sh test/checks/script.sh
testsyntax: hac
	sh test/syntax/script.sh
testlexical: hac
	sh test/lexical/script.sh

src/grammar.c: src/grammar.y
	bison -d src/grammar.y --output src/grammar.c
	
src/lex.c: src/rules.lex src/grammar.c
	flex src/rules.lex
	mv lex.yy.c src/lex.c

clean: 
	rm -f src/lex.c
	rm -f src/grammar.c
	rm -rf bin
	mkdir bin
	rm -rf temp
	mkdir temp
	rm -rf *.dSYM
	rm -f test/*/*.output
	rm -f va.txt
	rm -f val.out
	rm -f grammar.output
	rm -f *.out
	rm -f *.bf
	rm -f test/*.out
	rm -f test/*.bf
	rm -rf *.o
	rm -f hac
	rm -f bfide-bfi
	rm -f bfi
	rm -f expander
	rm -f bfalgoConverter
	rm -f *.exe
	rm -f *.linux

#always generate zip
zip:
	rm -rf zipfolder
	rm -f src/*.o
	zip -r zipfolder.zip src test README.txt Makefile
	mv zipfolder.zip ../hac.zip

zip-release: all
	rm -rf zipfolder
	rm -f src/*.o
	zip -r zipfolder.zip hac expander bfi bin/hac
	mv zipfolder.zip ../hac-release.zip

temp: 
	mkdir temp
bin: 
	mkdir bin

bin/hac: src/grammar.c src/lex.c temp bin $(OBJS)
	ls temp
	cc -o bin/hac temp/*.o -O3 

temp/optimizer.o:
	cc -o temp/optimizer.o -Wall -O3 -c src/optimizer.c
temp/expander.o:
	cc -o temp/expander.o -Wall -O3 -c src/expander.c
temp/testbfi.o:
	cc -o temp/testbfi.o -Wall -O3 -c src/testbfi.c
temp/compilerFunctions.o:
	cc -o temp/compilerFunctions.o -Wall -O3 -c src/compilerFunctions.c
temp/codeEss.o:
	cc -o temp/codeEss.o -Wall -O3 -c src/codeEss.c
temp/lextest.o: src/lextest.c
	cc -o temp/lextest.o -Wall -O3 -c src/lextest.c
temp/codeGen.o: src/codeGen.c
	cc -o temp/codeGen.o -Wall -O3 -c src/codeGen.c
temp/symbolTable.o: src/symbolTable.c src/compilerFunctions.c
	cc -o temp/symbolTable.o -Wall -O3 -c src/symbolTable.c
temp/grammar.o: src/grammar.c
	cc -o temp/grammar.o -Wall -O3 -c src/grammar.c
temp/tree.o: src/tree.c
	cc -o temp/tree.o -Wall -O3 -c src/tree.c
temp/main.o:  src/main.c
	cc -o temp/main.o -Wall -O3 -c src/main.c
temp/lex.o: src/lex.c
	cc -o temp/lex.o -Wall -O3 -c src/lex.c

