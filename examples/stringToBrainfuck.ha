// April 18 2020
// Author: Lucas Menezes
// String to Brainfuck program
// Read from stdin and print to stdout
// Should work in both types of bf interpreters
void main() {
    char c;
    byte bf;
    read c;
    while(c){
        bf = c;
        while(bf){
            @"+";
            bf--;
        }
        @".[-]";
        read c;
        if(c - (255 as char)){} else c = 0; //necessary for terminal emulation
    }
}