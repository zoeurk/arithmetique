#!/bin/sh
if [ $1 = ""]
then
	for P in $(ls patchs/*.patch)
	do
		FILE=`echo $P | sed 's#^.*/\(.*\)\.patch#\1/\1.c#'`
		echo "patch -i $P $FILE"
	done
elif [ $1 = "R" ]
then
	for P in $(ls patchs/*.patch)
	do
		FILE=`echo $P | sed 's#^.*/\(.*\)\.patch#\1/\1.c#'`
		echo "patch -i $P $FILE"
	done
fi
