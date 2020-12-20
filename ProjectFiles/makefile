# change application name here (executable output name)
TARGET=OCR-app

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall -Wextra -Werror -std=c99

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

SDLLIB=`sdl2-config --cflags --libs`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) $(SDLLIB) -lSDL2_image -lSDL2_gfx -export-dynamic -rdynamic -lm

OBJS=    src/.o/main.o src/.o/imageprocessing.o src/.o/extraction.o src/.o/extractMethods.o src/.o/matrice.o src/.o/NN.o src/.o/nn_tools.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
src/.o/main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) $(SDLLIB) -o src/.o/main.o    
src/.o/imageprocessing.o: src/imageprocessing.c
	$(CC) -c $(CCFLAGS) src/imageprocessing.c $(GTKLIB) $(SDLLIB) -o src/.o/imageprocessing.o
src/.o/extraction.o: src/extraction/extraction.c
	$(CC) -c $(CCFLAGS) src/extraction/extraction.c $(GTKLIB) $(SDLLIB) -o src/.o/extraction.o
src/.o/extractMethods.o: src/extraction/extractMethods.c
	$(CC) -c $(CCFLAGS) src/extraction/extractMethods.c $(GTKLIB) $(SDLLIB) -o src/.o/extractMethods.o
src/.o/matrice.o: src/matrice/matrice.c
	$(CC) -c $(CCFLAGS) src/matrice/matrice.c $(GTKLIB) $(SDLLIB) -o src/.o/matrice.o
src/.o/NN.o: src/neural_network/NN.c
	$(CC) -c $(CCFLAGS) src/neural_network/NN.c $(GTKLIB) $(SDLLIB) -o src/.o/NN.o
src/.o/nn_tools.o: src/neural_network/nn_tools.c
	$(CC) -c $(CCFLAGS) src/neural_network/nn_tools.c $(GTKLIB) $(SDLLIB) -o src/.o/nn_tools.o
clean:
	rm -f src/.o/*.o $(TARGET) -r cache/*