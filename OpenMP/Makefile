# Proiect APP
# Georgescu Alin-Andrei 342 C3
# Iuga Florin-Eugen     342 C5
# Negru Bogdan-Crisitan 342 C3

.PHONY: build run pack clean

# Compilator
CC = gcc

# Flaguri compilator
CFLAGS = -Wall -Wextra -std=c99

# Biblioteci externe
LIBS=-lm -fopenmp

# Sursa
SRC = blur_openmp.c

# Numele executabilului
EXE = blur

# Regula build
build: $(SRC)
	$(CC) $(CFLAGS) $^ -o $(EXE) $(LIBS)

# Regula de rulare
run: build
	./$(EXE) $(IN) $(OUT) $(FILTERS)

# Curatare director curent
clean:
	rm -f *.o $(EXE)
