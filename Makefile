TARGET	=	lem-ipc
CC		=	gcc
CFLAGS	=	-g -lncurses #-Wall -Wextra -Werror
RM		=	rm -rf
SRC		=	$(wildcard ./src/*.c)
OBJ		=	$(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

.PHONY: all clean fclean re
