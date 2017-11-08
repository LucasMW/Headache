#!/bin/bash
FILES="test/checks/*.ha"
COUNT="0"
OK=0
Wrongs=0
for f in $FILES
do
	COUNT=$((COUNT+1))
	name=$(echo $f | cut -f 1 -d '.')
	echo "Testing $name"
	./hac -check $f > $f.output
	if [ ! -f $name.answer ]; then
    	echo "no answer associated"
    	continue
	fi
	if(cmp $f.output $name.answer) then
		rm $f.output
		echo "OK"
		OK=$((OK+1))
	else
		echo "Wrong"
		diff $f.output $name.answer
	fi
done
echo "$OK OK of $COUNT in total"
