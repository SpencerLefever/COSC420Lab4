/*
 * Spencer Lefever
 *COSC420 Lab2 matrix operations 
*/

#define INDEX(i,j,n,m) i*m +j

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include "matrix.h"

//Macros to simplify matric operations
//#define INDEX(i,j,n,m) i*m +j
#define ACCESS(A,i,j) A.data[INDEX(i,j,A.rows, A.cols)]

//Make a struct for a matrix
typedef struct matrix {
int rows;
int cols;
int* data
} matrix;


int main(int argc, char** argv)
{
//MPI Starting commands
MPI_Init(&argc, &argv);
int worldSize, myRank;

MPI_Comm world = MPI_COMM_WORLD;
MPI_Comm_size(world, &worldSize);
MPI_Comm_rank(world, &myRank);

//Initialize matrix struct
matrix A;
A.rows = 1000;
A.cols = 1000;
//A.data = NULL;
A.data = malloc(A.rows*A.cols*sizeof(int));

matrix B;
B.rows = 1000;
B.cols = 1000;
//B.data = NULL;
B.data = malloc(A.rows*A.cols*sizeof(int));

/*
//Only root allocates space for the two matrices
if(myRank == 0)
{
 A.data = malloc(A.rows*A.cols*sizeof(int));
 B.data = malloc(B.rows*B.cols*sizeof(int));
}
*/


//Variables used for file outputs
FILE *handle;
char** fname[256];

//Generate two random vectors of over 1000 elements
if(myRank == 0)
{
//Populate arrays using rand(); using double for loop
for(int i = 0; i<A.rows; i++)
{
	for(int j=0; j<A.cols; j++)
	{
		ACCESS(A,i,j) = rand() % 1000;
	}

}

for(int i = 0; i<B.rows; i++)
{
	for(int j=0; j<B.cols; j++)
	{
		ACCESS(B,i,j) = rand() % 1000;
	}

}

//Print matrix A into file for testing purposes
sprintf(fname, "outfile_mainA.txt");

handle = fopen(fname, "w+");

fprintf(handle, "Begin output for matrix A\n");

for(int i=0; i<A.rows*A.cols; i++)
{
	
	fprintf(handle, "%d ", A.data[i]);
	//if statement used to make new line every A.cols numbers
	if(i%A.cols == 0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);


}


//Print matrix B into a file for testing purposes
sprintf(fname, "outfile_mainB.txt");

handle = fopen(fname, "w+");

fprintf(handle, "Begin output for matrix B\n");

for(int i=0; i<B.rows*B.cols; i++)
{
	
	fprintf(handle, "%d ", B.data[i]);
	//if statement used to make new line every A.cols numbers
	if(i%B.cols == 0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);
/*
//Scatter VectorA
int *recBufA = malloc(((A.rows*A.cols)/worldSize)*sizeof(int));	//Receive buffer for inner prod
printf("Rank %d scattering data\n", myRank);
//Calling MPI_Scatter
MPI_Scatter(
A.data,		//sendbuf 
(A.rows*A.cols)/worldSize,	//sendcount 
MPI_INT, 	//sendtype
recBufA, 	//recvbuf
(A.rows*A.cols)/worldSize,	//recvcount 
MPI_INT,	//recvtype
0, 	//rootrank
world	//comm
); 
printf("Rank %d after scatter\n", myRank);
*/

//Updated scatter uses scatterv instead
int* sendctsA = malloc(worldSize*sizeof(int));
int minSendA = A.rows/worldSize;
int* displsA = malloc(worldSize*sizeof(int));
//Calc num of rows each node will receive
for(int i=0; i<worldSize; i++)
{
 sendctsA[i] = minSendA;
}
for(int i=0; i<A.rows%worldSize; i++)
{
 sendctsA[i]++;
}
for(int i=0; i<worldSize; i++)
{
 sendctsA[i] *= A.cols; 
}
//Calculate displacement
displsA[0] = 0;
for(int i=1; i<worldSize; i++)
{
 displsA[i] = displsA[i-1] + sendctsA[i-1];
}

//begin to scatterv
int* recvbufA = malloc(sendctsA[myRank]*sizeof(int));

MPI_Scatterv(A.data, sendctsA, displsA, MPI_INT, recvbufA, sendctsA[myRank], MPI_INT, 0, world);


//Individual nodes each output their piece of the array to their respective file
sprintf(fname, "outfile_%dA.txt", myRank);

handle = fopen(fname, "w+");

fprintf(handle, "Begin output from processor %d for matrix A\n", myRank);

for(int i=0; i<sendctsA[myRank]; i++)
{
	
	fprintf(handle, "%d ", recvbufA[i]);
	//if statement used to make new line every A.cols numbers
	if(i%A.cols == 0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);
/*
//Scatter VectorB
int *recBufB = malloc(((B.rows*B.cols)/worldSize)*sizeof(int));	//Receive buffer for inner prod

//Calling MPI_Scatter
MPI_Scatter(
B.data,		//sendbuf 
(B.rows*B.cols)/worldSize,	//sendcount 
MPI_INT, 	//sendtype
recBufB, 	//recvbuf
(B.rows*B.cols)/worldSize,	//recvcount 
MPI_INT,	//recvtype
0, 	//rootrank
world	//comm
); 
*/

//Set up scatterv for matrix B

//Updated scatter uses scatterv instead
int* sendctsB = malloc(worldSize*sizeof(int));
int minSendB = B.rows/worldSize;
int* displsB = malloc(worldSize*sizeof(int));
//Calc num of rows each node will receive
for(int i=0; i<worldSize; i++)
{
 sendctsB[i] = minSendB;
}
for(int i=0; i<B.rows%worldSize; i++)
{
 sendctsB[i]++;
}
for(int i=0; i<worldSize; i++)
{
 sendctsB[i] *= B.cols; 
}
/*
if(myRank == 0)
{
 for(int i=0; i<worldSize; i++)
 {
  printf("Will scatter %d elements to rank %d\n",sendctsB[i], i);
 }
}
*/
//Calculate displacemen
displsB[0] = 0;
for(int i=1; i<worldSize; i++)
{
 displsB[i] = displsB[i-1] + sendctsB[i-1];
}
/*
if(myRank==0)
{
 for(int i=0; i<worldSize; i++)
 {
  printf("Start scattering to rank %d at index %d\n", i, displsB[i]);
 }
}
*/
//begin to scatterv
int* recvbufB = malloc(sendctsB[myRank]*sizeof(int));

MPI_Scatterv(B.data, sendctsB, displsB, MPI_INT, recvbufB, sendctsB[myRank], MPI_INT, 0, world);

//Individual nodes each output their piece of the array to their respective file
sprintf(fname, "outfile_%dB.txt", myRank);

handle = fopen(fname, "w+");

fprintf(handle, "Begin output from processor %d for matrix B\n", myRank);

for(int i=0; i<sendctsB[myRank]; i++)
{
	
	fprintf(handle, "%d ", recvbufB[i]);
	//if statement used to make new line every A.cols numbers
	if(i%B.cols == 0 && i!=0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);

/*
//Loop to display what rows each processor is responsible for
for(int i=1; i< worldSize; i++)
{
	printf("Rank %d holding rows %d thru %d\n",i-1, displsB[i-1]/A.cols, displsB[i]/A.cols);
}*/

//Perform the partial inner product

//Allocate array to send back holding the inner product
//int* prodMatrix = malloc((A.rows*B.cols)/worldSize*sizeof(int));

//Nested for loop to find inner product
//For loops are nested this way to try and make 
//the cache more efficient

/*
//Inner Product calculation
for(int i=0; i<A.cols/worldSize; i++)
{
	for(int j=0; j<A.rows/worldSize; j++)
	{
		for(int k=0; k<B.cols/worldSize; k++)
		{
			//Inner product of the to recBuf matrices are execcuted and stored
			//in a new matrix prodMatrix
			prodMatrix[(j*(B.cols/worldSize))+k] += 
			recvbufA[(j*(A.cols/worldSize))+i] * recvbufB[(i*(B.cols/worldSize))+k];
		}
	}
}
*/
//Store partial inner products of each node in a file before gathering
/*
sprintf(fname, "outfile_%dIP.txt", myRank);

handle = fopen(fname, "w+");

fprintf(handle, "Begin output from processor %d for the inner product matrix\n", myRank);

for(int i=0; i<(A.rows/worldSize)*(B.cols/worldSize); i++)
{
	
	fprintf(handle, "%d ", prodMatrix[i]);
	//if statement used to make new line every A.cols numbers
	if(i%B.cols == 0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);
*/

//Write a library of simple matrix operations 
//Addition, Subtraction, mulitplication and tanspose

//TODO
//1. Use time command to record the times
//2. Make library for inner product, addition subtraction multiplication and transpose

//Variables to hold the elapsed time
double bTime;	//time before
double aTime;	//time after
double eTime;	//Elapsed time
//Variable to hold the add function result and var to hold all addResults
int* addResult = malloc(sendctsA[myRank]*sizeof(int));
int* finalAdd = NULL;

if(myRank==0)
{
 finalAdd = malloc(A.rows*A.cols*sizeof(int));
}
//Call add function

bTime = MPI_Wtime();
addMatrix(recvbufA, recvbufB, addResult, sendctsA[myRank]);
aTime = MPI_Wtime();
eTime = aTime- bTime;

printf("Addition operation took %f seconds\n", eTime);

/*
//print add results for testing purposes
printf("Rank %d beginning addResult output\n",myRank);
for(int i=0; i<sendctsA[myRank]; i++)
{
 printf("%4d ", addResult[i]);
}
*/

//Call MPI_Gatherv to gather results of addition
//if(myRank==0)
//{
MPI_Gatherv(
addResult, //sendbuf 
sendctsA[myRank], //sendcount
MPI_INT, //sendtype
finalAdd, //recvbuf
sendctsA, //recvcounts
displsA, //displs
MPI_INT, //recvtype
0, //root 
world //comm
);
//}

//Print out the add result to a file
if(myRank==0)
{
sprintf(fname, "outfile_addResult.txt");

handle = fopen(fname, "w+");

fprintf(handle, "Begin output from processor for addition matrix\n");

for(int i=0; i<A.rows*A.cols; i++)
{
	
	fprintf(handle, "%d ", finalAdd[i]);
	//if statement used to make new line every A.cols numbers
	if(i%A.cols == 0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);
}

//Perform subtraction of the matrices

int* subResult = malloc(sendctsA[myRank]*sizeof(int));
int* finalSub = NULL;

if(myRank==0)
{
 finalSub = malloc(A.rows*A.cols*sizeof(int));
}


//Call sub function
bTime = MPI_Wtime();
subMatrix(recvbufA, recvbufB, subResult, sendctsA[myRank]);
aTime = MPI_Wtime();

eTime = aTime-bTime;

printf("Subtraction operation took %f seconds\n",eTime);

/*
//print sub results for testing purposes
printf("Rank %d beginning subResult output\n",myRank);
for(int i=0; i<sendctsA[myRank]; i++)
{
 printf("%4d ", subResult[i]);
}
*/
//Call MPI_Gatherv to gather results of subtraction
//if(myRank==0)
//{
MPI_Gatherv(
subResult, //sendbuf 
sendctsA[myRank], //sendcount
MPI_INT, //sendtype
finalSub, //recvbuf
sendctsA, //recvcounts
displsA, //displs
MPI_INT, //recvtype
0, //root 
world //comm
);
//}

//Print out the sub result to a file
if(myRank==0)
{
sprintf(fname, "outfile_subResult.txt");

handle = fopen(fname, "w+");


fprintf(handle, "Begin output from processor for subtraction matrix\n");

for(int i=0; i<A.rows*A.cols; i++)
{
	
	fprintf(handle, "%d ", finalSub[i]);
	//if statement used to make new line every A.cols numbers
	if(i%A.cols == 0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);
}

//TODO: Inner product, multiplication and transpose

//Multiplication function execution
//Var to hold resulting matrix
int* multResult = malloc((sendctsA[myRank]/A.cols)*B.cols*sizeof(int));
int* finalMult = NULL;
if(myRank==0)
{
 finalMult = malloc(A.rows*B.cols*sizeof(int));
}
//Initialize all values of finalMult to 0
/*for(int i=0; i<A.rows*B.cols; i++)
{
 multResult[i]=0;
}
*/

//if(myRank==0)
//{
//Broadcast matrix B
MPI_Bcast(
B.data,	//Buffer
B.rows*B.cols,	//Count
MPI_INT,	//datatype
0,	//root
world);	//comm
//}

//printf("Rank %d calling multMatrix function\n", myRank);
//Call multMatrix function
bTime = MPI_Wtime();
multMatrix(recvbufA, B.data, multResult, sendctsA[myRank]/A.cols, sendctsA[myRank]/A.rows, B.cols);
aTime = MPI_Wtime();

eTime = aTime - bTime;
printf("Multiplication operations took %f seconds\n", eTime);
//printf("Rank %d after calling multMatrix function\n", myRank);

//Calculate displacement and send counts for multiplication matrix
int* multDispls=NULL;
if(myRank==0)
{
 multDispls = malloc(worldSize*sizeof(int));
 multDispls[0]=0;
 for(int i=1;i<worldSize;i++)
 {
  multDispls[i] = multDispls[i-1]+ sendctsA[i-1];
 }
}

/*
if(myRank==0)
{
 for(int i=0; i<worldSize; i++)
 {
  printf("Start gathering from rank %d holding index %d\n", i, multDispls[i]);
 }
}
*/
int* multSend=NULL;
if(myRank==0)
{
 multSend = malloc(worldSize*sizeof(int));
 for(int i=0; i<worldSize; i++)
 {
  multSend[i] = (sendctsA[i]/A.cols)*B.cols;
 }
}

/*
//Display multiplication results from each processor for testing purposes
printf("Begin output of multMatrix from rank %d\n",myRank);
for(int i=0; i<(sendctsA[myRank]/A.cols)*B.cols; i++)
{
  printf("%4d ", multResult[i]);
}
*/

//multMatrix(recvbufA, B.data, multResult, sendctsA[myRank]/A.cols, sendctsA[myRank]/A.rows, B.cols);

//Gather results
MPI_Gatherv(
multResult, //sendbuf 
(sendctsA[myRank]/A.cols)*B.cols, //sendcount
MPI_INT, //sendtype
finalMult, //recvbuf
multSend, //recvcounts
multDispls, //displs
MPI_INT, //recvtype
0, //root 
world //comm
);
//printf("Rank %d after calling gatherv\n", myRank);

/*
//Output final mult matrix for testing purposes
if(myRank==0)
{
 printf("Begin output of final multiplication matrix\n");
 for(int i=0; i<A.rows*B.cols; i++)
 {
  printf("%4d ", finalMult[i]);
  if(i%B.cols == 0) printf("\n"); 
 }
}
*/


//Print results to file
if(myRank==0)
{
sprintf(fname, "outfile_multResult.txt");

handle = fopen(fname, "w+");


fprintf(handle, "Begin output from processor for multiplication matrix\n");

for(int i=0; i<A.rows*B.cols; i++)
{
	
	fprintf(handle, "%d ", finalMult[i]);
	//if statement used to make new line every A.cols numbers
	if(i%B.cols == 0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);
}

//Tranpose operations
//Every node has data from the matrix stored on recvbufA
//int* tMatrix = malloc(sendctsA[myRank]*sizeof(int));

//transMatrix(recvbufA, tMatrix, sendctsA[myRank]/A.cols, A.cols);

int* transResult= malloc(A.rows*A.cols*sizeof(int));
/*if(myRank==0)
{
 int* transResult = malloc(A.rows*A.cols*sizeof(int));
}
*/

bTime = MPI_Wtime();
transMatrix(recvbufA, transResult, sendctsA[myRank]/A.cols, A.cols);
aTime = MPI_Wtime();

eTime = aTime - bTime;
printf("Transpose operation took %f seconds\n",eTime);
/*
//Gather results
MPI_Gather(
tMatrix, //sendbuf 
sendctsA[myRank], //sendcount
MPI_INT, //sendtype
transResult, //recvbuf
sendctsA, //recvcounts
MPI_INT, //recvtype
0, //root 
world //comm
);
*/

//Print resulting transpose to a file
if(myRank==0)
{
sprintf(fname, "outfile_tposeResult.txt");

handle = fopen(fname, "w+");


fprintf(handle, "Begin output from processor for transposed matrix\n");

for(int i=0; i<A.rows*A.cols; i++)
{
	
	fprintf(handle, "%d ", transResult[i]);
	//if statement used to make new line every A.cols numbers
	if(i%A.cols == 0)
	{
		fprintf(handle, "\n");
	}
}
fclose(handle);
}

//Compute Inner product

//Allocate arrays for each node and root
int ipResult;
int ipBuf;	//send buffer for MPI_Reduce, holds partial inner products from each node

//Call ip function
bTime = MPI_Wtime();
ipBuf = ipMatrix(recvbufA, recvbufB,sendctsA[myRank]); 
aTime = MPI_Wtime();

eTime = aTime - bTime;
printf("Inner product operation took %f seconds\n",eTime);

//MPI Reduce the sum
int fullIP;
MPI_Reduce(&ipBuf, &fullIP, 1, MPI_INT, MPI_SUM, 0, world); 

//Output IP to std:out
//printf("The inner product of matrixA and Matrix b is %d\n", fullIP);


//printf("Rank %d freeing all malloced data\n", myRank);

//Free all malloc data
if(myRank==0)
{
printf("The inner product of matrixA and MatrixB is %d\n", fullIP);
free(finalAdd);
free(finalSub);
free(finalMult);
free(multDispls);
free(A.data);
free(B.data);
free(transResult);
}
free(recvbufA);
free(recvbufB);
free(addResult);
free(subResult);
free(multResult);
//free(tMatrix);

MPI_Finalize();
return(0);
}

