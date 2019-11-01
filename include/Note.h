// Note.h
#include <string>
#ifndef Note_H
#define Note_H

class Note
{

public:
  std::string note;
  std::string strScale[7];
  Note(std::string note, std::string strScale[7]);


};

#endif