#include <stdio.h>
#include <string.h>

static char bfchars[] = {'>','<','+','-',',','.','[',']'};
static int isBfChar(char c){
	for(int i=0;i<8;i++){
		if(c == bfchars[i]){
			return 1;
		}
	}
	return 0;
}
static int zmark = 1002;
static int ymark = 1001;
static int xmark = 1000;
int main() {
	char c;
	int n;
	//char string[strlen("temp")];
	//char * p = string;
	char bigStr[1024*1024]; //1MB
	int tempList[999]; 
	int * tempListPtr = tempList;
	char* bigStrPtr = bigStr;
	while(fscanf(stdin,"%c",&c)!=EOF){
		if(!isBfChar(c)) {
			if(c == 't') {
				if(fscanf(stdin,"%c",&c)!=EOF){
					if(c == 'e') {
						if(fscanf(stdin,"%c",&c)!=EOF){
							if(c == 'm') {
								if(fscanf(stdin,"%c",&c)!=EOF){
									if(c == 'p'){
										if(fscanf(stdin,"%d",&n)!=EOF){
											*bigStrPtr = '$';
											bigStrPtr++;
											*tempListPtr = n;
											tempListPtr++;
											continue;
										}
									}
								}
							}
						}
					}
				}
			}
			else {
				*bigStrPtr = c;
				if(c == 'x'){
					*bigStrPtr = '$';
					*tempListPtr = xmark;
					tempListPtr++;
				} else if (c == 'y'){
					*bigStrPtr = '$';
					*tempListPtr = ymark;
					tempListPtr++;
				} else if (c == 'z'){
					*bigStrPtr = '$';
					*tempListPtr = zmark;
					tempListPtr++;
				}
				bigStrPtr++;
				continue;
			}
		}
		*bigStrPtr = c;
		bigStrPtr++;
	}
	*bigStrPtr = '\0';
	*tempListPtr = -1;
	printf("\nbfalgo(\"%s\"",bigStr);
	for(tempListPtr = tempList; *tempListPtr > -1; tempListPtr++){
		
		if(*tempListPtr == xmark){
			printf(",x");

		} else if(*tempListPtr == ymark) {
			printf(",y");
		}
		 else if(*tempListPtr == zmark) {
			printf(",z");
		}
		else {
			printf(",temp%d",*tempListPtr);
		}
		
	}
	printf(");\n");
}
