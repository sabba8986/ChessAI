#define PY_SSIZE_T_CLEAN
#include<python3.10/Python.h>
#include<wchar.h>
#include<stdint.h>


extern uint64_t generate_all_moves(uint8_t pos);
extern void make_move(uint8_t start, uint8_t end);
extern int8_t check();
extern int8_t checkmate();
extern uint64_t get_king_pos(int is_white);
extern void reset();


static PyObject* py_generate_all_moves(PyObject* self, PyObject* args){
    uint8_t pos;
    if(!PyArg_ParseTuple(args, "b", &pos)){
        return NULL;
    }
    
    uint64_t result = generate_all_moves(pos);
    return PyLong_FromUnsignedLongLong(result);
}


static PyObject* py_make_move(PyObject* self, PyObject* args){
    uint8_t start, end;
    if(!PyArg_ParseTuple(args, "bb", &start, &end)){
        return NULL;
    }
    make_move(start, end);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject* py_check(PyObject* self, PyObject* args){
    return PyLong_FromLong(check());
}


static PyObject* py_checkmate(PyObject* self, PyObject* args){
    return PyLong_FromLong(checkmate());
}

static PyObject* py_get_king_pos(PyObject* self, PyObject* args){
    int is_white;
    if(!PyArg_ParseTuple(args, "p", &is_white)){
        return NULL;
    }
    uint64_t king_pos = get_king_pos(is_white);
    return PyLong_FromUnsignedLongLong(king_pos);
}

static PyObject* py_reset(PyObject* self, PyObject* args){
    reset();
    Py_INCREF(Py_None);
    return Py_None;
}


static PyMethodDef api_funcs[] = {
    {"generate_all_moves", py_generate_all_moves, METH_VARARGS, "Returns the list of all possible moves made by the piece at position pos.",},
    {"make_move", py_make_move, METH_VARARGS, "Moves the piece at position start to position end. This method assumes the move is valid."},
    {"check", py_check, METH_VARARGS, "Returns whether either king is in check (1 for white king, 0 for black king), or -1 if there is no check."}, 
    {"checkmate", py_checkmate, METH_VARARGS, "Returns whether either king is in checkmate (1 for white king, 0 for black king), or -1 if there is no checkmate."},
    {"get_king_pos", py_get_king_pos, METH_VARARGS, "Returns the bitboard position of the king of the specified color."},
    {"reset", py_reset, METH_VARARGS, "Resets the board to its initial state and player turn."},
    {NULL, NULL, 0, NULL}
};


static PyModuleDef api_module = {
    PyModuleDef_HEAD_INIT,
    "chess_engine", 
    NULL, 
    -1, 
    api_funcs
};


PyMODINIT_FUNC PyInit_chess_engine(){
    return PyModule_Create(&api_module);
}


