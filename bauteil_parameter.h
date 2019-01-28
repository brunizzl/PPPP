#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include "doubleBruch.h"

enum parameter_status {
	d_bekannt,		//double bekannt
	s_bekannt,		//string bekannt
	unbekannt,		//unbekannt wird gesucht
	freie_var		//unbekannt und wird mit geringer prio gesucht (in berechnung als freie var bevorzugt)
};


class bauteil_parameter
{
private:
	bool addition(std::string &testausdruck);
public:
	double d_wert;
	std::string s_wert;
	parameter_status status;

	bauteil_parameter(double d_wert);
	bauteil_parameter();

	bauteil_parameter operator+ (const bauteil_parameter& summand2);            //definieren rechenregeln 
	bauteil_parameter operator- (const bauteil_parameter& subtraktor);			//alle strings beginnen mit vorzeichen (auch ggf. plus) (gerne optimieren)
	bauteil_parameter operator* (const bauteil_parameter& faktor2);
	bauteil_parameter operator/ (const bauteil_parameter& divisor);

	bool bekannt() const;
	void operator= (double wert);
	void operator= (const std::string &wert);

	
};

