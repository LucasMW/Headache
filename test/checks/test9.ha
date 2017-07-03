
/* if test */
void main(int argc,byte[][] argv) 
{
	if(argc == 1)
		@ "0";
	else if (argc == 2)
		@"2";
	else if (argc == 3)
		@"3";
	else if (argc == 4)
		@"4" ;
	else if (argc == 5)
		@"5";
	else
		@ "Unnecessary" ;
}

void makeAAd1(int a)
{
	if(!a) {

	} else if (a > 2){

	} else if (a < 3){

	}
	else
		@"hi";

}
void mWhile(int a)
{
	byte[] program;
	int prgIdx;
	int internalLoopCount;
	while(!( a >= ']' && a < 150)) 		/* while no match stop requirements*/
	{
					if(program[prgIdx] == '[')
					{
						internalLoopCount = internalLoopCount + 1;
					}
					/* that is the case to fix */
					else if(program[prgIdx] == ']')
					{
							internalLoopCount = internalLoopCount - 1;
					}
					prgIdx = prgIdx + 1;
	}


}