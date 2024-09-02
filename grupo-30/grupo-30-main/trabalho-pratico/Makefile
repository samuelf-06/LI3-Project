SOURCE  = src/queries.c  src/teste.c src/catalog.c src/date.c src/flight.c src/passenger.c src/prints.c scr/printsterminal.c src/reservation.c src/user.c src/validator.c src/main.c
HEADER  = includes/queries.h includes/catalog.h includes/date.h includes/flight.h includes/prints.h includes/printsterminal.h includes/user.h includes/reservation.h includes/validator.h
OUT     = programa-principal
TESTES_OUT = programa-testes
CC      = gcc
GLIBCFLAGS := `pkg-config --cflags glib-2.0`
FLAGS      =  $(GLIBCFLAGS) -g -Wall
LFLAGS    := `pkg-config --libs glib-2.0` -lcurses -O3

OBJS := date.o user.o flight.o passenger.o prints.o printsterminal.o reservation.o validator.o catalog.o main.o queries.o
TESTE_OBJ := teste.o

all: programa-principal programa-testes

programa-principal: $(filter-out $(TESTE_OBJ), $(OBJS))
	$(CC) $(FLAGS) $(filter-out $(TESTE_OBJ), $(OBJS)) $(LFLAGS) -o $(OUT)

programa-testes: $(TESTE_OBJ) $(filter-out main.o, $(OBJS))
	$(CC) $(FLAGS) $(TESTE_OBJ) $(filter-out main.o, $(OBJS)) $(LFLAGS) -o $(TESTES_OUT)

main.o: src/main.c 
	$(CC) $(FLAGS) -c src/main.c -std=gnu17

teste.o: src/teste.c
	$(CC) $(FLAGS) -c src/teste.c -std=gnu17

queries.o: src/queries.c
	$(CC) $(FLAGS) -c src/queries.c -std=gnu17

catalog.o: src/catalog.c
	$(CC) $(FLAGS) -c src/catalog.c -std=gnu17

date.o: src/date.c
	$(CC) $(FLAGS) -c src/date.c -std=gnu17

flight.o: src/flight.c 
	$(CC) $(FLAGS) -c src/flight.c -std=gnu17

passenger.o: src/passenger.c 
	$(CC) $(FLAGS) -c src/passenger.c -std=gnu17

prints.o: src/prints.c
	$(CC) $(FLAGS) -c src/prints.c -std=gnu17

printsterminal.o: src/printsterminal.c
	$(CC) $(FLAGS) -c src/printsterminal.c -std=gnu17

reservation.o: src/reservation.c 
	$(CC) $(FLAGS) -c src/reservation.c -std=gnu17

user.o: src/user.c 
	$(CC) $(FLAGS) -c src/user.c -std=gnu17

validator.o: src/validator.c 
	$(CC) $(FLAGS) -c src/validator.c -std=gnu17

clean:
	rm -f $(OBJS) $(TESTE_OBJ) $(OUT) $(TESTES_OUT)