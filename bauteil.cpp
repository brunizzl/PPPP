#include "bauteil.h"

bauteil::bauteil(std::string &name_, int knoten1_, int knoten2_)
    :name(name_), knoten1(knoten1_), knoten2(knoten2_), n_bekannt(0)
{
}

bauteil::~bauteil()
{
}

void bauteil::set_volt(double wert)
{
    if (!volt.bekannt()) {
        n_bekannt++;
    }
    volt = wert;
}

void bauteil::set_volt(std::string & wert)
{
	if (!volt.bekannt()) {
		n_bekannt++;
	}
	volt = wert;
}

void bauteil::set_volt_frei()
{
	volt.status = freie_var;
}

bool bauteil::volt_frei()
{
	return volt.status == freie_var;
}

void bauteil::set_ampere(double wert)
{
    if (!ampere.bekannt()) {
        n_bekannt++;
    }
    ampere = wert;
}

void bauteil::set_ampere(std::string & wert)
{
	if (!ampere.bekannt()) {
		n_bekannt++;
	}
	ampere = wert;
}

void bauteil::set_ampere_frei()
{
	ampere.status = freie_var;
}

bool bauteil::ampere_frei()
{
	return ampere.status == freie_var;
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

void bauteil::berechne()
{
	//allgemeines bauteil kann nicht bauteil_parameter auseinader berrechnen
	return;
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
    :bauteil(name_, knoten1_, knoten2_)
{
}

widerstand::~widerstand()
{
}

void widerstand::set_ohm(double wert)
{
    if (!ohm.bekannt()) {
        n_bekannt++;
    }
    ohm = wert;
}

void widerstand::set_ohm_frei()
{
	ohm.status = freie_var;
}

bool widerstand::ohm_frei()
{
	return ohm.status == freie_var;
}

char widerstand::var_gesucht()
{
    if (n_bekannt == 3) {
        double delta_ohm = volt.d_wert / ampere.d_wert - ohm.d_wert;
        if (!(delta_ohm < 0.001) || !(delta_ohm > -0.001)) {
            std::cout << "Warnung: Widerstand " << get_name() << " hat einen falsch eingegeben Wert. fuer Rechnung wird ohm ignoriert." << '\n';
            ohm.status = unbekannt;
        }
        return '\0';                    //es wird keine variable mehr gesucht
    }
    else if (n_bekannt == 2) {
        if (!volt.bekannt()) {
            //set_volt(ohm.d_wert * ampere.d_wert);
			volt = ohm * ampere;
            return '\0';
        }
        else if (!ampere.bekannt()) {
            if (ohm.d_wert != 0) {         //wenn ohm == 0 muss volt auch == 0 sein, dann ist ampere beliebig.
                //set_ampere(volt.d_wert / ohm.d_wert);
				ampere = volt / ohm;
                return '\0';
            }
            else {
                if (volt.d_wert != 0) {
                    std::cout << "Fehler: Widerstand " << get_name() << " hat 0 Ohm, aber ein Spannungsabfall != 0 Volt (= " << volt.d_wert << " Volt)" << '\n';
                    n_bekannt--;
                    volt.status = unbekannt;
                }
                return 'I';
            }
        }
        else {
            //set_ohm(volt.d_wert / ampere.d_wert);
			ohm = volt / ampere;
            return '\0';
        }                //dritte variable wurde gerade berechntet -> es wird keine variable mehr gesucht
    }
    else if (n_bekannt == 1) {
        if (!volt.bekannt()) {
            return 'U';             //volt ist unbekannt -> ampere oder ohm bekannt -> wenn volt per matrix berechnet, kann dritter wert ausgerechnet werden.
        }
        else {
            return 'I';             //volt ist bekannt -> ampere und ohm sind unbekannt -> ampere wird per matrix berechntet, ohm per ohmschen gesetz spaeter
        }
    }
    else {
        return 'b';                 //alle drei werte gesucht -> volt und ampere werden berechntet, ohm dann spaeter aus den beiden mit ohmschen gesetz
    }
}

void widerstand::berechne()
{
	if (n_bekannt == 2) {
		if (!volt.bekannt()) {
			volt = ohm * ampere;
		}
		else if (!ampere.bekannt()) {
			ampere = volt / ohm;
		}
		else {	//ohm ist unbekannt
			ohm = volt / ampere;
		}
	}
}

matrix_elem widerstand::spannung(int knoten)
{
    matrix_elem ausgabe;
    if (n_bekannt == 3) {
        ausgabe.v_typ = '\0';
        ausgabe.faktor = -volt.d_wert;
    }
    else if (n_bekannt == 2) {
        std::cout << "Fehler: matrix muss nach variablen aufgestellt werden (Widerstand " << get_name() << " hat 2 Unbekannte)" << '\n';
    }
    else if (n_bekannt == 1) {
        if (volt.bekannt()) {
            ausgabe.v_typ = '\0';
            ausgabe.faktor = -volt.d_wert;
        }
        else {  // volt ist unbekannt -> volt steht in matrix (weil es prioritaet ueber ampere hat, in matrix geschrieben zu werden(siehe var_gesucht()))
            ausgabe.v_typ = 'U';
            ausgabe.faktor = 1;
        }
    }
    else {  //n_bekannt == 0
        ausgabe.v_typ = 'U';
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
    if (n_bekannt == 3) {
        ausgabe.v_typ = '\0';
        ausgabe.faktor = -ampere.d_wert;
    }
    else if (n_bekannt == 2) {
        std::cout << "Fehler: matrix muss nach variablen aufgestellt werden (Widerstand " << get_name() << " hat 2 Bekannte)" << '\n';
    }
    else if (n_bekannt == 1) {				//bauteil kommt nur ein mal in variablen<> vor
        if (ampere.bekannt()) {
            ausgabe.v_typ = '\0';				//wird in ergebnis geschrieben
            ausgabe.faktor = -ampere.d_wert;    //ergebnis andere seite von gleich -> * -1 rechnen
        }
        else if (volt.bekannt()) {		// 'I' ist dann in variablen<> eingetragen
            ausgabe.v_typ = 'I';
            ausgabe.faktor = 1;
        }
        else {      //nur ohm ist bekannt -> 'U' ist in variablen<> eingetragen
            ausgabe.v_typ = 'U';
            ausgabe.faktor = 1 / ohm.d_wert;
        }
    }
    else {          //n_bekannt == 0 -> 'U' und 'I' sind in variablen<> eingetragen
        ausgabe.v_typ = 'I';
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
    if (volt.status == d_bekannt) {
        std::cout << "U: " << DoubleZuBruchStr(volt.d_wert, false) << " Volt" << '\n';
    }
	else if (volt.status == s_bekannt) {
		std::cout << "U: " << volt.s_wert << " Volt" << '\n';
	}
    if (ampere.status == d_bekannt) {
        std::cout << "I: " << DoubleZuBruchStr(ampere.d_wert, false) << " Ampere" << '\n';
    }
	else if (ampere.status == s_bekannt) {
		std::cout << "I: " << ampere.s_wert << " Ampere" << '\n';
	}
    if (ohm.status == d_bekannt) {
        std::cout << "R: " << DoubleZuBruchStr(ohm.d_wert, false) << " Ohm" << '\n';
    }
	else if (ohm.status == s_bekannt) {
		std::cout << "R: " << ohm.s_wert << " Ohm" << '\n';
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
    if (n_bekannt == 0) {
        return 'b';
    }
    else if (!volt.bekannt()) {
        return 'U';
    }
    else if (!ampere.bekannt()) {
        return 'I';
    }
    else {
        return '\0';
    }
}

matrix_elem quelle::spannung(int knoten)
{
    matrix_elem ausgabe;
    if (volt.bekannt()) {
        ausgabe.v_typ = '\0';
        ausgabe.faktor = -volt.d_wert;
    }
    else {
        ausgabe.v_typ = 'U';
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
    if (ampere.bekannt()) {
        ausgabe.v_typ = '\0';
        ausgabe.faktor = -ampere.d_wert;
    }
    else {
        ausgabe.v_typ = 'I';
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
    if (volt.status == d_bekannt) {
        std::cout << "U: " << DoubleZuBruchStr(volt.d_wert, false) << " Volt" << '\n';
    }
	else if (volt.status == s_bekannt) {
		std::cout << "U: " << volt.s_wert << " Volt" << '\n';
	}
    if (ampere.status == d_bekannt) {
        std::cout << "I: " << DoubleZuBruchStr(ampere.d_wert, false) << " Ampere" << '\n';
    }
	else if (volt.status == s_bekannt) {
		std::cout << "I: " << ampere.s_wert << " Ampere" << '\n';
	}
    std::cout << '\n';
}










