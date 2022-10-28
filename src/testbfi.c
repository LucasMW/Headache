#define DEFAULT_SIZE 30000
#define STACK_MAX 10000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// based on  http://groups.csail.mit.edu/graphics/classes/6.837/F04/cpp_notes/stack1.html

typedef struct Stack {
    char* data[STACK_MAX];
    int size;
} Stack;

typedef enum state {
	used,
	clean
} State;

typedef struct memBlock {
	char * block_start;
	int size;
	State state;
} MemBlock;

void Stack_Init(Stack **S){
    *S = calloc(sizeof(Stack),1); //allocating a Stack
    (*S)->size = 0;
}

char* Stack_Top(Stack *S){
    if (S->size == 0) {
        fprintf(stderr, "Error: stack empty\n");
        return NULL;
    }
    return S->data[S->size-1];
}

void Stack_Push(Stack *S, char* d){
    if (S->size < STACK_MAX)
        S->data[(S->size)++] = d;
    else
        fprintf(stderr, "Error: stack full\n");
}

void Stack_Pop(Stack *S){
    if (S->size == 0)
        fprintf(stderr, "Error: stack empty\n");
    else {
        S->size--;
    }
}

char* memory;

int lookForWrittenMemory(){
	for(int i=0;i<30000;i++){
		//printf("%d\n",i);
		if(memory[i]!=0) {
			//printf("found %d\n", i);
			return i;
		}
	}
	return -1;
}
void printMemory(int start) {
	printf("start %d: ",start );
	for(int i=start;i<start+18;i++){
		unsigned char c = (unsigned char)memory[i];
		printf("|%u",(unsigned int)c);
	}
	printf("|\n");
}
static inline int isWritten(char* memory,int wide){
	for(int j=0;j<=wide;j++){
	   if(memory[j]!=0){
	      return 1;
	   }
	}
	return 0;
} 
void printAllWrittenMemory(){
  	int wide = 18;
  	int limit = 30000 - wide-1;
  	for(int i=0;i<limit;i+=wide){
	    if(isWritten(memory+i,wide)) {
	      	printMemory(i);
	    }
	}
}

char* readFileFast(const char* path){
	long size;
	int read;
	FILE * input;
	char * fileString;
	input = fopen(path,"rb");
	if(!input)
		return NULL;
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	rewind(input);
	fileString = (char*)malloc(size+1);
	if(!fileString){
		fclose(input);
		return NULL;
	}
	read = fread(fileString,1,size,input);
	if(read != size) {
		fclose(input);
		free(fileString);
		return NULL;
	}
	fileString[size] = '\0';
	fclose(input);
	return fileString;
}
char * readFile(const char* path){
	int size;
	FILE * input;
	char * fileString;
	char c;
	int i;
	input = fopen(path,"rt");
	if(!input)
		return NULL;
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	rewind(input);
	fileString = (char*)malloc(size+1);
	i = 0;
	while(fscanf(input,"%c",&c) != EOF){
		fileString[i] = c;
		i++;
	}
	fileString[size] = '\0';
	fclose(input);
	return fileString;
}

int execute(char* program,int memorySize, char extra){

	Stack* loopStack;
	char instructions[] = {'>','<','+','-','.',',','[',']'};
	char extras_instructions[] = {'@','#','$','%','&'};
	char* ptr; 
	char c; 
	char *loopStart = NULL;
	int internalLoopCount = 0;
	char* farthestPtr = memory;
	memory = (char*) calloc(sizeof(char),memorySize); // memory of program must be zeroed.
	const char* memLimit = memory + memorySize -1;
	ptr = memory;
	Stack_Init(&loopStack);
	//printf("%s\n", extra ? "extra enabled" : "extra disabled" );
	while(*program){
		c = *program;
		if (c == instructions[0]) {
			if(ptr == memLimit) {
				printf("Access violation, there is no cell %d \n",memorySize);
				exit(1);
			}
			++ptr;
		}
		else if (c == instructions[1]){
			if(ptr == memory) {
				printf("Access violation, there is no cell %d\n",-1);
				exit(1);
			}
			--ptr;
		}
		else if (c == instructions[2]){
			++*ptr;
		}
		else if (c == instructions[3]){
			--*ptr;
		}
		else if (c == instructions[4]){
			putchar(*ptr);
		}
		else if (c == instructions[5]){
			*ptr=getchar();
		}
		else if (c == instructions[6]){
		    // will enter while
			if(*ptr) {
				loopStart = program;
				Stack_Push(loopStack,loopStart);
			}
			// skip while sequence
			else {
				program++;
				internalLoopCount = 0;
				// while not match stop requirements
				while(!(*program == ']' && internalLoopCount == 0)) {
					if(*program == '['){
						internalLoopCount++;
					}
					else if(*program == ']'){
						internalLoopCount--;
					}
					program++;
				}
			}
		}
		else if (c ==instructions[7]){
			// repeats loop
			if(*ptr){
				loopStart = Stack_Top(loopStack);
				program = loopStart;
			}
			else{
				Stack_Pop(loopStack); // out of this loop
			}
		}
		// output memory
		else if(extra) {
			if(c == extras_instructions[0]){
				printf("\n---\t bfi Debug Begin ---\t\n");
				printAllWrittenMemory();
				printf("---\t bfi Debug End ---\t\n" );
			}
			else if( c == extras_instructions[1]) {
			
			}
			else if( c == extras_instructions[2]) {
			
			}
			else if( c == extras_instructions[3]) {
			
			}
			else if( c == extras_instructions[4]) {
			
			}
		}
		else{

		}
		program++;
		if(ptr > farthestPtr)
		{
			farthestPtr = ptr;
		}
	}
	free(loopStack);
	return farthestPtr - memory;
}
int parse(char* program){
	char* p = program;
	int open = 0;
	while(*p){
		if(*p=='['){
			open++;
		}
		if(*p==']'){
			if(open<=0){
				return 0;
			}
			open--;
		}
		p++;
	}
	return open == 0;
}
#ifdef STANDALONE
int shouldHelp(int argc, char** argv) {
	for(int i=1;i<argc;i++){
		if(strcmp(argv[i],"--help")==0){
			return 1;
		}
	}
	return 0;
}
void help(){
	printf("expects: bfi filepath [-debug | -extra | -memory number]\n");
}
int main (int argc, char** argv){
	char * program;
	char extra = 0;
	int size = DEFAULT_SIZE;
	//expects bfi filepath [-debug -extra]
	//expects bfi filepath [-memory 100000]
	if(shouldHelp(argc,argv)){
		help();
		return 0;
	}
	if(argc >= 4){
		if(strcmp(argv[2],"-memory")==0){
			size = atoi(argv[3]);
		}
	}
	else if(argc >= 3)
	{
		if(strcmp(argv[2],"-extra")==0){
			extra = 1;
		}
		else if(strcmp(argv[2],"-debug")==0) {
			extra = 1;
		}
	} 
	if(argc >= 2){
		program = readFileFast(argv[1]);
		if(!program){
			printf("Couldn't read file: %s\n",argv[1]);
			return -1;
		}
		else if(!parse(program)){
			printf("Unmatched brackets. Aborting\n");
			return -1;
		} else {
			execute(program,size,extra);
			free(memory);
			free(program);
			return 0;
		}
		return -1;
	}
	else{
		printf("Error!\nProgram usage is: \n%s sourcecode\n",argv[0]);
		return -1;
	}
}
#endif
