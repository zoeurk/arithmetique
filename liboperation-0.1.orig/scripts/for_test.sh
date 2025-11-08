#!/bin/sh
rm -v math.* calcule.txt bc.txt 2>/dev/null
if test ! -e main-bin
then
	printf "You need to compile program\n"
	exit
fi
for k in $(seq 50)
do
	for j in 999999999999999999999999 1 0.1
	do
		for i in $(seq 100)
		do
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
			LD_LIBRARY_PATH=./operation ./main-bin $VAL1 $VAL2 $SCALE | sed '1, 4d' > math.txt || exit
			sed 's/^.*= *//g' math.txt >> math.result.txt
			printf "$VAL1+$VAL2\n$VAL1-$VAL2\nscale=$S_MUL;$VAL1*$VAL2\nscale=$SCALE;$VAL1/$VAL2\nscale=$SCALE;$VAL1%%$VAL2\n" | BC_LINE_LENGTH=0 bc >> bc.txt
			sed -e 's/^\./0./g' -e 's/^-\./-0./g' -e '/\./ s/0*$//g' -e 's/\.$//g' -i bc.txt
			diff -q math.result.txt bc.txt || exit
		done
	done
done
printf "test == OK\n"
