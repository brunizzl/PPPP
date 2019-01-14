#include "netzwerk.h"

netzwerk::netzwerk()
    :n_variablen(0)
{
}

netzwerk::~netzwerk()
{
    knoten.clear();

    for (unsigned int i = 0; i < bauteile.size(); i++) {
        delete bauteile[i];
    }

    for (unsigned int i = 0; i < matrix.size(); i++) {
        delete[] matrix[i];
    }

    variablen.clear();
    maschen.clear();
}

void netzwerk::add_knoten(int name)
{
    std::vector<bauteil *> dummy;                   ///leerer vektor, der mit in map gespeichert werden kann
    bool erfolg = knoten.insert(std::make_pair(name, dummy)).second;
    if (!erfolg) {
        std::cout << "Fehler: Knoten " << name << " existiert bereits." << '\n';
    }
}

widerstand *netzwerk::add_widerstand(std::string &name, int knoten1, int knoten2)
{
    widerstand *neu = new widerstand(name, knoten1, knoten2);                           ///baue widerstand
    bauteile.push_back(neu);                                                            ///speichere widerstand in vector

    knoten[knoten1].push_back(neu);         ///trage neue verbindung in graphen unter bauteile vector von knoten 1 ein
    knoten[knoten2].push_back(neu);         ///trage neue verbindung in graphen unter bauteile vector von knoten 2 ein
    return neu;                             ///r�ckgabe ist pointer auf neuen widerstand
}

quelle *netzwerk::add_quelle(std::string &name, int knoten1, int knoten2)
{
    quelle *neu = new quelle(name, knoten1, knoten2);                               ///baue quelle
    bauteile.push_back(neu);                                                        ///speichere quelle in vector

    knoten[knoten1].push_back(neu);         ///trage neue verbindung in graphen unter bauteile vector von knoten 1 ein
    knoten[knoten2].push_back(neu);         ///trage neue verbindung in graphen unter bauteile vector von knoten 2 ein
    return neu;                             ///r�ckgabe ist pointer auf neue quelle
}

void netzwerk::build_netzwerk()
{
    std::ifstream datei("netzwerk.txt");

    if (!datei) {
        std::cout << "Fehler: Datei netzwerk.txt befindet sich nicht in diesem Verzeichnis." << '\n';
    }
    else {
        std::cout << "Lese aus netzwerk.txt ein:" << '\n' << '\n';

        std::string zeile;
        while (std::getline(datei, zeile)) {

            if (zeile.find("knoten") != std::string::npos) {
                zeile.erase(0, 7);          ///l�sche das wort "knoten " inklusive erstes leerzeichen
                std::stringstream str;      ///restlicher string wird in int gewandelt und als name gespeichert
                str << zeile;
                int name;
                str >> name;
                add_knoten(name);
            }
            else if (zeile.find("widerstand") != std::string::npos) {
                zeile.erase(0, 11);     ///l�sche das wort "widerstand " inklusive erstes leerzeichen

                std::string name;
                int knoten1;
                int knoten2;
                std::stringstream str;
                str << zeile;           ///str liest ganze zeile ein
                str >> name;            ///erstes wort der zeile ist name
                str >> knoten1;         ///zweites wort der zeile ist knoten1
                str >> knoten2;         ///drittes wort der zeile ist knoten2
                widerstand *neu = add_widerstand(name, knoten1, knoten2);

                std::string extra_bed;
                std::string wert;
                while (str >> extra_bed) {
                    str >> wert;
                    switch (extra_bed[0]) {
                    case 'u':
                        neu->set_volt(BruchStrZuDouble(wert));
                        break;
                    case 'i':
                        neu->set_ampere(BruchStrZuDouble(wert));
                        break;
                    case 'r':
                        neu->set_ohm(BruchStrZuDouble(wert));
                        break;
                    }
                }
                neu->print();
            }
            else if (zeile.find("quelle") != std::string::npos) {
                zeile.erase(0, 7);      ///l�sche das wort "quelle " inklusive erstes leerzeichen

                std::string name;
                int knoten1;
                int knoten2;
                std::stringstream str;
                str << zeile;           ///str liest ganze zeile ein
                str >> name;            ///erstes wort der zeile ist name
                str >> knoten1;         ///zweites wort der zeile ist knoten1
                str >> knoten2;         ///drittes wort der zeile ist knoten2
                quelle *neu = add_quelle(name, knoten1, knoten2);

                std::string extra_bed;
                std::string wert;
                while (str >> extra_bed) {
                    str >> wert;
                    switch (extra_bed[0]) {
                    case 'u':
                        neu->set_volt(BruchStrZuDouble(wert));
                        break;
                    case 'i':
                        neu->set_ampere(BruchStrZuDouble(wert));
                        break;
                    }
                }
                neu->print();
            }
            else if (zeile.find("ende") != std::string::npos) {
                break;                  ///erlaubt es in die selbe datei noch alte netzwerke und/ oder eine bedienungsanleitung zu schreiben
            }
        }
    }
}

