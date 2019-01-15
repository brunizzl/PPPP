#include<string>
#include<sstream>
#include<vector>
#include<math.h>

#ifndef HEADER_doubleBruch_H
#define HEADER_doubleBruch_H

///das zeugs ist inhaltlich irrelevant für den rest. das macht das nur schöner mit der ein und ausgabe.

double BruchStrZuDouble(std::string &name);

std::pair<int, int> dtoB(double wert, int fehler);  ///eigentlich nur interne funktion für DoubleZuBruchStr

std::string DoubleZuBruchStr(double wert, bool showpos);

std::string druckenfloat(double wert, bool showpos);




#endif //HEADER_doubleBruch_H
