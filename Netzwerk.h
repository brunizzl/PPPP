#ifndef netzwerk_H
#define netzwerk_H

#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <fstream>
#include <sstream>
#include <iostream>

#include "bauteil.h"
#include "doubleBruch.h"

//structs werden hier nur als h�bschere alternative zu std::pair genutzt
struct variable
{
    bauteil *v_teil;
    char v_typ;
};


struct maschenabschnitt
{
    bauteil *m_teil;
    int m_knoten;
};

class netzwerk
{
private:
    std::unordered_map<int, std::vector<bauteil *> > knoten;        //speichert knoten (int) und pointer auf damit verbundene bauteile (vector)
    std::vector<bauteil *> bauteile;                                //speichert bauteile

    std::vector<double*> matrix;                //gleichungssystem zur berechnung unbekannter variablen (pointer auf zeilen)
    std::vector<variable> variablen;            //speichert, wo welche variable im gleichungssystem steht ('u' fuer volt, 'i' fuer ampere) (spaltenbeschriftung)
    std::vector<std::string> variablen_werte;   //bei freien Var werden hier Ergebnisse eingetragen
    unsigned int n_variablen;                   //ein bauteil kann auch mehrere unbekannte haben -> muss extra gez�hlt werden (matrix breite)
    unsigned int n_pivotelemente;               //gibt an, wie viele variablen berechnet werden koennen (wird nach gaussen auggestellt)

    std::list<std::list<maschenabschnitt> > maschen;        //liste der maschen gespeichert jeweils als liste des weges einer masche (int ist knoten1 von bauteil*)

    void add_knoten(int name);                                                  //knoten hat nur nummer als name
    widerstand *add_widerstand(std::string &name, int knoten1, int knoten2);    //widerstand hat string als name
    quelle *add_quelle(std::string &name, int knoten1, int knoten2);            //quelle hat string als name

    void build_matrix();                                            //baut gleichungssystem zur berechnung inklusive bauen von variablen vektor
    void finde_maschen(std::list<maschenabschnitt> masche);         //wird in build matrix aufgerufen um rekursiv maschengleichungen aufzustellen (maschen liste zu f�llen)
    void gauss_matrix();                                            //gausst matrix und rechnet variablen aus
    void spaltentausch(int spalte1, int spalte2);                   //wird bei berechnung der variablen in gauss_matrix ben�tigt
    bool ergebnis();                                                //�berpr�ft ob nullzeilen in matrix ergebnis != 0 haben (wenn nicht -> ergebnis = true)

public:
    netzwerk();
    ~netzwerk();

    void build_netzwerk();      //liest netzwerk aus datei ein
    void berechne_netzwerk();   //macht den ganzen matrixdreck

    void draw_matrix();         //debuggingfunktion, die matrix darstellt
    void ergebnisausgabe();     //listet alle bauteile mit jetzt bekannten werten auf
};

#endif // netzwerk_H


