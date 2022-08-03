CC = /usr/bin/c++ 
CFLAGS = -m32 -std=c++20
WFLAGS = -Wall -Wextra -Wpedantic \
		-Wconversion -Wsign-conversion -Wdouble-promotion \
		-Wcast-qual -Wold-style-cast -Wuseless-cast \
        -Wnull-dereference -Wzero-as-null-pointer-constant -Wstrict-null-sentinel \
        -Weffc++ -Wimplicit-fallthrough=5 -Wsuggest-override \
        -Wduplicated-branches -Wduplicated-cond \
        -Wshadow -Woverloaded-virtual -Wlogical-op -Wformat=2 -Wundef -Wno-long-long

all: test.o
	$(CC) -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wdouble-promotion -Wcast-qual -Wold-style-cast -Wuseless-cast -Wnull-dereference -Wzero-as-null-pointer-constant -Wstrict-null-sentinel -Weffc++ -Wimplicit-fallthrough=5 -Wsuggest-override -Wduplicated-branches -Wduplicated-cond -Wshadow -Woverloaded-virtual -Wlogical-op -Wformat=2 -Wundef -Wno-long-long -Og -m32 object.o -o RodosPlayground_test  /usr/local/lib/libCatch2Main.a /usr/local/lib/libCatch2.a

test.o: Test/Source/test.cpp
	$(CC) Test/Source/test.cpp -c -o object.o $(CFLAGS) $(WFLAGS) 
