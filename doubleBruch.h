#include<string>
#include<sstream>
#include<vector>
#include<math.h>

#ifndef HEADER_doubleBruch_H
#define HEADER_doubleBruch_H

//das zeugs ist inhaltlich irrelevant f�r den rest. das macht das nur sch�ner mit der ein und ausgabe.

double BruchStrZuDouble(std::string &name);

std::pair<int, int> dtoB(double wert, int fehler);  //eigentlich nur interne funktion f�r DoubleZuBruchStr

std::string DoubleZuBruchStr(double wert, bool showpos);

std::string druckenfloat(double wert, bool showpos);




#endif //HEADER_doubleBruch_H
