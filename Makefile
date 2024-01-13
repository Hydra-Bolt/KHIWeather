CC=gcc
CFLAGS=-Wall
LIBS=-lcurl -lcjson -lplplot -lncurses -lhpdf

SRC=main.c apiparams.c analysis.c plotting.c emailanomalies.c processdata.c weathercast.c
OBJ=$(SRC:.c=.o)
EXEC=main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)
