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

director::director(){edge=0;};

void director::CreateGame()
{
	int InputChoice;

	fflush(stdin);

	cout<<"Welcome to Doctor-Virus Game\n"
		<<"Press 1 to read data from file"<<endl
		<<"2 to enter data now\n"<<endl;
	cin>>InputChoice;

	int x,y,hn=0,dn=0,vn=0,i,j;
	char type;

	if(InputChoice==1)
	{
		FILE *dvptr;
		if((dvptr=fopen("params.in","r"))==NULL)
			cout<<"There is no file";

		else
		{
			fscanf(dvptr,"%d%d%d%d%d",&edge,&VirusNumber,&HumanNumber,&DoctorNumber,&TurnNumber);
			board=(char**)malloc(sizeof(char*)*edge);
	
			for(i=0;i<edge;i++)
			{
				board[i]=(char*)malloc(sizeof(char)*edge);
			}
			for(i=0;i<edge;i++)
				for(j=0;j<edge;j++)
					board[i][j]=' ';// the board is created

			V=(virus*)malloc(sizeof(V)*VirusNumber);
			H=(human*)malloc(sizeof(H)*HumanNumber);
			D=(doctor*)malloc(sizeof(D)*DoctorNumber);
			

			while(!feof(dvptr))
			{
				fscanf(dvptr,"%d%d%c",&x,&y,&type);
				
				if(type=='H')
				{
					H[hn].x=x;
					H[hn].y=y;				
					H[hn].SicknessTime=0;
					H[hn].isInfected=false;
					hn++;
				}
				if(type=='D')
				{
					D[dn].x=x;
					D[dn].y=y;	
					D[dn].SicknessTime=0;
					D[dn].isInfected=false;
					dn++;
				}
				if(type=='V')
				{
					V[vn].x=x;
					V[vn].y=y;
					vn++;
				}
			}
		}
	}
	else if(InputChoice==2)
	{	
		while(1)
		{
			fflush(stdin);
			cout<<"Enter the edge of the game board(values from 1 to 20 are possible)\n";
			cin>>edge;
			if(edge<20 && edge>0)
				break;
			cout<<"\nError";
		}
		while(1)
		{
			fflush(stdin);
			cout<<"Enter the number of doctors:";
			cin>>DoctorNumber;
			if(DoctorNumber<20 && DoctorNumber>=0)
				break;
			cout<<"\nError";
		}

		while(1)
		{
			fflush(stdin);
			cout<<"\nEnter the number of humans:";
			cin>>HumanNumber;
			if(HumanNumber<20 && HumanNumber>=0)
				break;
			else
			    cout<<"\nError"<<endl;
		}
		while(1)
		{
			fflush(stdin);	
			cout<<"\nEnter the number of viruses:";
			cin>>VirusNumber;
			if(VirusNumber<20 && VirusNumber>=0)
				break;
			cout<<"\nError"<<endl;
		}
		while(1)
		{
			cout<<"\nEnter the turn number:";
			cin>>TurnNumber;
			if(TurnNumber>0)
				break;
			cout<<"\nError";
		}
		board=(char**)malloc(sizeof(char*)*edge);
		for(i=0;i<edge;i++)
		{
			board[i]=(char*)malloc(sizeof(char)*edge);
		}

		for(i=0;i<edge;i++)
			for(j=0;j<edge;j++)
				board[i][j]=' ';// the board is created

		V=(virus*)malloc(sizeof(V)*VirusNumber);
		H=(human*)malloc(sizeof(H)*HumanNumber);
		D=(doctor*)malloc(sizeof(D)*DoctorNumber);

		for(vn=0;vn<VirusNumber;vn++)
		{
			V[vn].x=Locator(edge);
			V[vn].y=Locator(edge);
		}
		for(dn=0;dn<DoctorNumber;dn++)
		{
			D[dn].x=Locator(edge);
			D[dn].y=Locator(edge);
			D[dn].SicknessTime=0;
			D[dn].isInfected=false;
		}
		for(hn=0;hn<HumanNumber;hn++)
		{
			H[hn].x=Locator(edge);
			H[hn].y=Locator(edge);
			H[hn].SicknessTime=0;
			H[hn].isInfected=false;
		}
	}
	else
		cout<<"Error\n";

	BoardDesigner();
}

