# Nom de l'exécutable
TARGET = space_invaders

# Dossiers
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# Fichiers sources et objets
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES)) $(OBJ_DIR)/main.o

# Commandes et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I$(INCLUDE_DIR) `pkg-config --cflags sdl2 SDL2_image SDL2_ttf`
LDFLAGS = `pkg-config --libs sdl2 SDL2_image SDL2_ttf`
ARCH = -arch arm64

# Cible principale
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJ_FILES)
	$(CC) $(ARCH) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(ARCH) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c
	$(CC) $(ARCH) $(CFLAGS) -c main.c -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild complet
re: clean all

.PHONY: clean re