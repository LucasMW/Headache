#!/bin/bash
FILES="test/*.ha"
COUNT="0"
OK=0
Wrongs=0
for f in $FILES
do
	let COUNT=COUNT+1
	name=$(echo $f | cut -f 1 -d '.')
	echo "Testing $name"
	cat $f | ./comp -noBin > $f.output
	mv a.ll $name.ll
	if(clang $name.ll -c -o $name.out) then
		rm $f.output
		rm $name.ll
		rm $name.out
		echo "OK"
		let OK=OK+1
	else
		echo "Wrong"
		
	fi
done
echo "$OK OK of $COUNT in total"
