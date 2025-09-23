gcc -o test_link example.c -L../lib -lgosiuml -I../include -pthread
LD_LIBRARY_PATH=../lib ./test_link
