/* Algorithms extracted from 

https://esolangs.org/wiki/Brainfuck_bitwidth_conversions 

Quote:
"Depending on the optimisations that an interpreter does they may be faster or slower. 
In addition they can be applied repeatedly to, in theory, expand the cells to any size required.
For example, the 'No Copy' routine will be the fastest on an interpreter that does not optimise, 
even if it's applied twice to create a 1→4 conversion."
*/

char * expand8to16NoCopy(char b);
char * expand8to16(char b);
char * expand8to32(char b);
void runStr(FILE* out, char* str, int mode);
char* expandedString(const char* str, int mode);