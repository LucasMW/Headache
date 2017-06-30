#include <stdio.h>


void codeStr(const char* str) {
	fprintf(output, "%s",str);
}
void codeGoTo(int cellIndex) {
	//printf("goto %d\n",cellIndex );
	int units = unitsToMoveTo(cellIndex);
	char direction = '>';
	int count=0;
	assert(currentCell + units == cellIndex);
	//printf("%dvs%d units: %d\n", currentCell, cellIndex,units);
	
	if(units < 0) {
		direction = '<';
		units = -units;
	}
	currentCell += units;
	
	for(;units>0;units--) {
		count++;
		fprintf(output, "%c",direction);
	}
	//printf("%d\n",count );	
	currentCell = cellIndex;
}