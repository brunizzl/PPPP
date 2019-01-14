#ifndef BAUTEIL_H
#define BAUTEIL_H

#include <string>
#include <iostream>

#include "doubleBruch.h"

class netzwerk;         ///deklaration der netzwerk klasse um pointer auf netzwerk als bauteilparameter haben zu können

struct matrix_elem      ///nur funktionsrückgabe
{
    char v_typ;
    double faktor;
};


class bauteil
{
protected:
    std::string name;   ///name für nutzer
    int knoten1;        ///verknüpfungspunkt1 positiv: von 1 nach 2
    int knoten2;        ///verknüpfungspunkt2
    double volt;        ///wie viel volt fallen über bauteil ab
    double ampere;      ///vie viel ampere fließen duch bauteil
    bool v_known;       ///false: 'u' ist var, true: 'u' ist konst
    bool a_known;       ///analog
    int  n_known;       ///zählt, wie viel variablen insgesamt unbekannt sind

public:
    bauteil(std::string &name_, int knoten1_, int knoten2_);
    virtual ~bauteil();

    std::string get_name();     ///gibt name zurück
    int get_knoten1();          ///gibt knoten1 zurück
    int get_knoten2();          ///gibt knoten2 zurück

    void set_volt(double wert);         ///schreibt volt und v_known
    void set_ampere(double wert);       ///schreibt ampere und a_known
    double get_volt();                  ///gibt variable volt aus
    double get_ampere();                ///gibt variable ampere aus
    bool get_v_known();                 ///ist volt gesucht oder nicht?
    bool get_a_known();                 ///ist ampere gesucht oder nicht?

    virtual char var_gesucht();                 ///gibt 'u' zurück, wenn volt gesucht, 'i', wenn ampere gesucht und 'b', wenn beides gesucht ist '\0' wenn alles known
    virtual matrix_elem spannung(int knoten);   ///gibt wert für maschengleichung (matrix) zurück. char gibt an an welche stelle in variablen<> geschrieben werden soll
    virtual matrix_elem strom(int knoten);      ///analog zu spannung nur für knotengleichungen char ist '\0': ergebnis, 'i': <'i', aktuelles bauteil*>, 'u': <'u', aktuelles bauteil*>

    virtual void print();
};


class widerstand : public bauteil
{
protected:
    double ohm;         ///wie viel ohm hat widerstand
    bool o_known;       ///analog zu v_known und a_known

public:
    widerstand(std::string &name_, int knoten1_, int knoten2_);
    ~widerstand();

    void set_ohm(double wert);      ///schreibt ohm und o_known
    double get_ohm();               ///gibt variable ohm aus
    bool get_o_known();             ///ist ohm gesucht oder known?

    char var_gesucht() override;                ///gibt 'u' zurück, wenn volt gesucht, 'i', wenn ampere gesucht und 'b', wenn beides gesucht ist (widerstand kann dann errechnet werden)
    matrix_elem spannung(int knoten) override;  ///gibt wert für maschengleichung (matrix) zurück. char gibt an an welche stelle in variablen<> geschrieben werden soll
    matrix_elem strom(int knoten) override;     ///analog zu spannung nur für knotengleichungen char ist '\0': ergebnis, 'i': <'i', aktuelles bauteil*>, 'u': <'u', aktuelles bauteil*>

    void print() override;
};


///quelle wird immer als spannungsquelle angenommen. aufteilung in zwei klassen möglich aktuell: strom fließt positiv gegen bauteilrichtung (knoten 2->1)
class quelle : public bauteil
{
protected:
    ///alle variablen befinden sich in stammklasse
public:
    quelle(std::string &name_, int knoten1_, int knoten2_);
    ~quelle();

    char var_gesucht() override;
    matrix_elem spannung(int knoten) override;  ///gibt wert für maschengleichung (matrix) zurück. char gibt an an welche stelle in variablen<> geschrieben werden soll
    matrix_elem strom(int knoten) override;     ///analog zu spannung nur für knotengleichungen char ist '\0': ergebnis, 'i': <'i', aktuelles bauteil*>, 'u': <'u', aktuelles bauteil*>

    void print() override;
};


#endif // BAUTEIL_H