void director::BoardDesigner()
{
	int vn,dn,hn;
	for(dn=0;dn<DoctorNumber;dn++)
		board[D[dn].x][D[dn].y]='D';
	for(hn=0;hn<HumanNumber;hn++)
		board[H[hn].x][H[hn].y]='H';
	for(vn=0;vn<VirusNumber;vn++)
		board[V[vn].x][V[vn].y]='V';
	dn=0;
	hn=0;
	for(vn=0;vn<VirusNumber;vn++)
	{
		if(V[vn].x==H[hn].x && V[vn].y==H[hn].y)
		{
			H[hn].isInfected=true;
			board[H[hn].x][H[hn].y]='X';
		}
		if(D[dn].x==V[vn].x && V[vn].y==D[dn].y)
		{
			D[dn].isInfected=true;
			board[D[dn].x][D[dn].y]='S';
		}	
		dn++;
		hn++;
	}
}
void director::Simulation()
{
	int i,j,m,illh=0,illd=0;
	int turn=0;
	char terminate;
	int hn=0,dn=0,vn=0,hn2=0,dn2=0;
	
	cout<<"How frequently do you want to stop the game?(per how many turns)\n";
	cin>>stopper;

	while(turn<TurnNumber)
	{
		while(hn<HumanNumber)
		{
			
			if(H[hn].isInfected==1)
			{
				H[hn].SicknessTime++;
				if(H[hn].SicknessTime<10)
					DoctorFinder(turn,hn);
			}
			if(H[hn].isInfected==0)
			{
				m=movement();
				board[H[hn].x][H[hn].y]=' ';
				for(vn=0;vn<VirusNumber;vn++)
					if(H[hn].x==V[vn].x && H[hn].y==V[vn].y)				
						board[H[hn].x][H[hn].y]='V';						
				vn=0;

				for(dn=0;dn<DoctorNumber;dn++)
					if(H[hn].x==D[dn].x && H[dn].y==D[dn].y)
					{
						board[H[hn].x][H[hn].y]='D';
						if(D[dn].isInfected)
							board[D[dn].x][D[dn].y]='S';
					}
				dn=0;

				for(hn2=0;hn2<HumanNumber;hn2++)
					if(H[hn].x==H[hn2].x && H[hn].y==H[hn2].y && hn2!=hn)
					{
						board[H[hn2].x][H[hn2].y]='H';	
						if(H[hn2].isInfected)
							board[H[hn2].x][H[hn2].y]='X';
					}
				if(m==0)			
				    H[hn].x--;
				if(m==1)
				    H[hn].x++;
				if(m==2)
					H[hn].y--;
				if(m==3)
					H[hn].y++;
				if(H[hn].x==edge)
					H[hn].x--;
				if(H[hn].x==-1)
					H[hn].x++;
				if(H[hn].y==edge)
					H[hn].y--;
				if(H[hn].y==-1)
					H[hn].y++;
				for(vn=0;vn<VirusNumber;vn++)
					if(H[hn].x==V[vn].x && H[hn].y==V[vn].y)
						H[hn].isInfected=true;
				for(hn2=0;hn2<HumanNumber;hn2++)
					if(H[hn2].x==H[hn].x && H[hn2].y==H[hn].y && hn!=hn2 && H[hn2].isInfected==1)
						H[hn].isInfected=true;
				for(dn=0;dn<DoctorNumber;dn++)
					if(D[dn].x==H[hn].x && H[hn].y==D[dn].y && D[dn].isInfected==1)
						H[hn].isInfected=true;
				board[H[hn].x][H[hn].y]='H';
				if(H[hn].isInfected)
					board[H[hn].x][H[hn].y]='X';
			}
		sleep();
		system("cls");
		displayer();		
		hn++;		
		}
		dn=0;
		while(dn<DoctorNumber)
		{
	
			board[D[dn].x][D[dn].y]=' ';
			for(vn=0;vn<VirusNumber;vn++)
				if(D[dn].x==V[vn].x && D[dn].y==V[vn].y)
					board[D[dn].x][D[dn].y]='V';

			for(dn2=0;dn2<DoctorNumber;dn2++)
				if(D[dn].x==D[dn2].x && D[dn].y==D[dn2].y && dn2!=dn)
				{
					board[D[dn].x][D[dn].y]='D';
					if(D[dn2].isInfected)
						board[D[dn2].x][D[dn2].y]='S';
				}

			for(hn=0;hn<HumanNumber;hn++)
				if(D[dn].x==H[hn].x && D[dn].y==H[hn].y)
				{
					board[D[dn].x][D[dn].y]='H';
					if(H[hn].isInfected)
						board[H[hn].x][H[hn].y]='X';
				}
				
			if(D[dn].isInfected==0 || (D[dn].isInfected==1 && turn%2==1))
			{
				
				m=movement();
				if(m==0)			
				    D[dn].x--;
				if(m==1)
				    D[dn].x++;
				if(m==2)
					D[dn].y--;
				if(m==3)
					D[dn].y++;
				if(D[dn].x==edge)
					D[dn].x--;
				if(D[dn].x==-1)
					D[dn].x++;
				if(D[dn].y==edge)
					D[dn].y--;
				if(D[dn].y==-1)
					D[dn].y++;
				for(vn=0;vn<VirusNumber;vn++)
				{
					if(D[dn].x==V[vn].x && D[dn].y==V[vn].y)
						D[dn].isInfected=true;
				}
				board[D[dn].x][D[dn].y]='D';
			}
			if(D[dn].isInfected==1)
			{
				D[dn].SicknessTime++;
				board[D[dn].x][D[dn].y]='S';
			}
			if(D[dn].SicknessTime==5)
			{
				D[dn].SicknessTime=0;
				D[dn].isInfected=false;
				board[D[dn].x][D[dn].y]='D';
			}
			for(hn=0;hn<HumanNumber;hn++)
				if(D[dn].isInfected==0 && D[dn].x==H[hn].x && D[dn].y==H[hn].y && H[hn].isInfected==1)			
					H[hn].isInfected=false;

			for(dn2=0;dn2<DoctorNumber;dn2++)
				if(D[dn].isInfected==0 && D[dn].x==D[dn2].x && D[dn].y==D[dn2].y && D[dn2].isInfected==1)			
					D[dn2].isInfected=false;			
			system("cls");
			dn++;
			displayer();				
			sleep();
		}
		for(i=0;i<HumanNumber;i++)
			if(H[i].isInfected)
				illh++;
		for(i=0;i<DoctorNumber;i++)
			if(D[i].isInfected)
				illd++;
			cout<<"Turn Report"<<endl
				<<illh<<" infected humans"<<endl
				<<(HumanNumber-illh)<<" not infected humans"<<endl
				<<illd<<" infected doctors"<<endl
				<<(DoctorNumber-illd)<<" not infected doctors"<<endl
				<<VirusNumber<<" viruses"<<endl;
		sleep();
		system("cls");
		turn++;
		hn=0;
		illh=0;
		illd=0;
		dn=0;
		vn=0;
		if(turn%stopper==0)
		{
			cout<<"Do you want to exit?"<<endl
				<<"y to exit, any other key to return game"<<endl;
			cin>>terminate;
			if(terminate=='y')
				turn=TurnNumber;
		}
	}
}

