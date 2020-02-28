#!/bin/bash

## delete long base 

if [ "$#" -ne 1 ]; then
    echo "ERROR: Missing parameter."
    echo "./remove_long_base <path/to/Grammar.txt>"
    exit 1
fi

input_grammar="$1"

function delete_long_base() {
    max_base_len="$1"
    while read -r line; do
        dual=($line)
        base=${dual[0]}
	prob=${dual[1]}
        len=${#base}
    
	if [ $len -le $max_base_len ]; then
	    echo -e "${base}\t${prob}"
	fi
    done < "$input_grammar"
}

delete_long_base 10 > tmp.txt
mv tmp.txt $input_grammar
