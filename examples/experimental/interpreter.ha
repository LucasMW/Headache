bit eq(char c,byte b){
    return (c - (b as char)) as bit;
}
void main() {
    char c;
    char mem;
    read c;
    while(c){
        if (eq(c,'+')){} else { 
            mem++; 
            @mem as byte;
        }
        if (eq(c,'-')){} else { mem--; }
        if (eq(c,'.')){} else { @mem; }
        if (eq(c,',')){} else { read mem; }
        read c;
        if(c - (255 as char)){} else c = 0; //necessary for terminal emulation
    }
    print "Finished\n";
}