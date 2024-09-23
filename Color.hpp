#ifndef COLOR
#define COLOR

enum Color{
    BLACK = 0, 
    WHITE = 1 
};


inline Color operator ~(Color color){
    return color == Color::WHITE ? Color::BLACK : Color::WHITE;
}


#endif
