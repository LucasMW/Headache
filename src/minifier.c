
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include "testbfi.h"
void minifyBf(char* bf);
void simplifyBf(char* bf);
int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage %s file.bf\n",argv[0]);
        return 1;
    }
    char* bf = readFileFast(argv[1]);
    if(!bf){
       printf("Error: Can't open %s\n",argv[1]);
       return 2;
    }
    //int c1 = clock();
    minifyBf(bf);
    //int c2 = clock();
    simplifyBf(bf);
    //int c3 = clock();
    printf("%s\n",bf);
    //int c4 = clock();
   // printf("minify %d cicles\n", c2 - c1);
    //printf("simplify %d cicles\n", c3 - c2);
    //printf("print %d cicles\n", c4 - c3);
    free(bf);
    return 0;
}
void minifyBf(char* bf){
    char* p, *q;
    p = bf;
    q = bf;
    while(*p){
        switch(*p){
            case '>':
            case '<':
            case '+':
            case '-':
            case ',':
            case '.':
            case '[':
            case ']':
                *q = *p;
                q++;
        }
        p++;
    }
    q++;
    *q = '\0';
}

// int shouldCollapse(char* stack, int len) {
//     if (len <= 1) return 0;
//     char top = stack[len - 1];
//     char subtop = stack[len - 2];
//     return (top == '>' && subtop == '<') || (top == '<' && subtop == '>')
//         || (top == '+' && subtop == '-') || (top == '-' && subtop == '+');
// }

// void simplifyBf(char* bf) {
//     int len = strlen(bf);
//     int writeIdx = 0;
//     for (int i = 0; i < len; ++i) {
//         bf[writeIdx] = bf[i];
//         writeIdx++;
//         while (writeIdx > 1 && shouldCollapse(bf, writeIdx)) {
//             writeIdx -= 2;
//         }
//     }
//     bf[writeIdx] = '\0';
// }


int shouldCollapse(char *stack){
    int len = strlen(stack);
    if(len <= 1) return 0;
    char* top = stack + len - 1;
    char* subtop = top - 1;
    return (*top == '>' && *subtop == '<') || (*top == '<' && *subtop == '>')
    || (*top == '+' && *subtop == '-') || (*top == '-' && *subtop == '+');
    
}
void simplifyBf(char* bf){
    char* p;
    char* stack = (char*)malloc(strlen(bf));
    char * s = stack;
    p = bf;
    *s = *p;
    while(*p){
            *s = *p;
            s++;
            while(shouldCollapse(stack)){
                *s = '\0';
                s--;
                *s = '\0';
                s--;
            }
        p++;
    }
    s++;
    *s = '\0';
    p = bf;
    strcpy(p,stack);
    free(stack);
}
