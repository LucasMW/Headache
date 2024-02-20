// Receives Headache Program, optization level and a bufferSize
// Generates a BF program and returns. Everything in RAM
char* compileHa2bf(const char* program, int level, int bufferSize);

// Receives Headache Program, optization level
// Auto calculates the bufferSize
// Generates a BF program and returns. Everything in RAM
char* compileHa2bfAutoBuffer(const char* program, int level);


// Uses filepaths for input and output
// Read and write from files
void compileHa2File(const char* input, const char* output, int level);


//Same but outputting LLVM
char* compileHa2LLVM(const char* program, int level, int bufferSize);
char* compileHa2LLVMAutoBuffer(const char* program, int level);
void compileHa2FileLLVM(const char* input, const char* output, int level);
