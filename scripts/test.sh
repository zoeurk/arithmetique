#!/bin/dash
export LD_LIBRARY_PATH=./operation
mul1=${1#*.}
mul2=${2#*.}
mul=`printf "${#mul1}+${#mul2}\n" | bc`
printf "======= calcule =========\n"
./scripts/date.sh > date.cal
test -n "$4" && ./main-bin $1 $2 $3 "$4" || ./main-bin $1 $2 $3 | sed '4 w result.txt'
./scripts/date.sh >> date.cal
printf "======= bc =========\n"
VAL=`cat result.txt`
rm result.txt
printf " $VAL is "
test `printf " $VAL\n" | bc` -eq 1 && printf "true\n" || printf "false\n"
printf " $1 + $2 = `printf "scale=$3;$1 + $2\n" | bc`\n"
printf " $1 - $2 = `printf "scale=$3;$1 - $2\n" | bc`\n"
printf " $1 * $2 = `printf "scale=$mul;$1 * $2\n" | bc`\n"
printf " $1 / $2 = `printf "scale=$3;$1 / $2\n" | bc`\n"
printf " $1 %% $2 = `printf "scale=$3;$1 %% $2\n" | bc`\n"
./scripts/date.sh > date.bc
printf " $1 ^ $2 = `printf "scale=$3;$1 ^ $2\n" | bc`\n"
./scripts/date.sh >> date.bc
mv date.cal date.comp
echo "./main-bin: $(./scripts/date-comp.sh)"
mv date.bc date.comp
echo "bc: $(./scripts/date-comp.sh)"
rm date.comp
