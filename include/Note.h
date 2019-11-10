// Note.h
#include <string>
#include <vector>
#ifndef Note_H
#define Note_H

class Note
{

public:
  std::string noteName;
  int degree;
  int idx;
  Note();
  Note(std::string _noteName, int _degree, int _idx);


};

#endif