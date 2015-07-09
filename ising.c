/* Simple 2D Ising simulation
   Written by Robert C. Helling <helling@atdotde.de>
   
   Published under GPL 2.0

   Compile as clang -O4 ising.c -lm -o ising
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 600
#define PLOTSIZE 80
#define torus(a) (a < 0 ? a + SIZE : (a >= SIZE ? a - SIZE : a))

int grid[SIZE][SIZE];

void init(){
	int x,y;
	FILE *mag;

	mag = fopen("/tmp/magnetization","w");
	fclose(mag);

	for (x=0; x<SIZE; x++)
		for (y=0; y<SIZE; y++)
			grid[x][y] = random()/(float)RAND_MAX < 0.3  ? -1 : 1;

	srandom(time(NULL));
}

void output(float H){
	int x,y;
	int m=0;
	FILE *mag;

    /* Cursor home */
	printf("\033[1;1H");
	for (x=0; x<PLOTSIZE; x++){
		for (y=0; y<PLOTSIZE*2; y++){
			if (grid[x][y] == 1)
				putchar('*');
			else
				putchar('.');
		}
		putchar('\n');
	}

	for (x=0; x<SIZE; x++)
		for (y=0; y<SIZE; y++)
			m += grid[x][y];
	
	printf("m=%f\n",m/(float)(SIZE * SIZE));
	mag = fopen("/tmp/magnetization","a");
    fprintf(mag, "%f %f\n",log(H), m/(float)(SIZE * SIZE));
	fclose(mag);
}

void update(float J, float H){
	int x = random() * SIZE/RAND_MAX;
	int y = random() * SIZE/RAND_MAX;

	int sum = grid[x][torus(y+1)] + grid[x][torus(y - 1)] + grid[torus(x + 1)][y] + grid[torus(x - 1)][y];

	double E = 2.0 * grid[x][y] * (J * sum + H);

	if ((E < 0.0) || (exp(-E) > (random()/(double)RAND_MAX)))
		grid[x][y] *= -1;
}
		

int main(){

	float T = 1.0;
    float J = 1.0/2.296185;
    float H = 1.0;
	
	long t;

	init();

	for (t=0L; 1; t++){
		update(J / T, H / T);
        if(!(t % 100000)){
            output(H);
            H *= .99;
		}
	}
	return(0);
}
