# PPPP
ProzProg ProgrammierProjekt
Beschreibung:
------------------

Das Programm soll in der Lage sein, ein beliebiges zeitunabhängiges elektrisches Netzwerk mit Widerständen und Quellen als Komponenten berechnen können. 
Das heißt, man gibt dem Programm Parameter einzelner Bauteile, etwa den Strom, die Spannung und/oder den Widerstand eines Widerstandes. Diese Information soll genutzt werden um fehlende Größen anderer Bauteile auszurechnen. Werden zu wenige Größen eingegeben, wird das Ergebnis in Abhängigkeit nicht bekannter Parameter berechnet.
Das Netzwerk wird als Textdatei eingelesen. Der Nutzer muss in die Datei in einem festgelegten Format jedes Bauteil mit anliegenden Knoten auflisten. Optional dazu können bekannte Bauteilgrößen mit dem Bauteil angegeben werden. Richtungsabhängige Größen werden dabei immer in Richtung von erstem angegebenen Knoten zu zweitem angegebenen Knoten eingetragen.
Bauteilparameter eines Widerstandes sind:
	- Strom (Richtungsabhängig, implizit Ampere)
	- Spannung (Richtungsabhängig, implizit Volt)
	- Widerstand (Richtungsunabhängig, implizit Ohm)
Bauteilparameter einer Quelle sind:
	- Strom (Richtungsabhängig, implizit Ampere)
	- Spannung (Richtungsabhängig, implizit Volt)
Die Quelle wird immer in Richtung einer Spannungsquelle eingetragen. Eine Stromquelle mit (als einziges bekanntem) positivem Strom ist in dieser Notation also eine Spannungsquelle mit unbekannter Spannung, sowie negativem Strom.

Beim Einlesen wird das Programm zwei Strukturen anlegen: eine Liste aller Bauteile, sowie eine Map, die jedem Knoten die direkt verbundenen Bauteile zuordnet. Als Ganzes kann das Programm das Netzwerk dann als Graph behandeln.

Nach dem Einlesen der Bauteile stellt das Programm eine Liste von gesuchten Variablen auf, wobei aber aufgrund des Ohmschen Gesetzes höchstens zwei Parameter eines Widerstandes gesucht werden müssen, der dritte kann aus den anderen berechnet werden. 

Um die gesuchten Werte zu berechnen legt das Programm eine Matrix an, wobei jede Spalte für eine gesuchte Variable steht (plus Ergebnisspalte). Die Gleichungen der Matrix werden aus den Knotengleichungen jedes Knotens, sowie den Maschengleichungen aufgestellt. Während die an einen Knoten angrenzenden Bauteile bekannt sind, müssen die Maschen erst noch gesucht werden. 
Das Programm läuft um das zu bewerkstelligen am ersten Bauteil los (das muss also mindest eine einfache Verbindung zu dem restlichen Netzwerk haben) und läuft rekursiv an jedem Knoten in jede weitere Richtung. Ist eine Masche als solche erkannt wird sie als Liste der entlanggegangenen Knoten und Bauteile in einer Liste mit allen Maschen gespeichert. Mögliche lineare Abhängigkeit kann hierbei ignoriert werden, die Matrix wird am Ende einfach Nullzeilen aufweisen. Diese Nullzeilen können zudem als Überprüfung der Rechnung genutzt werden (denkbar ist auch ein direktes eintragen einer gefundenen Masche in die Matrix).

Der Nächste Schritt ist das gaussen der Matrix, sowie die Berechnung der in der Matrix vorkommenden Variablen. Diese Können dann zur Berechnung der restlichen Unbekannten genutzt werden. 

Am Ende werden die Ergebnisse auf der Konsole ausgegeben.

Leistungsmerkmale:
	- Einlesen von Netzwerk aus Datei  *
	- Verwaltung von Netzwerk als Graph ***
	- Finden der Maschen in eingelesenem Netzwerk ***
	- Aufstellen einer Matrix aus Maschengleichungen und Knotengleichungen **
	- Gaussen der Matrix **
	- Berrechnen der Variablen der Matrix, wenn notwendig in Abhängigkeit von freien Variablen ***
	- Berechnen der restlichen Unbekannten **
(Sterne sind geschätzter Schwierigkeitgrad / Arbeitsaufwand von 1 (leicht) bist 3 (schwer))


wichtige Datentypen:
----------------------------

Knoten sind einfach int

"Spaltenbeschriftung" der Matrix:
variable
	- Bauteil  *v_teil
	- Char v_typ

Element einer Masche:
Maschenabschnitt
	- Bauteil *m_teil
	- Int m_knoten

Rückgabewert von spannung(int knoten) und strom(int knoten) funktionen:
Matrix_elem
	- Char v_typ
	- Double faktor

Bauteil
	- String name
	- Char typ
	- Int knoten1
	- Int knoten2
	- Double volt
	- Double ampere
	- Double ohm
	- Bool v_known
	- Bool a_known
	- Bool o_known
	- Int n_known

Listen 
Standard Listen mit verschiedenen Inhalten
Doppelt verknüpft und so.

Map 
Wahrscheinlich realisiert als Liste aus Paaren aus Liste von Bauteilen und Knoten
(hat bei Suche und Elementzugriff nur O(n) Komplexität, statt O(log n), aber das sind Hashtabellen nicht wert)

Matrix
Liste aus Zeilen aus Arrays
Sowie liste die Spaltenbeschriftung der Matrix enthält, bestehend aus Struct variable

Funktionen:
----------------

Bauteil:

Void setup (Bauteil *this, char typ, char *name, int knoten1, int knoten2)
Quasi Konstruktor

Void set_volt(double wert)
Setzt volt, v_known und zählt n_known + 1

Analog dazu:
Void set_ampere(double wert)
Void set_ohm(double wert)

Char var_gesucht()
Gibt als char zurück, ob volt ('u'), ampere ('i') oder beide ('b') in die Matrix sollen
Prüft ggf. zudem auf Sinn der gegeben var, und rechet bei zwei var die dritte aus

Matrix_elem spannung(int knoten)
Gibt wenn spannung gesucht ist Variablen in die eingetragen werden soll + wert für matrix zurück

Analog dazu:
Matrix_elem strom(int knoten)

Netzwerk:

Void build_netzwerk()
Liest Netzwerk aus Datei ein

Void build_matrix()
Baut Matrix aus gesuchten Variablen, füllt Matrix dann mit Knotengleichungen
Und Maschengleichungen

Void finde_maschen(liste masche)
Wird in build_matrix aufgerufen um Maschen zu finden 

Void berechne_netwerk()
Gausst Matrix, berechnet variablen der Matrix

Void draw_matrix()
Gibt Matrix auf Bildschirm aus (hauptsächlich debugging)

