
/* if test */
void main(int argc,byte[][] argv) 
{
	if(argc == 1)
		printf("0");
	else if (argc == 2)
		printf("2");
	else if (argc == 3)
		printf("3");
	else if (argc == 4)
		printf("4");
	else if (argc == 5)
		printf("5");
	else
		printf("Unnecessary");
}

void makeAAd1(int a)
{
	if(!a) {

	} else if (a > 2){

	} else if (a < 3){

	}
	else
		printf("hi");

}
void mWhile(int a)
{
	while(!( a >= ']' && a < 150)) 
				/* while not match stop requirements*/
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
					/*prgIdx = prgIdx + 1;*/
				}


}