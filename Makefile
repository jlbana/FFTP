SRCDIR	:= src/
OBJDIR	:= build/
SRC	:= $(wildcard $(SRCDIR)/*.c)
OBJ	:= $(SRC:.c=.o)
CFLAGS	:= -fshort-enums -Wall -O
LDFLAGS	:= -lpthread -lcrypt
CC	:= gcc
BIN	:= fftp

all:	$(OBJ)
	@$(CC) $(subst $(SRCDIR),$(OBJDIR),$^) -o $(addprefix $(OBJDIR),$(BIN)) $(LDFLAGS)
	@rm -rf build/*.o

%.o: %.c
	@$(CC) -c $< -o $(subst $(SRCDIR),$(OBJDIR),$@) $(CFLAGS)

