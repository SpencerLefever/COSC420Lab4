
#include<stdio.h>
#include<mpi.h>


int main(int argc, char** argv){

MPI_Init(&argc, &argv);

//Set world size using MPI_COMM_WORLD constant
MPI_Comm world = MPI_COMM_WORLD;

//Variables needed for hello world task
int worldSize;
int myRank;
char name[MPI_MAX_PROCESSOR_NAME];
int nameLen;

//Call Comm_size function
MPI_Comm_size(world, &worldSize);

//Call MPI function to get rank
MPI_Comm_rank(world, &myRank);
//Call MPI function to get the processor name
MPI_Get_processor_name(name, &nameLen);
//Print hello world with world size, rank and processor name 
printf("Hello World! Processor %d out of %d\n", myRank, worldSize);
printf("Processor name is %s\n",name);

//Call MPI termination function
MPI_Finalize();
return 0;

}
