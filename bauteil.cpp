#include "bauteil.h"

bauteil::bauteil(std::string &name_, int knoten1_, int knoten2_)
    :name(name_), knoten1(knoten1_), knoten2(knoten2_),
     v_known(false), a_known(false), n_known(0)
{
}

bauteil::~bauteil()
{
}

void bauteil::set_volt(double wert)
{
    volt = wert;
    if (!v_known) {
        n_known++;
        v_known = true;
    }
}

void bauteil::set_ampere(double wert)
{
    ampere = wert;
    if (!a_known) {
        n_known++;
        a_known = true;
    }
}

double bauteil::get_volt()
{
    if (!v_known) {
        std::cout << "Fehler: unbekannte Spannung von Bauteil " << get_name() << " wird gelesen" << '\n';
    }
    return volt;
}

double bauteil::get_ampere()
{
    if (!a_known) {
        std::cout << "Fehler: unbekannte Stromstaerke von Bauteil " << get_name() << " wird gelesen" << '\n';
    }
    return ampere;
}

bool bauteil::get_v_known()
{
    return v_known;
}

bool bauteil::get_a_known()
{
    return a_known;
}

std::string bauteil::get_name()
{
    return name;
}

int bauteil::get_knoten1()
{
    return knoten1;
}

int bauteil::get_knoten2()
{
    return knoten2;
}

char bauteil::var_gesucht()
{
    std::cout << "Fehler: suche Variablen nur in allgemeinem bauteil!" << '\n';
    return '\0';
}

matrix_elem bauteil::spannung(int knoten)
{
    std::cout << "Fehler: Bauteil " << get_name() << " ist nicht naeher spezifiziert (widerstand oder quelle)" << '\n';
    return {0, 0};
}

matrix_elem bauteil::strom(int knoten)
{
    std::cout << "Fehler: Bauteil " << get_name() << " ist nicht naeher spezifiziert (widerstand oder quelle)" << '\n';
    return {0, 0};
}

void bauteil::print()
{
    std::cout << "Fehler: print nur fuer allgemeines bauteil aufgerufen" << '\n';
}


widerstand::widerstand(std::string &name_, int knoten1_, int knoten2_)
    :bauteil(name_, knoten1_, knoten2_),
     o_known(false)
{
}

widerstand::~widerstand()
{
}

void widerstand::set_ohm(double wert)
{
    ohm = wert;
    if (!o_known) {
        n_known++;
        o_known = true;
    }
}

double widerstand::get_ohm()
{
    if (!o_known) {
        std::cout << "Fehler: unbekannter Widerstand von Bauteil " << get_name() << " wird gelesen" << '\n';
    }
    return ohm;
}

bool widerstand::get_o_known()
{
    return o_known;
}

char widerstand::var_gesucht()
{
    if (n_known == 3) {
        double delta_ohm = volt / ampere - ohm;
        if (!(delta_ohm < 0.001) || !(delta_ohm > -0.001)) {
            std::cout << "Warnung: Widerstand " << get_name() << " hat einen falsch eingegeben Wert. fuer Rechnung wird ohm ignoriert." << '\n';
            o_known = 0;
        }
        return '\0';                    //es wird keine variable mehr gesucht
    }
    else if (n_known == 2) {
        if (!v_known) {
            set_volt(ohm * ampere);
            return '\0';
        }
        else if (!a_known) {
            if (ohm != 0) {         //wenn ohm == 0 muss volt auch == 0 sein, dann ist ampere beliebig.
                set_ampere(volt / ohm);
                return '\0';
            }
            else {
                if (volt != 0) {
                    std::cout << "Fehler: Widerstand " << get_name() << " hat 0 Ohm, aber ein Spannungsabfall != 0 Volt (= " << get_volt() << " Volt)" << '\n';
                    n_known--;
                    v_known = false;
                }
                return 'i';
            }
        }
        else {
            set_ohm(volt / ampere);
            return '\0';
        }                //dritte variable wurde gerade berechntet -> es wird keine variable mehr gesucht
    }
    else if (n_known == 1) {
        if (!v_known) {
            return 'u';             //volt ist unbekannt -> ampere oder ohm bekannt -> wenn volt per matrix berechnet, kann dritter wert ausgerechnet werden.
        }
        else {
            return 'i';             //volt ist bekannt -> ampere und ohm sind unbekannt -> ampere wird per matrix berechntet, ohm per ohmschen gesetz spaeter
        }
    }
    else {
        return 'b';                 //alle drei werte gesucht -> volt und ampere werden berechntet, ohm dann spaeter aus den beiden mit ohmschen gesetz
    }
}

