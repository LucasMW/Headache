byte modulus8(byte a, byte b){
	byte r,q;
	q = a/b;
	r = a - b*q;
	return r;
}

void factor(byte n) {
    byte x,i;
    print n; @"! = ";
    x = n;
    i = 2;
    while (x > 1) {
      if (modulus8(x,i) == 0) {
        print " "; @i;
        x = x / i;
        i = 1;
      }
      i++;
    }
    print "\n";
  }
void main() {
    byte n;
    print "Enter number:\n";
    read n;
    factor(n);
}