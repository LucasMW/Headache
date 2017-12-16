#!/bin/bash

if [ !command -v timeout]; then
	echo ""
else
	alias timeout="gtimeout"
fi 

FILES="test/optimize/*.ha"
COUNT="0"
OK=0
Wrongs=0
for f in $FILES
do
	COUNT=$((COUNT+1))
	name=$(echo $f | cut -f 1 -d '.')
	echo "Testing $name"
	if [ ! -f $name.answer ]; then
    	echo "no answer associated"
    	continue
	fi
	./hac -noBin $f -O1 > $f.O1.output
	mv a.bf a.O1.bf
	./hac -noBin $f > $f.output
	timeout 2 ./bfi a.bf > $f.run.output
	timeout 2 ./bfi a.O1.bf > $f.run.O1.output
	if(cmp $f.run.output $name.answer ) then
		if(cmp $f.run.output $f.run.O1.output) then
			rm $f.output
			rm $f.run.output
			rm $f.run.O1.output
			rm $f.O1.output
			echo "OK"
			OK=$((OK+1))
		else
			echo "Wrong"
			diff $f.run.output $f.run.O1.output
		fi
	else
		echo "Wrong"
		diff $f.run.output $name.answer
	fi
done
echo "$OK OK of $COUNT in total"
