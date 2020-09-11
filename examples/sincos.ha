 short factorial(short x){
 	short y,z;
 	y = x;
 	z = 1;
 	while(y) {
 		z = y * z;
 		y--;
 	}
 	return z;
 }
 short pow(short x, short y){
	short r,z;
	r=1;
	z = y;
	while(z){
		r = r*x;
		z--;
	}
	return r;
}
short exp(short x){
    byte grade;
    short sum,n,z;
    grade = 2;
    sum = 0;
    n = 1;
    while(n){
        @"n is "; @n; @"\n";
        sum = sum + ((100 as short) * pow(x,n))/factorial(x);
        n--;
    }
    return sum;
}
short sin(short x){
    short grade;
    short i;
    short sum;
    
    sum = (100 as short)*((1 as short) - x*x*x/(6 as short) + pow(x,5 as short)/(120 as short));
    return sum;
}
short cos(short x){
    short grade;
    short i;
    short sum;

    //sum = (x - x*x/2 + pow(x,4)/factorial(4))*100;
    return sum;
}
void main() {
    short x,e;
    read x;
    e = exp(x);
    print "exp("; @x; @") = "; @e; @"\n";
    print "sin("; @x; @") = "; @sin(x); @"\n";
}
