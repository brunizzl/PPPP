# PPPP
ProzProg ProgrammierProjekt
Beschreibung:
------------------

Das Programm soll in der Lage sein, ein beliebiges zeitunabhaengiges elektrisches Netzwerk mit Widerstaenden und Quellen als Komponenten berechnen können.
Das heißt, man gibt dem Programm Parameter einzelner Bauteile, etwa den Strom, die Spannung und/oder den Widerstand eines Widerstandes. Diese Information soll genutzt werden um fehlende Größen anderer Bauteile auszurechnen. Werden zu wenige Größen eingegeben, wird das Ergebnis in Abhaengigkeit nicht bekannter Parameter berechnet.
Das Netzwerk wird als Textdatei eingelesen. Der Nutzer muss in die Datei in einem festgelegten Format jedes Bauteil mit anliegenden Knoten auflisten. Optional dazu können bekannte Bauteilgrößen mit dem Bauteil angegeben werden. Richtungsabhaengige Größen werden dabei immer in Richtung von erstem angegebenen Knoten zu zweitem angegebenen Knoten eingetragen.
Bauteilparameter eines Widerstandes sind:
	- Strom (Richtungsabhaengig, implizit Ampere)
	- Spannung (Richtungsabhaengig, implizit Volt)
	- Widerstand (Richtungsunabhaengig, implizit Ohm)
Bauteilparameter einer Quelle sind:
	- Strom (Richtungsabhaengig, implizit Ampere)
	- Spannung (Richtungsabhaengig, implizit Volt)
Die Quelle wird immer in Richtung einer Spannungsquelle eingetragen. Eine Stromquelle mit (als einziges bekanntem) positivem Strom ist in dieser Notation also eine Spannungsquelle mit unbekannter Spannung, sowie negativem Strom.

Beim Einlesen wird das Programm zwei Strukturen anlegen: eine Liste aller Bauteile, sowie eine Map, die jedem Knoten die direkt verbundenen Bauteile zuordnet. Als Ganzes kann das Programm das Netzwerk dann als Graph behandeln.

Nach dem Einlesen der Bauteile stellt das Programm eine Liste von gesuchten Variablen auf, wobei aber aufgrund des Ohmschen Gesetzes höchstens zwei Parameter eines Widerstandes gesucht werden muessen, der dritte kann aus den anderen berechnet werden.

Um die gesuchten Werte zu berechnen legt das Programm eine Matrix an, wobei jede Spalte fuer eine gesuchte Variable steht (plus Ergebnisspalte). Die Gleichungen der Matrix werden aus den Knotengleichungen jedes Knotens, sowie den Maschengleichungen aufgestellt. Waehrend die an einen Knoten angrenzenden Bauteile bekannt sind, muessen die Maschen erst noch gesucht werden.
Das Programm laeuft um das zu bewerkstelligen am ersten Bauteil los (das muss also mindest eine einfache Verbindung zu dem restlichen Netzwerk haben) und laeuft rekursiv an jedem Knoten in jede weitere Richtung. Ist eine Masche als solche erkannt wird sie als Liste der entlanggegangenen Knoten und Bauteile in einer Liste mit allen Maschen gespeichert. Mögliche lineare Abhaengigkeit kann hierbei ignoriert werden, die Matrix wird am Ende einfach Nullzeilen aufweisen. Diese Nullzeilen können zudem als ueberpruefung der Rechnung genutzt werden (denkbar ist auch ein direktes eintragen einer gefundenen Masche in die Matrix).

Der Naechste Schritt ist das gaussen der Matrix, sowie die Berechnung der in der Matrix vorkommenden Variablen. Diese Können dann zur Berechnung der restlichen Unbekannten genutzt werden.

Am Ende werden die Ergebnisse auf der Konsole ausgegeben.
