#!/bin/bash

set -euo pipefail

echo "=========================================="
echo "Cleaning environment: make clean"
echo "=========================================="
make clean

echo "=========================================="
echo "Testing CTest: make test"
echo "=========================================="

if make test; then
	make clean
    echo "------------------------------------------"
    echo "STATUS: SUCCESS "
    echo "------------------------------------------"
    exit 0
else
	make clean
    echo "------------------------------------------" >&2
    echo "STATUS: FAILURE " >&2
    echo "------------------------------------------" >&2
    exit 1
fi