void netzwerk::build_matrix()
{
    ///baue variblen vektor (quasi spaltenbeschriftung der matrix)
    for (unsigned int nummer = 0; nummer < bauteile.size(); nummer++) {
        switch (bauteile[nummer]->var_gesucht()) {
        case 'u':
            variablen.push_back({bauteile[nummer], 'u'}); ///vektor mit zu berechnenden variablen wird um neue variable erg�nzt
            n_variablen++;
            break;
        case 'i':
            variablen.push_back({bauteile[nummer], 'i'});
            n_variablen++;
            break;
        case 'b':
            variablen.push_back({bauteile[nummer], 'u'});
            variablen.push_back({bauteile[nummer], 'i'});
            n_variablen += 2;
            break;
        }
    }
    ///stelle matrix auf: erste zeilen kommen aus knotengleichungen, zeilen danach aus maschengleichungen
    ///knotengleichungen:
    for (auto i_knoten : knoten) {                                              ///i_knoten ist std::pair<int, std::vector<bauteil *> > (also ein mapelement)
        double *zeile = new double[n_variablen + 1];                            ///matrixzeile (+ 1 ist ergebnisspalte)
        for (unsigned int i = 0; i < n_variablen + 1; i++) {
            zeile[i] = 0;
        }
        ///jedes an knoten angrenzende teil muss in zeile vorkommen (wenn bekannt in ergebnisspalte, sonst als variable)
        for (auto teil : i_knoten.second) {                                     ///teil ist bauteil* aus vektor aus map "knoten"
            matrix_elem wert = teil->strom(i_knoten.first);
            if (wert.v_typ == '\0') {
                zeile[n_variablen] += wert.faktor;                              ///wert wird auf ergebnisspalte addiert
            }
            else {
                bool gefunden = false;
                for (unsigned int var = 0; var < variablen.size() && !gefunden; var++) {        ///h�tte gerne auto genutzt, brauche aber zu beschreibende spalte von matrix (aka var)
                    if (variablen[var].v_typ == wert.v_typ && variablen[var].v_teil == teil) {
                        gefunden = true;
                        zeile[var] = wert.faktor;
                    }
                }
                if (!gefunden) {
                    std::cout << "Fehler: Bauteil " << teil->get_name() << " wurde nicht in variablen der matrix gefunden, aber gesucht. (strom)" << '\n';
                }
            }
        }

        matrix.push_back(zeile);
    }

    ///maschengleichungen:
    std::list<maschenabschnitt> anfang;
    anfang.push_back({bauteile[0], bauteile[0]->get_knoten2()});
    finde_maschen(anfang);

    for (auto masche : maschen) {                                   ///masche ist liste mit maschenabschnitten, die kreis bilden
        double *zeile = new double[n_variablen + 1];
        for (unsigned int i = 0; i < n_variablen + 1; i++) {
            zeile[i] = 0;
        }
        ///jedes bauteil in der masche muss in zeile vorkommen. entweder als var oder in erg
        for (auto it : masche) {
            matrix_elem wert = it.m_teil->spannung(it.m_knoten);
            if (wert.v_typ == '\0') {
                zeile[n_variablen] += wert.faktor;
            }
            else {
                bool gefunden = false;
                for (unsigned int var = 0; var < variablen.size(); var++) {
                    if (variablen[var].v_typ == wert.v_typ && variablen[var].v_teil == it.m_teil) {
                        gefunden = true;
                        zeile[var] = wert.faktor;
                    }
                }
                if (!gefunden) {
                    std::cout << "Fehler: Bauteil " << it.m_teil->get_name() << " wurde nicht in variablen der matrix gefunden, aber gesucht. (spannung)" << '\n';
                }
            }
        }
        matrix.push_back(zeile);
    }
}

