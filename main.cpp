#include <iostream>

#include "Netzwerk.h"
#include "resources.h"

int main()
{
    netzwerk Netzwerk;
	if (Netzwerk.build_netzwerk()) {
		Netzwerk.berechne_netzwerk();
		Netzwerk.ergebnisausgabe();
	}
    std::cin.get();
    return 0;
}
