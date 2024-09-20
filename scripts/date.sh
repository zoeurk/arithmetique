#!/bin/sh
DATE=`date "+%M:%S:%N"`
MIN=${DATE%%:*}
SEC=${DATE%:*}
SEC=${SEC##*:}
NANO=${DATE#$MIN:$SEC:}
echo "$MIN*60*1000000000+$SEC*1000000000+$NANO"
