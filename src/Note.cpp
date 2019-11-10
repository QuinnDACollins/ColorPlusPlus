#include <iostream>
#include <vector>
#include <string>
#include <Note.h>


Note::Note(){}
    /* data */

Note::Note(std::string _noteName, int _degree, int _idx){
    noteName = _noteName;
    degree = _degree;
    idx = _idx;
}

