#!/bin/bash
set -e

echo "=== Running tch Advanced Flag Tests ==="

make clean
make

TEST_DIR="build/sandbox"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"
TCH_BIN="../../build/tch"

echo "[Test 1/3] Testing help flag..."
$TCH_BIN --help | grep -q "Usage: tch"
echo "  -> Pass: Help menu output verified."

echo "[Test 2/3] Verification of path creation with a flag..."
$TCH_BIN -m flag_test/nested_file.txt
if [ -f "flag_test/nested_file.txt" ]; then
    echo "  -> Pass: File created successfully even when passing flags."
else
    echo "  -> Fail: Flag blocks file creation."
    exit 1
fi

echo "=== All Flag Tests Passed Successfully! ==="
