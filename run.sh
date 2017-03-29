#!/bin/sh

clear
rm som
gcc -std=c99 -pedantic -Wall main.c data.c neural_map.c formulas.c -lm -o som
if [ $? -ne 0 ]
then 
    echo 'Compilation failure';
    exit 1
else 
    echo 'Compilation suceeded';
fi

./som input.txt
