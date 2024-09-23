CC = clang++
CFLAGS =  -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all -Wextra -std=c++20

all: chess

chess: Board.o main.o move_gen_t.o move_gen.o attack_tables.o Moves.o Indices.o 
	@$(CC) $(CFLAGS) -o chess -fuse-ld=lld Board.o main.o move_gen_t.o move_gen.o attack_tables.o Moves.o Indices.o

Board.o: Board.cpp Board.hpp Piece.hpp PieceInfo.hpp move_gen.hpp
	@$(CC) $(CFLAGS) -c Board.cpp

main.o: main.cpp Board.hpp move_gen_t.hpp
	@$(CC) $(CFLAGS) -c main.cpp

move_gen_t.o: move_gen_t.cpp defines.hpp capture.hpp move_gen_t.hpp
	@$(CC) $(CFLAGS) -c move_gen_t.cpp

move_gen.o: move_gen.cpp defines.hpp capture.hpp Piece.hpp move_gen.hpp
	@$(CC) $(CFLAGS) -c move_gen.cpp

attack_tables.o: attack_tables.cpp Moves.hpp BasicPiece.hpp
	@$(CC) $(CFLAGS) -c attack_tables.cpp

Moves.o: Moves.cpp Moves.hpp Indices.hpp Color.hpp
	@$(CC) $(CFLAGS) -c Moves.cpp

Indices.o: Indices.hpp defines.hpp
	@$(CC) $(CFLAGS) -c Indices.cpp

clean:
	@rm *.o chess
