# Proiect APD
# Georgescu Alin-Andrei 342 C3
# Iuga Florin-Eugen     342 C5
# Negru Bogdan-Crisitan 342 C3

.PHONY: build run pack clean

# Compilator
CC = gcc

# Flaguri compilator
CFLAGS = -Wall -Wextra -std=c99

# Biblioteci externe
LIBS=-lm -lpthread

# Sursa
SRC = blur_pthreads.c

# Numele executabilului
EXE = blur

# Regula build
build: $(SRC)
	$(CC) $(CFLAGS) $^ -o $(EXE) $(LIBS)

# Regula de rulare
run: build
	./$(EXE) $(IN) $(OUT) $(FILTERS) $(NUM_THREADS)

# Curatare director curent
clean:
	rm -f *.o $(EXE)