int director::Locator(int edge)
{
	time_t t;
	int l;
	srand((unsigned) time(&t));
	l=rand();
	Sleep(99);
    return l%edge;
}

int director::movement()
{	
	time_t t;
	int m;
	srand((unsigned) time(&t));
	m=rand();
    return m%4;
}

void director::sleep()
{

	Sleep(2000);
}

void director::DoctorFinder(int turn,int hn)
{
	int dn=0,m;
	int minx=100;
	int miny=100;

	int vn=0,hn2=0;

	if(turn%2==1)
	{
		board[H[hn].x][H[hn].y]=' ';
		for(vn=0;vn<VirusNumber;vn++)
			if(H[hn].x==V[vn].x && H[hn].y==V[vn].y)
				board[H[hn].x][H[hn].y]='V';

		for(dn=0;dn<DoctorNumber;dn++)
			if(D[dn].x==H[hn].x && D[dn].y==H[hn].y)
			{
				board[D[dn].x][D[dn].y]='H';
				if(H[hn].isInfected)
					board[H[hn].x][H[hn].y]='X';
			}

		for(hn2=0;hn2<HumanNumber;hn2++)
			if(H[hn].x==H[hn2].x && H[hn].y==H[hn2].y && hn2!=hn)
			{
				board[H[hn2].x][H[hn2].y]='H';	
				if(H[hn2].isInfected)
					board[H[hn2].x][H[hn2].y]='X';
			}

		for(dn=0;dn<DoctorNumber;dn++)
		{
			if(D[dn].isInfected==0 &&(minx*minx+miny*miny)>((D[dn].x-H[hn].x)*(D[dn].x-H[hn].x)+(D[dn].y-H[hn].y)*(D[dn].y-H[hn].y)))
			{
				minx=D[dn].x;
				miny=D[dn].y;
			}
			dn++;
		}
		if(dn==0)
		{
			m=movement();
				if(m==0)			
				    H[hn].x--;
				if(m==1)
				    H[hn].x++;
				if(m==2)
					H[hn].y--;
				if(m==3)
					H[hn].y++;
				if(H[hn].x==edge)
					H[hn].x--;
				if(H[hn].x==-1)
					H[hn].x++;
				if(H[hn].y==edge)
					H[hn].y--;
				if(H[hn].y==-1)
					H[hn].y++;
		}
		else
		{
			if(H[hn].x-minx>0)
				H[hn].x--;
			if(H[hn].x-minx<0)
				H[hn].x++;
			if(H[hn].x==minx)
			{
				if(H[hn].y<miny)
					H[hn].y++;
				if(H[hn].y>miny)
					H[hn].y--;
				if(H[hn].x==minx && H[hn].y==miny)
					H[hn].isInfected=false;
			}
		}
	}
	if(H[hn].isInfected)
		board[H[hn].x][H[hn].y]='X';
	else
		board[H[hn].x][H[hn].y]='H';
}
void director::displayer()
{
	//ASCI CODES
	char VerticalLine=179;
    char HorizontalLine=196;

	int i,j,k;
	
	cout<<"  "<<VerticalLine<<" ";
	for(k=0;k<edge;k++)
	{	
		if(k>=9)
			cout<<k<<VerticalLine;
		else
			cout<<k<<VerticalLine<<" ";
		
	}
	cout<<endl;
	for(k=0;k<3*edge+2;k++)
		cout<<HorizontalLine;

	
	cout<<"\n";	
	for(i=0;i<edge;i++)
	{
		if(i<10)
			cout<<i<<" "<<VerticalLine;
		else
			cout<<i<<VerticalLine;
		for(j=0;j<edge;j++)
		{
			if(board[i][j]=='X')
				cout<<"HV"<<VerticalLine;
			else if(board[i][j]=='S')
				cout<<"DV"<<VerticalLine;
			else
				cout<<board[i][j]<<" "<<VerticalLine;
		}
		cout<<"\n";
		for(k=0;k<3*edge+2;k++)
			cout<<HorizontalLine;		
		cout<<endl;
	}
}