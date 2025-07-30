#!/bin/bash
echo ----------------------------------------------*CAT 1
diff <(cat -b test_files/test_1_cat.txt) <(./s21_cat -b test_files/test_1_cat.txt) -s
echo ----------------------------------------------*CAT 2
diff <(cat -e "test_files/test_2_cat.txt") <(./s21_cat -e "test_files/test_2_cat.txt") -s
echo ----------------------------------------------*CAT 3
diff <(cat -n "test_files/test_3_cat.txt") <(./s21_cat -n "test_files/test_3_cat.txt") -s
echo ----------------------------------------------*CAT 4
diff <(cat -s "test_files/test_5_cat.txt") <(./s21_cat -s "test_files/test_5_cat.txt") -s
echo ----------------------------------------------*CAT 5
diff <(cat -t "test_files/test_3_cat.txt") <(./s21_cat -t "test_files/test_3_cat.txt") -s
echo ----------------------------------------------*CAT 6
diff <(cat -v "test_files/test_case_cat.txt") <(./s21_cat -v "test_files/test_case_cat.txt") -s
echo ----------------------------------------------*CAT 7
diff <(cat --number-nonblank "test_files/test_5_cat.txt") <(./s21_cat --number-nonblank "test_files/test_5_cat.txt") -s
echo ----------------------------------------------*CAT 8
diff <(cat --number "test_files/test_3_cat.txt") <(./s21_cat --number "test_files/test_3_cat.txt") -s
echo ----------------------------------------------*CAT 9
diff <(cat --squeeze-blank "test_files/test_case_cat.txt") <(./s21_cat --squeeze-blank "test_files/test_case_cat.txt") -s