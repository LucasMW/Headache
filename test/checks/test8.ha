
void main () {
	int a;
	int b;
	b = 1;
	a =  2 + 2 + 3 / 2 + 5 * 7 + 3  - b;
	@ a;
	@ main();
}
/* output:
root
|
|-func main()
  |
  |- int a;
  |- int b;
  |- Assign 
 	|
 	|- a
 	|- exp
 	   |
 	   |- +
 	   	  |- +
 	   	  |  |
 	   	  |	 |- /
 	   	  |	 |  |
 	   	  |	 |	|- 3
 	   	  |	 |	|- 2
 	   	  |	 |
 	   	  |	 |- + 
 	   	  |	 |	|
 	   	  |	 	|-2
 	   	  |	 	|-2
 	   	  |
 	   	  |	 
 	   	  |- + 
 	   	  |	 |
 	   	  	 |- +
 	   	  	   	|
 	   	  	   	| *


2 + 2 + (3 / 2) + (5 * 7) + 3 - b;
*/