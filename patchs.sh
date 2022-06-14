#!/bin/sh
if [ "$1" = "I" ]
then
	for P in $(ls patchs/*.patch)
	do
		FILE=`echo $P | sed -e 's#^.*/\(.*\)\.patch#\1/\1.c#' -e 's/^/exemple\//'`
		echo "patch -i $P $FILE"
	done
elif [ "$1" = "R" ]
then
	for P in $(ls patchs/*.patch)
	do
		FILE=`echo $P | sed -e 's#^.*/\(.*\)\.patch#\1/\1.c#' -e 's/^/exemple\//'`
		echo "patch -R -i $P $FILE"
	done
fi
