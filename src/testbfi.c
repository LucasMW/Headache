#define DEFAULT_SIZE 30000
#define STACK_MAX 10000
#include <stdio.h>
#include <stdlib.h>

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
		printf("|%d",(unsigned int)memory[i]);
	}
	printf("|\n");
}
void printAllWrittenMemory(){
  	char mark = 0;
  	int count = 0;
  	for(int i=0;i<30000;i+=18){
	  	mark = 0;
		for(int j=i;j<=18;j++){
	      if(memory[i]!=0){
	        mark = 1;
	        break;
	      }
	    }
	    if(mark == 1) {
	      	printMemory(i);
	    }
	}
}


char * readFile(const char* path){
	int size;
	FILE * input;
	char * fileString;
	char c;
	int i;
	input = fopen(path,"rt");
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

void execute(char* program,int memorySize){

	Stack* loopStack;
	char instructions[] = {'>','<','+','-','.',',','[',']'};
	char extras_instructions[] = {'@','#','$','%','&'};
	char* ptr; 
	char c; 
	char *loopStart = NULL;
	int internalLoopCount = 0;
	memory = (char*) calloc(sizeof(char),memorySize); // memory of program must be zeroed.
	const char* memLimit = memory + memorySize -1;
	ptr = memory;
	Stack_Init(&loopStack);
	while(*program){
		c = *program;
		if (c == instructions[0]) {
			if(ptr == memLimit) {
				printf("Access violation\n");
				exit(1);
			}
			++ptr;
		}
		else if (c == instructions[1]){
			if(ptr == memory) {
				printf("Access violation\n");
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
		else if( c == extras_instructions[0]) {
			printMemory(ptr - memory);
		}

		else if( c == extras_instructions[1]) {
			
		}
		else if( c == extras_instructions[2]) {
			
		}
		else if( c == extras_instructions[3]) {
			
		}
		else if( c == extras_instructions[4]) {
			
		}
		else{

		}
		program++;
	}
	//free(memory);
	//free(loopStack);
}
#ifdef STANDALONE
int main (int argc, char** argv){
	char * program;
	//expects bfi filepath
	if(argc == 2){
		program = readFile(argv[1]);
		execute(program,DEFAULT_SIZE);
		free(program);
		return 0;
	}
	else{
		printf("Error!\nProgram usage is: \n%s sourcecode\n",argv[0]);
		return -1;
	}
}
#endif
