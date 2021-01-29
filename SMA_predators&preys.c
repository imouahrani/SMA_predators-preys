#include <stdio.h>
#include <stdlib.h>
/*code given in lab2  */

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] =
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0);
    /* divided by 2^32-1 */
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0);
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0);
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void)
{
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6;
    return(a*67108864.0+b)*(1.0/9007199254740992.0);
}
/* These real versions are due to Isaku Wada, 2002/01/09 added */







int i,j;

enum type
{
	PREY,PREDATOR,NONE
};

struct Animal{
	enum type animal_type;
	int old;
	int last_eat;
};
typedef struct Animal Animal;



//create the grid
Animal** init_grid(int n){//on crée une grille de taille n*n
	Animal** retour = (Animal**) malloc(n*sizeof(Animal*));
	//printf("a");
	for (i=0; i<n; i++){
		retour[i] = (Animal*)malloc(n*sizeof(Animal));
		for (j=0; j<n; j++){
			retour[i][j].animal_type = NONE;
		}

	}
	return retour;
}
void print_grid(Animal** grid, int n){
	for (i=0; i< n; i++){
		for (j=0; j< n; j++){
			if (grid[i][j].animal_type == NONE){
				printf(".");
			}
			if (grid[i][j].animal_type == PREDATOR){//
				printf("P");
			}
			if (grid[i][j].animal_type == PREY){
				printf("X");
			}
			if (j==n-1){
				printf("\n");
			}
		}
	}
}

//use it
int count_living_neighbours(int x, int y, int** grid, int grid_size){
	int living_neighbours = 0;
	x= x+grid_size;
	y = y+grid_size;
	if (grid[(x-1)%grid_size][(y-1)%grid_size]==1){
		living_neighbours = living_neighbours+1;
	}
	if (grid[(x-1)%grid_size][y%grid_size]==1){
		living_neighbours = living_neighbours+1;
	}
	if (grid[(x-1)%grid_size][(y+1)%grid_size]==1){
		living_neighbours = living_neighbours+1;
	}
	if (grid[x%grid_size][(y-1)%grid_size]==1){
		living_neighbours = living_neighbours+1;
	}
	if (grid[x%grid_size][(y+1)%grid_size]==1){
		living_neighbours = living_neighbours+1;
	}
	if (grid[(x+1)%grid_size][(y-1)%grid_size]==1){
		living_neighbours = living_neighbours+1;
	}
	if (grid[(x+1)%grid_size][(y+1)%grid_size]==1){
		living_neighbours = living_neighbours+1;
	}
	if (grid[(x+1)%grid_size][y%grid_size]==1){
		living_neighbours = living_neighbours+1;
	}
	return living_neighbours;
}
/*


• Le prédateur mange la proie en croisant avec une probabilité = 90%;
• Le prédateur se déplace deux fois plus vite que sa proie (1 cas à la fois pour une proie), si une proie croise un prédateur, le prédateur prend sa place, (direction = aléatoire sauf avec une proie / un prédateur);
• Si un animal croise un autre de son spèce, un autre est créé (reproduction) avec une probabilité = 50%;
• Le prédateur ne peut survivre que 50 unités de temps;
• Prey ne peut survivre que 60 unités de temps;
• Le prédateur peut manger une proie, la proie mange comme il le faut;
• Si un prédateur traverse deux proies ou plus, seul le plus ancien est mangé, l'autre s'en va;
• Si une proie croise deux prédateurs ou plus, seul le plus jeune peut la manger.
*/
/*rules:
 * 1)Predator eats the prey when crossing with a probability = 90%
 * 2)Predator moves twice faster than prey (1 case at a time for a prey), if a prey crosses a predator, the predator takes it's place, (direction = random unless with a prey/predator)
 * 3)If an animal crosses another from his spiece, another one is created (reproduction) with a probability = 50%
 * 4)Predator can only survive 50 time units
 * 5)Prey can only survive 60 time units
 * 6)Predator can eating a prey, the prey eats as it needs
 * 7)If a predator crosses two or more preys, only the oldest one is eaten, other go away
 * 8)If a prey crosses two or more predators, only the youngest one can eat it
 * */
//déplacement des animaux : 8 directions possibles
int count_eatable_preys(Animal** grid, int x, int y){
	int a;
	int b;
	for (a=x-1; a<x+2; a++){
		for (b=(y-1);b<(y+2); b++){
			if (grid[x][y].animal_type == PREDATOR){
				if (grid[a][b].animal_type == PREY){
					return 1;
				}
			}
		}
	}
	return 0;
 }

Animal** next_step(Animal** grid, int grid_size){
	Animal** retour = init_grid(grid_size);
	for (i=0; i< grid_size; i++){
		for (j=0; j< grid_size; j++){

		}
	}

}
int main(){
	int grid_size;
	printf("entrer la taille de votre grille\n");
	scanf("%d", &grid_size);
	printf("taille de la grille : %d * %d \n", grid_size, grid_size);
	Animal** grid = init_grid(grid_size);




	return 0;
}
