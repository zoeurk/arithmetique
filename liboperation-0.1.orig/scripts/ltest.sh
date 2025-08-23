#!/bin/sh
gdate(){
	DATE=`date "+%M:%S:%N"`
	MIN=${DATE%%:*}
	SEC=${DATE%:*}
	SEC=${SEC##*:}
	NANO=${DATE#$MIN:$SEC:}
	echo "$MIN*60*1000000000+$SEC*1000000000+$NANO" | bc
}
j=9999999999999999
SCALE=$((26*$(shuf -i 0-65535 -n1)/65535))
SCALE1=$((26*$(shuf -i 0-65535 -n1)/65535))
SCALE2=$((26*$(shuf -i 0-65535 -n1)/65535))
VAL1=`printf "scale=$SCALE1;$j*$(shuf -i 0-65535 -n1)/65535\n" | BC_LINE_LENGTH=0 bc`
VAL2=`printf "scale=$SCALE2;$j*$(shuf -i 0-65535 -n1)/65535\n" | BC_LINE_LENGTH=0 bc`
printf "test with $VAL1, $VAL2 and scale $SCALE\n"
D=${VAL1#*.}
DOT1=${#D}
D=${VAL2#*.}
DOT2=${#D}
S_MUL=$(($DOT1+$DOT2))
if printf "$VAL2 == 0\n" | bc | grep -q "1"
then
	printf "Division by 0...\nSkipping\n"
	continue
fi
printf "VALUES: $VAL1 $VAL2 $SCALE\n" | tee -a calcule.txt
gdate;
LD_LIBRARY_PATH="./operation;./arithmetique" ./main-bin $VAL1 $VAL2 $SCALE >/dev/null
gdate;
gdate
printf "$VAL1+$VAL2\n$VAL1-$VAL2\nscale=$S_MUL;$VAL1*$VAL2\nscale=$SCALE;$VAL1/$VAL2\nscale=$SCALE;$VAL1%%$VAL2\n" | BC_LINE_LENGTH=0 bc > /dev/null
gdate
