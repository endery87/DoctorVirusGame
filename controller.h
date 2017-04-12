#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "cvirus.h"
#include "chuman.h"
#include "cdoctor.h"

class director
{
public:
	director();
	void CreateGame();
	void Simulation();
	void displayer();
	int  movement();
	void sleep();
	void DoctorFinder(int,int);
	int Locator(int);
	void BoardDesigner();
private:
	char **board;
	int edge;
	int HumanNumber;
	int VirusNumber;
	int DoctorNumber;
	int stopper;
	human *H;
	doctor *D;
	virus *V;
	int TurnNumber;
};

#endif