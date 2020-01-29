all: find

find : find.o kmp.o struct.o test.o
	gcc -o find find.o kmp.o struct.o test.o -g -Wall -Wextra

find.o : ./include/find.h ./src/find.c
	gcc -c ./src/find.c -g -Wall -Wextra

kmp.o : ./include/kmp.h ./src/kmp.c
	gcc -c ./src/kmp.c -g -Wall -Wextra

struct.o : ./include/struct.h ./src/struct.c
	gcc -c ./src/struct.c -g -Wall -Wextra

test.o : ./include/test.h ./src/test.c
	gcc -c ./src/test.c -g -Wall -Wextra
