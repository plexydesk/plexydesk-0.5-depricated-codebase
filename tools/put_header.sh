#!/bin/sh

for x in `ls *.cpp *.h`;
do
cat $x > temp;
cat header.template > $x;
cat temp >> $x;
rm temp;
echo "updated file : $x";
done