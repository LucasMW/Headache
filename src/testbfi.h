int lookForWrittenMemory();
void printMemory(int start);
void printAllWrittenMemory();
/* Returns the exactly number of cells used to run */ 
int execute(char* program,int memorySize, char extra);
/* Verifies if brackets are matched */
int parse(char* program);
char * readFile(const char* path);
char* readFileFast(const char* path);