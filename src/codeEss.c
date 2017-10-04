
void bfalgo(char* str, ...){

//based on http://www.gnu.org/software/libc/manual/html_node/Variadic-Example.html
  va_list ap;
  int i;
  int count = 0;

  for(i=0;str[i];i++){
  	if(str[i] == '$' || str[i] == '@') {
  		count++;
  	}
  }
  printf("count %d\n",count );
  va_start (ap, count);         /* Initialize the argument list. */

  for(i=0;str[i];i++){
  	if(str[i] == '$') {
  		int d =  va_arg (ap, int);
  		codeGoTo(d);
  	} else if (str[i] == '@'){ // Not sure if I will use
  		char* str = va_arg(ap,char*);
  		codeStr(str);
  	}
  	else {
  		fprintf(output, "%c",str[i]);
  	}
  }
  va_end (ap);                  /* Clean up. */
}
void codeZero(int x) {
	bfalgo("$[-]",x);
}
