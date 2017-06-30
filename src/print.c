/*
x [>>+>+<<<-]>>>[<<<+>>>-]<<+>[<->[>++++++++++<[->-[>+>>]>[+[-<+>]>+>>]<<<<<]>[-]
  ++++++++[<++++++>-]>[<<+>>-]>[<<+>>-]<<]>]<[->>++++++++[<++++++>-]]<[.[-]<]<

*/
void printValueOfAnySizedCell(int x){
	codeGoTo(x);
	codeStr(">>+>+<<<-]>>>[<<<+>>>-]<<+>[<->[>++++++++++<[->-[>+>>]>[+[-<+>]>+>>]<<<<<]>[-]++++++++[<++++++>-]>[<<+>>-]>[<<+>>-]<<]>]<[->>++++++++[<++++++>-]]<[.[-]<]<");
}

/* 
[-]>[-]+    // Clear sum
   [[-]                // Begin loop on first temp
   >[-],               // Clear the inp buffer to detect leave on eof and input
       [
           +[                          // Check for minus one on eof
               -----------[            // Check for newline
                   >[-]++++++[<------>-]       // Subtract 38 to get the char in zero to nine
                   <--<<[->>++++++++++<<]      // Multiply the existing value by ten
                   >>[-<<+>>]          // and add in the new char
               <+>]
           ]
       ]
   <]
   <
*/
void receiveInput() {

}