void netzwerk::finde_maschen(std::list<maschenabschnitt> masche)
{
    bool kreis = false;                 ///ist aktuells letzter knoten bereits vorher enthalten? (ist masche vollst�ndiger kreis?)
    int zaehler = 0;                    //zaehlt iteration von for schleife
    int abbruch = masche.size() - 1;    //sorgt f�r abbruch von for schleife ein schritt vor ende (letztes element soll nicht mit sich verglichen werden)
    for (auto &it : masche) {
        if (zaehler++ < abbruch) {      //(ich wei� selbst, dass das krebs ist, aber habe gerade keinen plan, wie das in echt gehen w�rde)
            if (it.m_knoten == masche.back().m_knoten) {
                kreis = true;
                break;
            }
        }
    }
    if (kreis) {
        while (masche.front().m_knoten != masche.back().m_knoten) {
            masche.pop_front();
        }
        masche.pop_front();
        maschen.push_back(masche);
    }
    else {
        /// baue neue masche f�r jede verzweigung
        int aktueller_knoten = masche.back().m_knoten;
        for (auto teil : knoten[aktueller_knoten]) {

            if (teil != masche.back().m_teil) {

                std::list<maschenabschnitt> masche2 = masche;

                int neuer_knoten = 0;
                if (teil->get_knoten1() == aktueller_knoten) {
                    neuer_knoten = teil->get_knoten2();
                }
                else if (teil->get_knoten2() == aktueller_knoten) {
                    neuer_knoten = teil->get_knoten1();
                }
                else {
                    std::cout << "Fehler: Bauteil " << teil->get_name() << " ist nicht an Knoten " << aktueller_knoten << " angeschlossen. (baue maschengleichungen)" << '\n';
                }

                masche2.push_back({teil, neuer_knoten});
                finde_maschen(masche2);
            }
        }
    }
    return;
}

void netzwerk::berechne_netzwerk()
{
    build_matrix();
    std::cout << '\n' << "Gleichungssystem:" << '\n';
    draw_matrix();
    gauss_matrix();
}

void netzwerk::draw_matrix()
{
    std::cout << '\n';
    for (auto it : variablen) {
        std::cout << it.v_typ << ' ' << it.v_teil->get_name() << '\t';
    }
    std::cout << '\n';

    for (auto it : matrix) {
        for (unsigned int i = 0; i < n_variablen; i++) {
            std::cout << DoubleZuBruchStr(it[i], false) << '\t';
        }
        std::cout << "| " << DoubleZuBruchStr(it[n_variablen], false) << '\n';
    }
    std::cout << '\n';
}

