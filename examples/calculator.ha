//12/09/2020
//Author: Lucas Menezes
//Interactive Calculator program
void main() {
    char operator;
    byte a,b;
    print "Enter first number:\n";
    read a;
    print "Received number "; print a; print "\n";
    print "Enter second number:\n";
    read b;
    print "Received number "; print b; print "\n";
    print "Enter operator (+,-,*,/):\n";
    read operator;
    if(operator == '+') {
        print "add\n";
        print a+b; print "\n";
    } else if(operator == '-') {
        print "subtract\n";
        print a-b; print "\n";
    } else if(operator == '*'){
        print "multiply\n";
        print a*b; print "\n";
    } else if(operator == '/'){
        print "div\n";
        if(b == 0) {
            if (a == 0) {
                print "0/0 is undefined\n";
            } else {
                print "division by zero isn't allowed\n";
            }
        } else {
            print a/b; print "\n";
        }
    } else {
        print "unknown operation\n";
    }
}