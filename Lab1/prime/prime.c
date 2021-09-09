#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>


int main(int argc, char** argv){
MPI_Init(&argc, &argv);

MPI_Comm world = MPI_COMM_WORLD;

int worldSize;
int myRank;

//Find and store world size
MPI_Comm_size(world, &worldSize);
//Find and stpre the cureent rank
MPI_Comm_rank(world, &myRank);

//Argument passed in is stored in argv[1]

//Calculate sqrt of number being tested
int x = atoi(argv[1]);
double root = sqrt((double)x);

int primeCheck = 0;

//variable to split work among nodes
//int split = (int)(root/worldSize);

//printf("The sqrt of %d is %f\n", x, root);

//MPI_Send the number being tested to all nodes
for(int i =1; i<=((int)(root+1)/worldSize); i++)
{
	//MPI_Send(&x, 1, MPI_INT, myRank, 0, MPI_COMM_WORLD);
	//test primality within loop
	//for(int i=0; i<(root/worldSize); i++)
	//{
		//If there is a factor other than the number itself run the loop to output the factors
		if(x % (i*(myRank+1)) == 0 && i*(myRank+1) != 1)
		{
			//The number is not prime
			printf("The number %d is not prime! The factors are %d and %d\n\n",
			x,i*(myRank+1), x/(i*(myRank+1)));
			primeCheck++;
		}
		
	//}
}
if(primeCheck == 0)
{
	printf("No factors found in node %d of %d!\n\n", myRank, worldSize);
}

//MPI finalize
MPI_Finalize();

return 0;
}
