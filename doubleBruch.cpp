#include "doubleBruch.h"

double BruchStrZuDouble(std::string &name) {

    double zaehler;
    double nenner;
    std::string Z;
    std::string N;
    int bruchpos = name.find_first_of("/");
    if (bruchpos == -1) {
        Z = name;
        N = "1";
    }
    else {
        Z = name.substr(0, bruchpos);
        N = name.substr(bruchpos+1, 100);
    }
    std::stringstream str1;
    str1 << Z;
    str1 >> zaehler;
    std::stringstream str2;
    str2 << N;
    str2 >> nenner;
    return zaehler / nenner;
}

std::pair<int, int> dtoB(double wert, int fehler) {

    std::vector<int> kettenbruch;                           //speichert this->wert als a + 1/(b + 1/(c + 1/(d + 1/...)))     (speichert nur betrag)
    int counter = 0;                                        //verhindert, das while()schleife endlos wird
    double rest = wert;                                     //buffer zur berechnung der werte von kettenbruch
    bool negativ = false;                                   //speichert vorzeichen
    if (wert < 0) {
        negativ = true;
        rest *= (-1);
    }

    do {
        int naehrung = rest;
        rest -= naehrung;
        rest = 1 / rest;
        kettenbruch.push_back(naehrung);
        counter ++;
    } while (rest < fehler && counter < 50);

    int zaehler = 1, nenner = 0;                            //spaetere rueckgabewerte (initialisierung mag komisch vorkommen, es wird aber fortlaufend der kehrwert genommen.)

    for (int i = kettenbruch.size(); i > 0; i--) {
        nenner += kettenbruch.back() * zaehler;
        int buffer = zaehler;
        zaehler = nenner;
        nenner = buffer;
        kettenbruch.pop_back();
    }
    if (negativ) zaehler *= (-1);

    std::pair<int, int> rueckgabe;
    rueckgabe.first = zaehler;
    rueckgabe.second = nenner;

    return rueckgabe;

}

std::string DoubleZuBruchStr(double wert, bool showpos) {

    const int fehler = 1000;
    std::pair<int, int> bruch = dtoB(wert, fehler);
    std::string ausgabe;

    if ((bruch.second > fehler*(-1) && bruch.second < fehler) && (bruch.first < 100 || bruch.second < 100)) {

        std::ostringstream str1;
        if (showpos) {
            str1 << std::showpos << bruch.first;
        }
        else {
            str1 << bruch.first;
        }
        ausgabe += str1.str();
        if (bruch.second != 1) {
            ausgabe += "/";
            std::ostringstream str2;
            str2 << bruch.second;
            ausgabe += str2.str();
        }
    }
    else {
        std::ostringstream str1;
        str1.precision(5);
        str1 << wert;
        ausgabe += str1.str();
    }

    return ausgabe;
}


std::string druckenfloat(double wert, bool showpos) {
    std::ostringstream str2;
    if (showpos) {
        str2 << std::showpos << wert;
    }
    else {
        str2 << wert;
    }
    return str2.str();
}
