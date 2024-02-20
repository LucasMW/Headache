CFLAGS = -Wall -std=c99 -O3
CFLAGSDEBUG = -g -Wall -std=c99 -O0
OUTFILE = hac
SOURCES = src/main.c src/lex.c src/grammar.c src/tree.c src/lextest.c src/symbolTable.c src/codeGen.c src/testbfi.c src/compilerFunctions.c src/codeEss.c src/optimizer.c src/expander.c src/highlight.c src/codellvm.c
OBJS = temp/codeGen.o temp/symbolTable.o temp/grammar.o temp/tree.o temp/main.o temp/lex.o temp/lextest.o temp/testbfi.o temp/compilerFunctions.o temp/codeEss.o temp/optimizer.o temp/expander.o temp/highlight.o temp/codellvm.o

ZIGCC = zig cc
WINCC = x86_64-w64-mingw32-gcc-11.1.0 #old one was i686-w64-mingw32-gcc; changed because false positives

#always compiles when using just make
test/hac: src/main.c src/lex.c src/grammar.c
	cc $(CFLAGS) -o hac $(SOURCES)

debug: $(SOURCES)
	cc $(CFLAGSDEBUG) -o hac $(SOURCES)
release: $(SOURCES)
	cc $(CFLAGS) -o hac $(SOURCES)
bin/hac.exe: $(SOURCES)
	$(WINCC) $(CFLAGS) -o bin/hac.exe $(SOURCES)
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
	$(WINCC) $(CFLAGS) -DSTANDALONE src/testbfi.c -o bfi.exe
bfi.linux: src/testbfi.c
	x86_64-linux-musl-cc $(CFLAGS) -DSTANDALONE src/testbfi.c -o bfi.linux
bfi.js: src/testbfi.c
	emcc $(CFLAGS) -DSTANDALONE src/testbfi.c -s FORCE_FILESYSTEM=1 -lnodefs.js -o bfi.js

expander.exe: src/expander.c
	$(WINCC) $(CFLAGS) -DSTANDALONE src/expander.c -o expander.exe
expander.linux: src/expander.c
	x86_64-linux-musl-cc -static $(CFLAGS) -DSTANDALONE src/expander.c -o expander.linux

bfalgoConverter.exe: src/bfalgoConverter.c
	$(WINCC) -static $(CFLAGS) src/bfalgoConverter.c -o bfalgoConverter.exe

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
temp/lib.o:
	cc -DHACLIB -o temp/lib.o -Wall -O3 -c src/main.c
temp/main.o:  src/main.c
	cc -o temp/main.o -Wall -O3 -c src/main.c
temp/lex.o: src/lex.c
	cc -o temp/lex.o -Wall -O3 -c src/lex.c
temp/highlight.o: src/highlight.c
	cc -o temp/highlight.o -Wall -O3 -c src/highlight.c
temp/codellvm.o: src/codellvm.c
	cc -o temp/codellvm.o -Wall -O3 -c src/codellvm.c

## LIBS
hac.a: $(OBJS) temp/lib.o
	rm -f temp/main.o
	ar ruv hac.a  temp/*.o
	ranlib hac.a

hac.so: $(OBJS) temp/lib.o
	rm -f temp/main.o
	cc -shared temp/*.o -o hac.so



## ZIG CC EXPERIMENT

zig_windows : bfi_z.exe hac_z.exe expander_z.exe bfalgoConverter_z.exe

bfi_z.exe: src/testbfi.c
	$(ZIGCC) $(CFLAGS) -DSTANDALONE src/testbfi.c -o bfi_z.exe -target x86_64-windows-gnu
hac_z.exe: $(SOURCES)
	$(ZIGCC) $(CFLAGS) -o hac_z.exe $(SOURCES) -target x86_64-windows-gnu
expander_z.exe: src/expander.c
	$(ZIGCC) $(CFLAGS) -DSTANDALONE src/expander.c -o expander_z.exe -target x86_64-windows-gnu
bfalgoConverter_z.exe: src/bfalgoConverter.c
	$(ZIGCC) $(CFLAGS) src/bfalgoConverter.c -o bfalgoConverter_z.exe -target x86_64-windows-gnu

hac-release-z-windows.zip: bfi_z.exe hac_z.exe expander_z.exe bfalgoConverter_z.exe
	rm -rf zipfolder.zip
	rm -f src/*.o
	zip -r zipfolder.zip hac_z.exe expander_z.exe bfi_z.exe bfalgoConverter_z.exe
	mv zipfolder.zip hac-release-z-windows.zip

zig: hac_z bfi_z expander_z bfalgoConverter_z

bfi_z: src/testbfi.c
	$(ZIGCC) $(CFLAGS) -DSTANDALONE src/testbfi.c -o bfi_z -target x86_64-macos-gnu
hac_z: $(SOURCES)
	$(ZIGCC) $(CFLAGS) -o hac_z $(SOURCES) -target x86_64-macos-gnu
expander_z: src/expander.c
	$(ZIGCC) $(CFLAGS) -DSTANDALONE src/expander.c -o expander_z -target x86_64-macos-gnu
bfalgoConverter_z: src/bfalgoConverter.c
	$(ZIGCC) $(CFLAGS) src/bfalgoConverter.c -o bfalgoConverter_z -target x86_64-macos-gnu