matrix_elem widerstand::spannung(int knoten)
{
    matrix_elem ausgabe;
    if (n_known == 3) {
        ausgabe.v_typ = '\0';
        ausgabe.faktor = -volt;
    }
    else if (n_known == 2) {
        std::cout << "Fehler: matrix muss nach variablen aufgestellt werden (Widerstand " << get_name() << " hat 2 Unbekannte)" << '\n';
    }
    else if (n_known == 1) {
        if (v_known) {
            ausgabe.v_typ = '\0';
            ausgabe.faktor = -volt;
        }
        else {  // volt ist unbekannt -> volt steht in matrix (weil es prioritaet ueber ampere hat, in matrix geschrieben zu werden(siehe var_gesucht()))
            ausgabe.v_typ = 'u';
            ausgabe.faktor = 1;
        }
    }
    else {  //n_known == 0
        ausgabe.v_typ = 'u';
        ausgabe.faktor = 1;
    }

    if (knoten == knoten1) {    //strom fliesst von knoten1 weg -> negativ in knotengleichung von knoten 1
        ausgabe.faktor *= -1;
    }
    return ausgabe;
}

//rueckgabe char ist ob zahl in ergebnis kommt (wert bekannt -> char == '\0') oder in eine variablenspalte,
matrix_elem widerstand::strom(int knoten)
{
    matrix_elem ausgabe;
    if (n_known == 3) {
        ausgabe.v_typ = '\0';
        ausgabe.faktor = -ampere;
    }
    else if (n_known == 2) {
        std::cout << "Fehler: matrix muss nach variablen aufgestellt werden (Widerstand " << get_name() << " hat 2 Bekannte)" << '\n';
    }
    else if (n_known == 1) {            //bauteil kommt nur ein mal in variablen<> vor
        if (a_known) {
            ausgabe.v_typ = '\0';       //wird in ergebnis geschrieben
            ausgabe.faktor = -ampere;   //ergebnis andere seite von gleich -> * -1 rechnen
        }
        else if (v_known) {     // 'i' ist dann in variablen<> eingetragen
            ausgabe.v_typ = 'i';
            ausgabe.faktor = 1;
        }
        else {      //nur ohm ist bekannt -> 'u' ist in variablen<> eingetragen
            ausgabe.v_typ = 'u';
            ausgabe.faktor = 1 / ohm;
        }
    }
    else {          //n_known == 0 -> 'u' und 'i' sind in variablen<> eingetragen
        ausgabe.v_typ = 'i';
        ausgabe.faktor = 1;
    }

    if (knoten == knoten2) {    //strom fliesst von knoten1 weg -> negativ in knotengleichung von knoten 1
        ausgabe.faktor *= -1;
    }
    return ausgabe;
}

void widerstand::print()
{
    std::cout.precision(10);
    std::cout << get_name() << ": " << knoten1 << "->" << knoten2 << '\n';
    //std::cout << "---------" << '\n';
    if (v_known) {
        std::cout << "U: " << DoubleZuBruchStr(volt, false) << " Volt" << '\n';
    }
    if (a_known) {
        std::cout << "I: " << DoubleZuBruchStr(ampere, false) << " Ampere" << '\n';
    }
    if (o_known) {
        std::cout << "R: " << DoubleZuBruchStr(ohm, false) << " Ohm" << '\n';
    }
    std::cout << '\n';
}


quelle::quelle(std::string &name_, int knoten1_, int knoten2_)
    :bauteil(name_, knoten1_, knoten2_)
{
}

quelle::~quelle()
{
}

char quelle::var_gesucht()
{
    if (n_known == 0) {
        return 'b';
    }
    else if (!v_known) {
        return 'u';
    }
    else if (!a_known) {
        return 'i';
    }
    else {
        return '\0';
    }
}

matrix_elem quelle::spannung(int knoten)
{
    matrix_elem ausgabe;
    if (v_known) {
        ausgabe.v_typ = '\0';
        ausgabe.faktor = -volt;
    }
    else {
        ausgabe.v_typ = 'u';
        ausgabe.faktor = 1;
    }

    if (knoten == knoten1) {    //strom fliesst von knoten2 weg -> negativ in knotengleichung von knoten1 (bei quelle ist strom und spannung in unterschiedlichen richtungen)
        ausgabe.faktor *= -1;
    }
    return ausgabe;
}

matrix_elem quelle::strom(int knoten)
{
    matrix_elem ausgabe;
    if (a_known) {
        ausgabe.v_typ = '\0';
        ausgabe.faktor = -ampere;
    }
    else {
        ausgabe.v_typ = 'i';
        ausgabe.faktor = 1;
    }

    if (knoten == knoten2) {    //strom fliesst von knoten2 weg -> negativ in knotengleichung von knoten1
        ausgabe.faktor *= -1;
    }
    return ausgabe;
}

void quelle::print()
{
    std::cout.precision(10);
    std::cout << get_name() << ": " << knoten1 << "->" << knoten2 << '\n';
    //std::cout << "---------" << '\n';
    if (v_known) {
        std::cout << "U: " << DoubleZuBruchStr(volt, false) << " Volt" << '\n';
    }
    if (a_known) {
        std::cout << "I: " << DoubleZuBruchStr(ampere, false) << " Ampere" << '\n';
    }
    std::cout << '\n';
}










