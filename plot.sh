#! /bin/bash

for fl in *.dot
{
    dot -Tpng $fl -o ${fl}.png
    echo "processed ${fl}"
}
