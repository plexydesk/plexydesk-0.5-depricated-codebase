#!/bin/sh

FILE_NAMES=""
FILE_NAMES_LONG=""
RESULT=""

SAVEIFS=$IFS
IFS=$';'

for a in `echo "$@"`; do
    FILE_NAMES="`basename \"$a\"` ${FILE_NAMES}"
    FILE_NAMES_LONG="$a\n${FILE_NAMES_LONG}"
done

IFS=$SAVEIFS

RESULT=$( echo ${FILE_NAMES} | sed -r "s/(\..{3}) /\1\n/g" | sort | uniq -D | uniq )

if [ -n "${RESULT}" ]; then
    for b in "${RESULT}"; do
        echo "${FILE_NAMES_LONG}" | grep "/${b}"
    done
fi
