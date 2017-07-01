/*
*********************** 
	Test Program 1 
***********************
 Just an ordinary program 
 to test data types and 
 ordinary data structures
*/
void main()
{
	char value;
	float fvalue;
	char[] array;
	if(value > 0)
	{
		/* only need to alloc array inside the if */
		array = new char[3];
		fvalue = 87; 
		array[1] = value; /* need to overwrite the array */
	}
	else
	{
		fvalue = 90;
	}
	printf("value is %f\n",fvalue);
	printf("END\n");
	return 0; /* no errors */
}