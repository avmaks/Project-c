#!/bin/bash
COUNTER_SUCCESS=0
COUNTER_FAIL=0
TEST_FILE1="1.txt"
TEST_FILE2="1.txt 2.txt 3.txt"
PATTERN1="123"
PATTERN2="testi"
PATTERN_FILE="patterns.txt"

flags_array=(-e -i -v -c -l -n -h -s -f -o)

echo "Testing processing. Wait..."

for var in ${flags_array[@]}
do
    if [ "$var" == "-f" ]
    then
        TEST1="$var $PATTERN_FILE $TEST_FILE1"
    else
        TEST1="$var $PATTERN1 $TEST_FILE1"
    fi
    ./s21_grep $TEST1 > s21_grep1.txt
    grep $TEST1 > grep1.txt
    DIFF_RES1="$(diff -s s21_grep1.txt grep1.txt)"
    if [ "$DIFF_RES1" == "Files s21_grep1.txt and grep1.txt are identical" ]
    then
        ((COUNTER_SUCCESS++))
    else
        echo "Test case $var with TEST_FILE1 failed"
        ((COUNTER_FAIL++))
    fi

    if [ "$var" == "-f" ]
    then
        TEST2="$var $PATTERN_FILE $TEST_FILE2"
    else
        TEST2="$var $PATTERN2 $TEST_FILE2"
    fi
    ./s21_grep $TEST2 > s21_grep2.txt
    grep $TEST2 > grep2.txt
    DIFF_RES2="$(diff -s s21_grep2.txt grep2.txt)"
    if [ "$DIFF_RES2" == "Files s21_grep2.txt and grep2.txt are identical" ]
    then
        ((COUNTER_SUCCESS++))
    else
        echo "Test case $var with TEST_FILE2 failed"
        ((COUNTER_FAIL++))
    fi
done

rm s21_grep1.txt grep1.txt s21_grep2.txt grep2.txt

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"