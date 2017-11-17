int lookForWrittenMemory();
void printMemory(int start);
void printAllWrittenMemory();
/* Returns the exactly number of cells used to run */ 
int execute(char* program,int memorySize, char extra);
char * readFile(const char* path);