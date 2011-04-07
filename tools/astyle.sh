#!/bin/bash

#script that fix headers and cpp files recursively.

VERBOSE=1

function debug
{
  msg=$1
  if [ "$VERBOSE" == "1" ]; then
    echo $msg
  fi
}

function fix
{
  fname=$1
  nfixedo=0
  case $(echo ${fname#*.}) in
    h) debug "[fix header] $fname"
    uncrustify  -c ~/plex.cfg  -f $fname  -o $fname > /dev/null
       nfixedo=`expr $nfixedo + 1`
       ;;
    cpp) debug "[fix source] $fname"
         uncrustify  -c ~/plex.cfg  -f $fname  -o $fname > /dev/null
         nfixedo=`expr $nfixedo + 1`
         ;;
    *) debug "omiting $fname"
       ;;
  esac
 return ${nfixedo}
}

function walk
{
  for dir in `echo *`; do
    if [ -d $dir ]; then
      numdirs=`expr $numdirs + 1`
      if cd $dir; then
        debug "[dir] $PWD"
        walk 
        cd ..
      fi
    elif [ -f $dir ]; then
      fix $dir
      nfixed=`expr $nfixed + $?`
    fi
  done
  
}

walk 0
echo "Folders visited: $numdirs"
echo "Files fixed: $nfixed"
