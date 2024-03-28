CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Og
LDFLAGS =
EXEC = ShinyModifier.exe
OBJECTS = ShinyModifier.o

.SILENT:
${EXEC}: ${OBJECTS}
	${CC} ${LDFLAGS} ${OBJECTS} -o ${EXEC}

%.o: %.c
	${CC} ${CFLAGS} -c $*.c

.PHONY: clean
clean:
	-del ${EXEC} ${OBJECTS}
