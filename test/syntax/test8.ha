void main () {
	int a;
	int b;
	a =  2 + 2 + 3 / 2 + 5 * 7 + 3  - b;
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