/* Simple 2D Ising simulation
   Written by Robert C. Helling <helling@atdotde.de>

   Published under GPL 2.0

   Compile as clang -O4 ising.c -lm -o ising
*/
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>        //For cursor control
#define SIZE 200
#define PLOTSIZE 48         //Use less than 49 otherwise screen overflows
#define torus(a) (a < 0 ? a + SIZE : (a >= SIZE ? a - SIZE : a))        //Defines how a torus behaves



using namespace std;

int grid[SIZE][SIZE];


/*Turns the cursor on/off*/
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

/*Sets cursor to the top left position to overwrite terminal output*/
void InitCurs(){


	/*Initialise objects for cursor manipulation*/
    HANDLE hStdout;
    COORD destCoord;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    /*Sets coordinates of cursor to (0,0) */
    destCoord.X = 0;
    destCoord.Y = 0;
    SetConsoleCursorPosition(hStdout, destCoord);

    ShowConsoleCursor(false);

}


void init(){
	int x,y;
	double p = 0.3;
	FILE *mag;

	mag = fopen("magnetization.txt","w");           //Overwrites pre-existing file or creates one
	fclose(mag);

    srand(time(NULL));                                  //Seeds rand() with pseudo-random initial integer

	for (x=0; x<SIZE; x++){                             //Creates random matrix of size SIZE x SIZE with entries -1 and 1, where P(-1) = p and P(1) = 1-p
		for (y=0; y<SIZE; y++){
            if(rand()/(float)RAND_MAX < p ){
                grid[x][y] = -1;
            }else{
                grid[x][y] = 1;
            }
		}
    }

}

/*Outputs sublattice and average magnetisation*/
void output(){
	int x,y;
	int m=0;
	FILE *mag;


    /*Prints sublattice into terminal with * for spin +1 and . for spin -1*/

	for (x=0; x<PLOTSIZE; x++){
		for (y=0; y<PLOTSIZE*2; y++){
			if (grid[x][y] == 1){
				putchar('*');
			}else{
				putchar('.');
			}
		}
		putchar('\n');
	}

	/*Sums spins of lattice*/
	for (x=0; x<SIZE; x++)
        {
        for (y=0; y<SIZE; y++)
			m += grid[x][y];
        }

	cout << "m="  << m/(float)(SIZE * SIZE) << "\n";        //Prints out average magnetisation
	mag = fopen("magnetization.txt","a");
	fprintf(mag, "%f\n", m/(float)(SIZE * SIZE));           //Prints average magnetisation in .txt file
	fclose(mag);
}

/*Update random lattice site*/
void update(float J, float H){

    /*Choose random lattice site*/
	int x = rand()/(float)RAND_MAX * SIZE;
	int y = rand()/(float)RAND_MAX * SIZE;

    /*Calculate Hamiltonian at that lattice site*/
	int sum = grid[x][torus(y+1)] + grid[x][torus(y - 1)] + grid[torus(x + 1)][y] + grid[torus(x - 1)][y];

	double E = 2.0 * grid[x][y] * (J * sum + H);

	/*Change spin at that site if conditions satisfied*/
	if ((E < 0.0) || (exp(-E) > (rand()/(double)RAND_MAX))){
		grid[x][y] *= -1;
	}
}

int main(){

	float T = 1.0;      //Temperature
    float J = 0.44;     //Coupling constant
	float H = 0.001;    //Magnetic field

	long t;

    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);        //Sets terminal windows to windowed full-screen for aesthetic reasons

    init();                         //Creates the initial random configuration of spins


    ShowConsoleCursor(false);       //Turns of blinking cursor for aesthetic reasons



	for (t=0L; t <= 1000; t++){
        InitCurs();                 //Puts cursor in initial position to overwrite output window
        for(int j = 1; j <= 100; j++){  //Update lattice 100 times so that the visualisation looks better
            update(J / T, H / T);
        }
		output();                   //Outputs sublattice in terminal and average magnetisation
    }


	return(0);
}
