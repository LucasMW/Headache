/* temp0[-]
temp1[-]
x[temp1+x-]+
y[temp1-temp0+y-]
temp0[y+temp0-]
temp1[x-temp1[-]]*/
void equals(int x,int y){

}
/*temp0[-]
temp1[-]
x[temp1+x-]
y[temp1-temp0+y-]
temp0[y+temp0-]
temp1[x+temp1[-]] */
void unequals(int x,int y){

}

/*temp0[-]
x[temp0+x[-]]+
temp0[x-temp0-] */
void negateX(int x){

}

/*temp0[-]
temp1[-] >[-]+ >[-] <<
y[temp0+ temp1+ y-]
temp0[y+ temp0-]
x[temp0+ x-]+
temp1[>-]> [< x- temp0[-] temp1>->]<+<
temp0[temp1- [>-]> [< x- temp0[-]+ temp1>->]<+< temp0-]*/

void lesserOrEqual(int x,int y){

}

/*temp0[-]
temp1[-] >[-]+ >[-] <<
y[temp0+ temp1+ y-]
temp1[y+ temp1-]
x[temp1+ x-]
temp1[>-]> [< x+ temp0[-] temp1>->]<+<
temp0[temp1- [>-]> [< x+ temp0[-]+ temp1>->]<+< temp0-] */
void lesser(int x, int y){

}

/*temp0[-]temp1[-]z[-]
x[ temp0+
       y[- temp0[-] temp1+ y]
   temp0[- z+ temp0]
   temp1[- y+ temp1]
   y- x- ]*/
void zReceivesXGreaterThanY(int x,int y){

}

/*temp0[-]
x[temp0+x[-]]+
temp0[x-temp0-] */
void XAndY(int x, int y){

}

/*temp0[-]
temp1[-]
x[temp1+x-]
temp1[x-temp1[-]]
y[temp1+temp0+y-]temp0[y+temp0-]
temp1[x[-]-temp1[-]] */
void XOrY(int x,int y){

}














