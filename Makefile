TARGET	=	lem-ipc
CC		=	gcc
CFLAGS	=	-g -lncurses #-Wall -Wextra -Werror
RM		=	rm -rf
SRC		=	$(wildcard ./src/*.c)
OBJ		=	$(SRC:.c=.o)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

all: $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

.PHONY: all clean fclean re
