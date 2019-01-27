#ifndef BAUTEIL_H
#define BAUTEIL_H

#include <string>
#include <iostream>

#include "doubleBruch.h"
#include "bauteil_parameter.h"

class netzwerk;         //deklaration der netzwerk klasse um pointer auf netzwerk als bauteilparameter haben zu koennen

struct matrix_elem      //nur funktionsrueckgabe
{
    char v_typ;
    double faktor;
};


class bauteil
{
protected:
    std::string name;				//name fuer nutzer
    int knoten1;					//verknuepfungspunkt1 positiv: von 1 nach 2
    int knoten2;					//verknuepfungspunkt2
    bauteil_parameter volt;         //wie viel volt fallen ueber bauteil ab
    bauteil_parameter ampere;       //vie viel ampere fliessen duch bauteil
    int  n_bekannt;					//zaehlt, wie viel variablen insgesamt unbekannt sind

public:
    bauteil(std::string &name_, int knoten1_, int knoten2_);
    virtual ~bauteil();

    std::string get_name();     //gibt name zurueck
    int get_knoten1();          //gibt knoten1 zurueck
    int get_knoten2();          //gibt knoten2 zurueck

    void set_volt(double wert);         //schreibt volt und volt.status
	void set_volt(std::string &wert);	//schreibt volt und volt.status
	void set_volt_frei();				//stellt volt auf status freie var
	bool volt_frei();					//gibt zurueck, ob status == freie_var
    void set_ampere(double wert);       //schreibt ampere und ampere.status
	void set_ampere(std::string &wert);	//schreibt ampere und ampere.status
	void set_ampere_frei();				//stellt ampere auf status freie var
	bool ampere_frei();					//gibt zurueck ob apere.status == freie_var

    virtual char var_gesucht();                 //gibt 'U' zurueck, wenn volt gesucht, 'I', wenn ampere gesucht und 'b', wenn beides gesucht ist '\0' wenn alles bekannt
	virtual void berechne();					//berechnet bei wiederstand wenn zwei werte gegeben sind (auch als strings) den dritten
    virtual matrix_elem spannung(int knoten);   //gibt wert fuer maschengleichung (matrix) zurueck. char gibt an an welche stelle in variablen<> geschrieben werden soll
    virtual matrix_elem strom(int knoten);      //analog zu spannung nur fuer knotengleichungen char ist '\0': ergebnis, 'I': <'I', aktuelles bauteil*>, 'U': <'U', aktuelles bauteil*>

    virtual void print();
};


class widerstand : public bauteil
{
protected:
    bauteil_parameter ohm;         //analog zu volt und ampere in basisklasse

public:
    widerstand(std::string &name_, int knoten1_, int knoten2_);
    ~widerstand();

    void set_ohm(double wert);      //schreibt ohm und o_bekannt
	void set_ohm_frei();			//setzt status von ohm auf freie_var
	bool ohm_frei();				//gibt ohm.status == freie_var zurueck

    char var_gesucht() override;                //gibt 'U' zurueck, wenn volt gesucht, 'I', wenn ampere gesucht und 'b', wenn beides gesucht ist (widerstand kann dann errechnet werden)
	void berechne() override;					//berechnet fehlenden parameter aus zwei bekannten
    matrix_elem spannung(int knoten) override;  //gibt wert fuer maschengleichung (matrix) zurueck. char gibt an an welche stelle in variablen<> geschrieben werden soll
    matrix_elem strom(int knoten) override;     //analog zu spannung nur fuer knotengleichungen char ist '\0': ergebnis, 'I': <'I', aktuelles bauteil*>, 'U': <'U', aktuelles bauteil*>

    void print() override;
};


//quelle wird immer als spannungsquelle angenommen. aufteilung in zwei klassen moeglich aktuell: strom fliesst positiv gegen bauteilrichtung (knoten 2->1)
class quelle : public bauteil
{
protected:
    //alle variablen befinden sich in stammklasse
public:
    quelle(std::string &name_, int knoten1_, int knoten2_);
    ~quelle();

    char var_gesucht() override;
    matrix_elem spannung(int knoten) override;  //gibt wert fuer maschengleichung (matrix) zurueck. char gibt an an welche stelle in variablen<> geschrieben werden soll
    matrix_elem strom(int knoten) override;     //analog zu spannung nur fuer knotengleichungen char ist '\0': ergebnis, 'I': <'I', aktuelles bauteil*>, 'U': <'U', aktuelles bauteil*>

    void print() override;
};


#endif // BAUTEIL_H
