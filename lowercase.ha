bit isLowerCase(char c){
    char y;
    bit answer;
    y = 'Z'+1;
    answer = false;
    while(c){
        c--;
        y--;
        if(c){ 
        } else {
            answer = false;
        }
        if(y) {
        } else {
            answer = true;
            c = 0;
        }
    }
    return answer;
}
void main(){
    char x;
    @"Enter a character\n";
    read x;
    if(isLowerCase(x)){
        @"received char "; @x; @" is lowercase\n";
    } else {
        @"received char "; @x; @" is not lowercase\n";
    }
    
}