from PyQt6.QtWidgets import QApplication, QVBoxLayout, QWidget,  QPushButton
from PyQt6.QtGui import QIcon
from PyQt6.QtCore import QSize
import sys



BITBOARD_TOP_LEFT = 9223372036854775808


class Tile(QPushButton):
    TILE_COLOR_MASK = 6172840429334713770
    INITIAL_LAYOUT = "rnbqkbnrpppppppp________________________________PPPPPPPPRNBQKBNR"
    ICONS = {} 

    def __init__(self, Window, i, j):
        super().__init__(None, Window)
        Tile.ICONS = {"_":QIcon("icons/Empty.png"), "r":QIcon("icons/black_rook.png"), "n":QIcon("icons/black_knight.png"), 
             "b":QIcon("icons/black_bishop.png"), "q":QIcon("icons/black_queen.png"), "k":QIcon("icons/black_king.png"), 
             "p":QIcon("icons/black_pawn.png"), "R":QIcon("icons/white_rook.png"), "N":QIcon("icons/white_knight.png"), 
             "B":QIcon("icons/white_bishop.png"), "Q":QIcon("icons/white_queen.png"), "K":QIcon("icons/white_king.png"), 
             "P":QIcon("icons/white_pawn.png")} 
        
        tile_no = 8*i + j
        self.position = tile_no
        self.reset_color()
        self.resize(100, 100)
        self.setIconSize(QSize(100, 100))
        self.reset_icon()
        self.move(100*j, 100*i)
        self.clicked.connect(lambda temp, ind = tile_no: self.window().highlight_moves(ind))
 
    def reset_icon(self):
        self.setIcon(Tile.ICONS[Tile.INITIAL_LAYOUT[self.position]])
 
    def set_empty_icon(self):
        self.setIcon(Tile.ICONS["_"])

    def reset_color(self):
        if((BITBOARD_TOP_LEFT >> self.position) & Tile.TILE_COLOR_MASK):
            self.set_color("#B88B4A")
        else:
            self.set_color("#E3C16F") 

    def set_color(self, color):
        self.setStyleSheet("background-color:{f_color}; border: #000000; border-width: thin".format(f_color = color))

    def highlight(self):
        if((BITBOARD_TOP_LEFT >> self.position) & self.window().get_piece_locations()):
            self.setStyleSheet("background-color:#FF0000; border: 1px solid black")
        else:
            self.setStyleSheet("background-color:#FFFF00; border: 1px solid black") 

    def highlight_check(self):
        self.setStyleSheet("background-color: #ffA500; border: 1px solid black")

    def highlight_checkmate(self):
        self.setStyleSheet("background-color: #FF0000; border: 1px solid black")

    def get_position(self):
        return self.position
    




