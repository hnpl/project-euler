#!/bin/bash
SAGE=$HOME/SageMath/sage

g++ problem367.cpp -O3 -o problem367.e
./problem367.e
eval "$SAGE problem367.py"
rm p367_count.txt
rm problem367.e
