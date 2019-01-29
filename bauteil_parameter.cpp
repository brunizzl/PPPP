#include "bauteil_parameter.h"



bauteil_parameter::bauteil_parameter()
	:status(unbekannt)
{
}

bauteil_parameter bauteil_parameter::operator+(const bauteil_parameter & summand2)
{
	if (!this->bekannt() || !summand2.bekannt()) {
		std::cout << "Fehler: bauteil_parameter ist unbekannt, soll aber verrechnet werden (operator +)" << '\n';
	}
	bauteil_parameter rueckgabe;

	if (this->status == s_bekannt || summand2.status == s_bekannt) {
		rueckgabe.status = s_bekannt;

		if (this->status == s_bekannt) {
			rueckgabe.s_wert = this->s_wert;
		}
		else {
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert = DoubleZuBruchStr(this->d_wert, false);
			rueckgabe.s_wert += ')';
		}

		if (summand2.status == s_bekannt) {
			if (summand2.s_wert[0] != '-') {
				rueckgabe.s_wert += '+';
			}
			rueckgabe.s_wert += summand2.s_wert;
		}
		else if (summand2.d_wert != 0) {
			rueckgabe.s_wert += DoubleZuBruchStr(summand2.d_wert, true);
		}
	}
	else {	//beide werte sind als double bekannt
		rueckgabe.status = d_bekannt;
		rueckgabe.d_wert = this->d_wert + summand2.d_wert;
	}

	return rueckgabe;
}

bauteil_parameter bauteil_parameter::operator-(const bauteil_parameter & subtraktor)
{
	if (!this->bekannt() || !subtraktor.bekannt()) {
		std::cout << "Fehler: bauteil_parameter ist unbekannt, soll aber verrechnet werden (operator -)" << '\n';
	}
	bauteil_parameter rueckgabe;

	if (this->status == s_bekannt || subtraktor.status == s_bekannt) {
		rueckgabe.status = s_bekannt;

		if (this->status == s_bekannt) {
			rueckgabe.s_wert = this->s_wert;
		}
		else {
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert = DoubleZuBruchStr(this->d_wert, false);
			rueckgabe.s_wert += ')';
		}

		if (subtraktor.status == s_bekannt) {
			rueckgabe.s_wert += '-';
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert += subtraktor.s_wert;
			rueckgabe.s_wert += ')';
		}
		else if (subtraktor.d_wert != 0) {
			rueckgabe.s_wert += DoubleZuBruchStr(subtraktor.d_wert * -1, true);
		}
	}
	else {	//beide werte sind als double bekannt
		rueckgabe.status = d_bekannt;
		rueckgabe.d_wert = this->d_wert - subtraktor.d_wert;
	}

	return rueckgabe;
}

bauteil_parameter bauteil_parameter::operator*(const bauteil_parameter & faktor2)
{
	if (!this->bekannt() || !faktor2.bekannt()) {
		std::cout << "Fehler: bauteil_parameter ist unbekannt, soll aber verrechnet werden (operator *)" << '\n';
	}
	bauteil_parameter rueckgabe;

	if (this->status == s_bekannt || faktor2.status == s_bekannt) {
		rueckgabe.status = s_bekannt;

		if (this->status == s_bekannt) {
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert += this->s_wert;
			rueckgabe.s_wert += ')';
		}
		else {
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert = DoubleZuBruchStr(this->d_wert, false);
			rueckgabe.s_wert += ')';
		}

		if (faktor2.status == s_bekannt) {
			rueckgabe.s_wert += '*';
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert += faktor2.s_wert;
			rueckgabe.s_wert += ')';
		}
		else if (faktor2.d_wert != 0) {
			rueckgabe.s_wert += '*';
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert += DoubleZuBruchStr(faktor2.d_wert, false);
			rueckgabe.s_wert += ')';
		}
	}
	else {	//beide werte sind als double bekannt
		rueckgabe.status = d_bekannt;
		rueckgabe.d_wert = this->d_wert * faktor2.d_wert;
	}

	return rueckgabe;
}

bauteil_parameter bauteil_parameter::operator/(const bauteil_parameter & divisor)
{
	if (!this->bekannt() || !divisor.bekannt()) {
		std::cout << "Fehler: bauteil_parameter ist unbekannt, soll aber verrechnet werden (operator /)" << '\n';
	}
	bauteil_parameter rueckgabe;

	if (this->status == s_bekannt || divisor.status == s_bekannt) {
		rueckgabe.status = s_bekannt;

		if (this->status == s_bekannt) {
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert += this->s_wert;
			rueckgabe.s_wert += ')';
		}
		else {
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert = DoubleZuBruchStr(this->d_wert, false);
			rueckgabe.s_wert += ')';
		}

		if (divisor.status == s_bekannt) {
			rueckgabe.s_wert += '/';
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert += divisor.s_wert;
			rueckgabe.s_wert += ')';
		}
		else if (divisor.d_wert != 0) {
			rueckgabe.s_wert += '/';
			rueckgabe.s_wert += '(';
			rueckgabe.s_wert += DoubleZuBruchStr(divisor.d_wert, false);
			rueckgabe.s_wert += ')';
		}
	}
	else {	//beide werte sind als double bekannt
		rueckgabe.status = d_bekannt;
		rueckgabe.d_wert = this->d_wert / divisor.d_wert;
	}

	return rueckgabe;
}


bool bauteil_parameter::addition(std::string & testausdruck)
{
	//idee: wenn plus oder minus gefunden wird false, bei minus vorher noch testen, ob nur vorzeichen (klammer auf davor)
	///////////////////////////////muss noch geschrieben werden
	return false;
}

bauteil_parameter::bauteil_parameter(double d_wert)
	:d_wert(d_wert), status(d_bekannt)
{
}

bool bauteil_parameter::bekannt() const
{
	return (status == d_bekannt || status == s_bekannt);
}

void bauteil_parameter::operator= (double wert)
{
	status = d_bekannt;
	d_wert = wert;
	return;
}

void bauteil_parameter::operator= (const std::string & wert)
{
	status = s_bekannt;
	s_wert = wert;
	return;
}
