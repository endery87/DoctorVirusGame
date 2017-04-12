//Ender Yunüs
//2004101303
//CMPE
#include<iostream>
#include<cstdlib>
#include<cmath>
#include<windows.h>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;

#include "controller.h"
#include "cvirus.h"
#include "chuman.h"
#include "cdoctor.h"

int main()
{
	director director;
	
	director.CreateGame();
	director.Simulation();

	return 0;
}