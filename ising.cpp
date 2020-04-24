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
#include "cursor.h"         //For cursor control
#define SIZE 200
#define PLOTSIZE 48         //Use less than 49 otherwise screen overflows
#define torus(a) (a < 0 ? a + SIZE : (a >= SIZE ? a - SIZE : a))        //Defines how a torus behaves



using namespace std;

int grid[SIZE][SIZE];


/*Defines random() function for Windows*/
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    int random(){
        return rand();
    }

    void srandom(unsigned seed){
        srand(seed);
    }
#endif



void init(){
	int x,y;                                        //Grid coordinates
	double p = 0.3;                                 //Initial probability of a spin being -1
	FILE *mag;

	mag = fopen("magnetization.txt","w");           //Overwrites pre-existing file or creates one
	fclose(mag);

    srandom(time(NULL));                                  //Seeds rand() with pseudo-random initial integer

	for (x=0; x<SIZE; x++){                             //Creates random matrix of size SIZE x SIZE with entries -1 and 1, where P(-1) = p and P(1) = 1-p
		for (y=0; y<SIZE; y++){
            if(random()/(float)RAND_MAX < p ){
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
				cout << "*";
			}else{
				cout << ".";
			}
		}
		cout << "\n";
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
	int x = random()/(float)RAND_MAX * SIZE;
	int y = random()/(float)RAND_MAX * SIZE;

    /*Calculate Hamiltonian at that lattice site*/
	int sum = grid[x][torus(y+1)] + grid[x][torus(y - 1)] + grid[torus(x + 1)][y] + grid[torus(x - 1)][y];

	double E = 2.0 * grid[x][y] * (J * sum + H);

	/*Change spin at that site if conditions satisfied*/
	if ((E < 0.0) || (exp(-E) > (random()/(double)RAND_MAX))){
		grid[x][y] *= -1;
	}
}

int main(){

    float J = 0.44;     //Coupling constant
	float T = 1.0;      //Temperature
	float H = 0.001;    //Magnetic field

	long t;


	/*Turn off if you are doing multiple Simulation!*/
    Fullscreen(true);        //Sets terminal windows to windowed full-screen for aesthetic reasons.



    init();                         //Creates the initial random configuration of spins


    ShowConsoleCursor(false);       //Turns of blinking cursor for aesthetic reasons



	for (t=0L; t <= 1000; t++){
        InitCurs();                 //Puts cursor in initial position to overwrite output window
        for(int j = 1; j <= 1000; j++){  //Update lattice 100 times so that the visualisation looks better
            update(J / T, H / T);
        }
		output();                   //Outputs sublattice in terminal and average magnetisation
    }


	return(0);
}
