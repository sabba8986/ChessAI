CC = clang++
CFLAGS =  -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all -Wextra -std=c++20

all: chess

chess: Board.o main.o move_gen_t.o move_gen.o 
	@$(CC) $(CFLAGS) -o chess -fuse-ld=lld Board.o main.o move_gen_t.o move_gen.o

Board.o: Board.cpp Board.hpp Piece.hpp info.hpp move_gen.hpp
	@$(CC) $(CFLAGS) -c Board.cpp

main.o: main.cpp Board.hpp move_gen_t.hpp
	@$(CC) $(CFLAGS) -c main.cpp

move_gen_t.o: defines.hpp capture.hpp move_gen_t.hpp
	@$(CC) $(CFLAGS) -c move_gen_t.cpp

move_gen.o: defines.hpp capture.hpp Piece.hpp move_gen.hpp
	@$(CC) $(CFLAGS) -c move_gen.cpp


clean:
	@rm *.o chess
