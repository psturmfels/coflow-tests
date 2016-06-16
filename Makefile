CC=g++
CFLAGS=-std=c++14 -Wall -Werror -Wvla -Wextra -pedantic -O3 
CFILES=open_shop.cpp coflow.cpp

coflow_tests:
	$(CC) $(CFLAGS) $(CFILES) coflow_tests.cpp -o coflow_tests

open_shop_tests:
	$(CC) $(CFLAGS) open_shop.cpp open_shop_tests.cpp -o open_shop_tests

clean:
	rm -f *.o coflow_tests open_shop_tests
