// April 18 2020
// Author: Lucas Menezes
// Headache noobcrypt program
// Read from stdin and print to stdout
// Should work in both types of bf interpreters
void main() {
    char x;
    byte key;
    char cyphered,decyphered;
    @"NoobCrypt program\n";
    @"Enter key ";
    read key;
    @"-----------------------\n";
    read x;
    while(x){
        cyphered = (x+(key as char));
        //decyphered = (cyphered - (key as char) as char);
        @cyphered;
        //@decyphered;
        //@(x+(key as char)-key) as char;
        read x;
        if(x - (255 as char)){} else x = 0; //necessary for terminal emulation
    }
}