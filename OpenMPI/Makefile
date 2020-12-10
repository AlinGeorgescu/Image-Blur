# Proiect APP
# Georgescu Alin-Andrei 342 C3
# Iuga Florin-Eugen     342 C5
# Negru Bogdan-Crisitan 342 C3

.PHONY: build run pack clean

# Compilator
CC = mpicc

# Flaguri compilator
CFLAGS = -Wall -Wextra -std=c99

# Biblioteci externe
LIBS=-lm

# Sursa
SRC = blur_openmpi.c

# Numele executabilului
EXE = blur

# Regula build
build: $(SRC)
	$(CC) $(CFLAGS) $^ -o $(EXE) $(LIBS)

# Regula de rulare
run: build
	mpirun -np $(TASKS) ./$(EXE) $(IN) $(OUT) $(FILTERS)

# Curatare director curent
clean:
	rm -f *.o $(EXE)
