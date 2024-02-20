short mod2(short a){
	short r,q;
	q = a/(2 as short);
	r = a - (2 as short)*q;
	print "(a,2)"; @"= ("; @a; @","; @2; @")";
	print "(r,q)"; @"= ("; @r; @","; @q; @")";
	return r;
}

void main() {
	short x,y,z;
	print "Enter a number:\n";

	read x;
	print "got "; @x; @" \n";
	y = x - (1 as  short);
	while(y){
		z = mod2(x);
		print "z is "; @z; @"\n";
		if(z == 0) {
			print "even\n";
			x = x/(2 as short);
		} 
		else {
			print "odd\n";
			x = (3 as short)*x+(1 as short);
		}
		print "=> "; @x; @"\n";
		y--;
		
	}
}