void netzwerk::gauss_matrix()
{
    ///gaussen:
    unsigned int kurze_seite;
    if (n_variablen < matrix.size()) {
        kurze_seite = n_variablen;
    }
    else {
        kurze_seite = matrix.size();
    }

    for (unsigned int ref_zeile = 0, ref_spalte = 0; ref_zeile < kurze_seite; ref_zeile++, ref_spalte++) {
        ///Bestimme erste Zeile mit Wert in ref_spalte != 0
        unsigned int pivot_zeile;
        bool pivot = false;
        unsigned int test_zeile = ref_zeile;
        do {
            if (matrix[test_zeile][ref_spalte] != 0) {
                pivot_zeile = test_zeile;
                pivot = true;
            }
            test_zeile++;
        } while (!pivot && test_zeile < matrix.size());

        if (pivot) {
            if (pivot_zeile > ref_zeile) {
                ///zeilen m�ssen getauscht werden
                double *buffer = matrix[pivot_zeile];
                matrix[pivot_zeile] = matrix[ref_zeile];
                matrix[ref_zeile] = buffer;
            }

            ///variablen unter pivotelement werden null
            for (unsigned int zeile = ref_zeile + 1; zeile < matrix.size(); zeile++) {
                double faktor = matrix[zeile][ref_spalte] / matrix[ref_zeile][ref_spalte];

                for (unsigned int spalte = ref_spalte; spalte < n_variablen + 1; spalte++) {
                    if (spalte == ref_spalte) {
                        ///vermeidung rundungsfehler -> direkt exakt zu null setzen
                        matrix[zeile][spalte] = 0;
                    }
                    else {
                        matrix[zeile][spalte] -= faktor * matrix[ref_zeile][spalte];
                    }
                }
            }
        }
        else {
            ref_zeile--;
        }
    } ///matrix ist jetzt in dreiecksform gebracht
    //draw_matrix();

    ///ergebnis ausrechnen (vorsicht trickologie) (ist etwas ausf�rlicher am ende der datei erkl�rt):
    ///erster schritt: freie variablen nach hinten schieben

    ///berechne positionen pivotelemente:
    bool *pivotelemente = new bool[n_variablen];        ///array in dem steht, in welchen spalten pivotelemente stehen
    for (unsigned int spalte = 0; spalte < n_variablen; spalte++) {
        pivotelemente[spalte] = false;
    }
    for (unsigned int zeile = 0; zeile < n_variablen && zeile < matrix.size(); zeile++) {
        unsigned int spalte = 0;
        while (matrix[zeile][spalte] == 0) {
            spalte++;
        }
        if (spalte < n_variablen) {
            pivotelemente[spalte] = true;
        }
    }

    ///zaehle pivotelemente
    n_pivotelemente = 0;
    for (unsigned int spalte = 0; spalte < n_variablen; spalte++) {
        if (pivotelemente[spalte]) n_pivotelemente++;
    }

    ///spalten mit pivotelementen werden nach vorne geschoben
    for (unsigned int spalte = 0; spalte < n_variablen; spalte++) {
        if (!pivotelemente[spalte]) {
            unsigned int spalte_pivotelement = spalte + 1;
            bool tausch = false;
            while (spalte_pivotelement < n_variablen && !tausch) {
                if (pivotelemente[spalte_pivotelement]) {
                    tausch = true;
                    pivotelemente[spalte_pivotelement] = false;
                    pivotelemente[spalte] = true;

                    spaltentausch(spalte, spalte_pivotelement);
                }
                else {
                    spalte_pivotelement++;
                }
            }
        }
    }
    delete[] pivotelemente;

    for (int zeile = n_pivotelemente - 1; zeile >= 0; zeile--) {

        ///freie variablen und ergebnisspalte durch jeweiliges pivotelement teilen
        ///(pivotelement selbst m�sste auch getielt werden, ist dann aber immer 1, lohnt sich also nicht (siehe trickologie))
        for (unsigned int spalte = n_pivotelemente; spalte < n_variablen + 1; spalte++) {
            matrix[zeile][spalte] /= matrix[zeile][zeile];      /// auf hauptdiagonalen -> zeile == spalte (matrix[zeile][zeile] ist pivotelement)
            if (spalte < n_variablen) {                         ///f�r freie Variablen (quasi umstellen auf andere seite des gleichs)
                matrix[zeile][spalte] *= -1;
            }
        }

        ///Freie Variablen und ergebnisspalte f�r zeilen dar�ber als anteil der gerade berechneten festen variable
        ///auf die bereits vorhandenen werte f�r freie variablen addieren
        for (int z = zeile - 1; z >= 0; z--) {
            for (unsigned int spalte = n_pivotelemente; spalte < n_variablen + 1; spalte++) {
                if (spalte < n_variablen) { ///freie variablen
                    matrix[z][spalte] += matrix[zeile][spalte] * matrix[z][zeile];
                }
                else {                      ///ergebnisspalte (steht auf anderer seite von gleichung ->  * -1
                    matrix[z][spalte] += matrix[zeile][spalte] * matrix[z][zeile] * -1;
                }
            }
        }
    }
    //draw_matrix();

    return;
}

