#include <stdio.h>

static FILE* output = NULL;

void setOutput(FILE* out){
  output = out;
}

void codeStr(const char* str) {
  fprintf(output, "%s",str);
}