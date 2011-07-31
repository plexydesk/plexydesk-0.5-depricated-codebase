#!/bin/sh

FILE_NAMES=""
FILE_NAMES_LONG=""
RESULT=""

SAVEIFS=$IFS
IFS=$';'

for a in `echo "$@"`; do
    FILE_NAMES="${FILE_NAMES} `basename \"$a\"`"
    FILE_NAMES_LONG="${FILE_NAMES_LONG}$a\n"
done

IFS=$SAVEIFS

RESULT=$( echo ${FILE_NAMES} | sed -e "sQ\\(\..\{3\}\\) Q\1\nQg" | sort | uniq -D | uniq )

if [ -n "${RESULT}" ]; then
    for b in "${RESULT}"; do
        echo "${FILE_NAMES_LONG}" | grep "/${b}"
    done
fi