void netzwerk::spaltentausch(int spalte1, int spalte2)
{
    variable buffer = variablen[spalte1];
    variablen[spalte1] = variablen[spalte2];
    variablen[spalte2] = buffer;

    for (unsigned int zeile = 0; zeile < matrix.size(); zeile++) {
        double spalte_buffer = matrix[zeile][spalte1];
        matrix[zeile][spalte1] = matrix[zeile][spalte2];
        matrix[zeile][spalte2] = spalte_buffer;
    }
}

bool netzwerk::ergebnis()
{
    for (int zeile = n_pivotelemente; zeile < matrix.size(); zeile++) {
        double wert = matrix[zeile][variablen.size()];
        if (wert > 0.001 || wert < -0.001) {
            return false;
        }
    }
    return true;
}

void netzwerk::ergebnisausgabe()
{
    if (ergebnis()) {
        if (n_variablen == n_pivotelemente) {
            ///iteriere �ber alle variablen:
            int zeile = 0;
            for (auto var : variablen) {
                if (var.v_typ == 'u') {
                    var.v_teil->set_volt(matrix[zeile][n_variablen]);
                }
                else {
                    var.v_teil->set_ampere(matrix[zeile][n_variablen]);
                }
                var.v_teil->var_gesucht();  ///var_gesucht erg�nzt auch werte, die sich direkt mit der bauteilinternen info berechnen lassen.
                zeile++;
            }
            std::cout << '\n' << "Berechnete Werte:" << '\n' << '\n';
            for (auto teil : bauteile) {
                teil->print();
            }
        }
        else {
            variablen_werte.resize(n_variablen);
        }
    }
    else {
        std::cout << "Fehler: Der Ergebnisvektor liegt ausserhalb des Bildraumes der Matrix." << '\n';
        std::cout << "Fertig umgestellte Matrix:" << '\n';
        draw_matrix();
    }
}



///Erkl�rung Funktionsweise des r�ckeinsetzens nach dem gaussen:
///(das habe ich alles nicht neu geschrieben, der matrixcode kommt aus einem alten matrixrechner von mir)
///Die Grundidee beim zur�ckeinsetzen ist, von unten nach oben die variablen auszurechnen und die werte f�r jede berechnete
///variable direkt auf die ergebnisspalte und die freien variablen draufzuaddieren.
///Sei x2 + 4*x3 - 2*x4 = 10   gespeichert als arrayzeile: 0   1   4   -2  |  10
///und 3*x3 - 6*x4 = 15        gespeichert als arrayzeile: 0   0   3   -6  |  15
///(zeile 1 ist hier vorhanden, jedoch nicht aufgef�hrt. die Matrix hat also 4 variablen bei 3 gleichungen.)
///als erstes wollen wir x3 alleine stehen haben. also rechnen wir durch den faktor vor x3
///und nehmen alle freien variablen *(-1), womit wir das gleichzeichen an x3 verschoben haben:
///x3 = 2*x4 + 5               gespeichert als arrayzeile: 0   0   1   2   |  5
///Nehmen wir an, x4 ist eine freie Variable, x3 ist damit fertig berechnet worden.
///(sonst h�tte es bereits eine zeile gegeben, in der x4 alleine steht.)
///Da x3 in x2 vier mal vorkommt, mehmen wir die Werte von x3 mal vier und speichern sie im array in der zeile von x2
///addiert mit den werten f�r x4 und die konstante die dort vorher schon standen.
///Da der konstante wert auf der anderen seite des gleichzeichens steht,
///m�ssen wir hier den konstanten wert von x3 (die 5) noch *(-1) nehmen, um ihn auch auf die andere Seite zu bringen.
///unsere eingesetzte Gleichung sieht also so aus: x2 + 6*x4 = -10
///als array brauchen wir also nur noch die speicherpl�tze von x4 und der konstanten Zahl um x2 darzustellen.
///die anderen arrayeintr�ge m�ssen wir nicht mehr ver�ndern:
///                                        als arrayzeile: 0   1   4   6   |  -10
///nun muss x3 auf die selbe weise in x1 (hier nicht dargestellt) eingesetzt werden, dann kann der ganze prozess mit x2 von vorne anfangen.































