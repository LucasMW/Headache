void rec(byte n) {
    if(n) {
        @n;@"\n";
        rec(n-1);
    }
}
void recLoop(byte l){
    byte n;
    n = l;
    while(n){
        @n;@"\n";
        n--;
    }
}
void main() {
    byte n;
    print "Recursion\n";
    print "Enter number:\n";
    read n;
    //rec(n);
    recLoop(n);
}
