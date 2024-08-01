from PyQt6.QtWidgets import QApplication, QVBoxLayout, QWidget, QMessageBox
from PyQt6.QtGui import QFont
from . import Tile
import chess_engine
import sys



BITBOARD_TOP_LEFT = 9223372036854775808


def bitboard_to_index(bitboard):
    counter = 0
    while((bitboard & BITBOARD_TOP_LEFT) == 0):
        counter += 1 
        bitboard <<= 1 
    return counter



class Window(QWidget): 
   
    def reset_board(self):
            self.highlighted_tiles = 0
            self.cur_selected_square = -1
            self.is_selected = False
            self.pieces_mask = 18446462598732906495
            for tile in self.tiles:
                tile.reset_icon()
                tile.reset_color()

    def __init__(self):
        super().__init__() 
        self.resize(800, 800)
        self.setWindowTitle("Chess")
        self.setLayout(QVBoxLayout())
        self.tiles = []
        self.reset_board()
        for i in range(8):
            for j in range(8):
                self.tiles.append(Tile.Tile(self, i, j))
    
    def get_piece_locations(self):
            return self.pieces_mask 

    def clear_highlighted_squares(self):
        for tile in self.tiles:
            tile.reset_color()
        self.highlighted_tiles = 0

    def highlight_moves(self, pos):   
        if(self.is_selected and ((BITBOARD_TOP_LEFT >> pos) & self.highlighted_tiles) != 0): 
            chess_engine.make_move(self.cur_selected_square, pos) 
            start_tile = self.tiles[self.cur_selected_square]
            end_tile = self.tiles[pos]
            end_tile.setIcon(start_tile.icon())
            start_tile.set_empty_icon()
            self.clear_highlighted_squares()
            self.is_selected = False
            self.pieces_mask &= ~(BITBOARD_TOP_LEFT >> self.cur_selected_square)
            self.pieces_mask |= (BITBOARD_TOP_LEFT >> pos) 
            check = chess_engine.check()
            checkmate = chess_engine.checkmate()
            if(checkmate != -1):
                self.show_end_message(True if checkmate == 1 else False)
            elif(check != -1):
                king_button = self.tiles[bitboard_to_index(chess_engine.get_king_pos(True if check == 1 else 0))]
                king_button.highlight_check()
        else:
            self.clear_highlighted_squares()
            self.is_selected = True
            self.cur_selected_square = pos
            self.highlighted_tiles = chess_engine.generate_all_moves(pos)
            for tile in self.tiles:
                if(((BITBOARD_TOP_LEFT >> tile.get_position()) & self.highlighted_tiles) != 0):
                    tile.highlight()



    def show_end_message(self, is_white):
        king_button = self.tiles[bitboard_to_index(chess_engine.get_king_pos(not is_white))]
        king_button.highlight_checkmate()

        game_over = QMessageBox(self.window())
        game_over.setWindowTitle("Game Over")
        game_over.setFont(QFont('Arial', 20))
        game_over.resize(600, 400)
        game_over.setText("{color} won!".format(color = "White" if is_white else "Black"))
        game_over.addButton("Play Again", QMessageBox.ButtonRole.AcceptRole)
        game_over.addButton("Exit", QMessageBox.ButtonRole.RejectRole)
        result = game_over.exec()
        if(result == 0):
            self.reset_board()
            chess_engine.reset()
        else:
            self.window().close()






app = QApplication(sys.argv)
window = Window()
window.show()
sys.exit(app.exec())



