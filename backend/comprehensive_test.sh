#!/bin/bash

echo "Comprehensive Solver Tests"
echo "="*60

echo -e "\n1. Solved cube"
echo "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB" | ./solver

echo -e "\n2. After U move"
echo "UUUUUUUUUBBBRRRRRRRRRFFFFFFDDDDDDDDDFFFLLLLLLLLLBBBBBB" | ./solver

echo -e "\n3. After R move"
echo "UUFUUFUUFRRRRRRRRRFFDFFDFFDDDBDDBDDBLLLLLLLLLUBBUBBUBB" | ./solver

echo -e "\n4. After F move"
# Need to compute F move correctly
