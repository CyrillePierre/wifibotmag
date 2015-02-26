
# mettre "yes" pour activer le débuggage
DEBUG		= no

EXEC		= $(notdir $(shell pwd))

CXX			= g++
STANDARD	= -std=c++11
CFLAGS		= -Wall -pedantic -Wextra 
LDFLAGS		= -lSDL2 -lpthread

OBJ_DIR     = obj
DEPENDS		= .depends
SRC			= $(wildcard *.cpp)
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
SHELL		= bash

.SUFFIXES: .cpp .hpp .o

ifeq ($(DEBUG), yes)
	CFLAGS += -g -pg -O0
else
	CFLAGS += -O2
endif

### MEMO ###
#   $@  :   nom de la cible
#   $<  :   nom de la première dépendance
#   $^  :   liste des dépendances
#   $+  :   liste des dépendances sans doublons 
#   $?  :   liste des dépendances plus récente que la cible

all : target $(EXEC)

$(EXEC) : $(OBJ)
	@echo -e "\e[01;39mCompilation du programme $@..."
	@echo -en "\e[01;31m"
	@if ! $(CXX) $+ -o $@ $(LDFLAGS); then \
		echo -en "\e[00;39m"; false; \
	else \
		echo -e "\e[01;32m\nCompilation terminée avec succes !"; \
		echo -e "\e[00m"; fi

$(OBJ_DIR)/%.o : %.cpp
	@echo -e "\E[01;39mCompilation de $<...\E[00;39m"
	@echo -en "\E[01;31m"
	@if ! $(CXX) $(STANDARD) $(CFLAGS) -c $< -o $@; then \
		echo -en "\E[00;39m"; false; fi

target :
	@mkdir -p $(OBJ_DIR)

clean :
	rm -f $(EXEC) $(OBJ)

dep : $(SRC)
	$(CXX) $(STANDARD) -MM $+ | sed -e 's/\(.*\)/$(OBJ_DIR)\/\1/' > $(DEPENDS)

.PHONY : clean

sinclude $(DEPENDS)
