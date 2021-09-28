//Header file that stores algorithms for matrix operations
//#define INDEX[i,j,n,m] i*m +j

//Function for inner product(IP)
int ipMatrix(int* matrixA, int* matrixB, int* sendct)
{
 int ip;	//Var to hold ip
 //For loop to calc ip
 for(int i=0; i<sendct; i++)
 {
  ip += matrixA[i] * matrixB[i];
 }
 return ip;
}

//Function for addition
void addMatrix(int* matrixA, int* matrixB, int* prodMatrix, int sendct)
{
 //Add the arrays and store in new array
 for(int i=0; i<sendct; i++)
 {
  prodMatrix[i] = matrixA[i] + matrixB[i];
 }
 return;
// return prodMatrix;	
}
//Function for subtraction
void subMatrix(int* matrixA, int* matrixB, int* prodMatrix, int sendct)
{
 //Add the arrays and store in new array
 for(int i=0; i<sendct; i++)
 {
  prodMatrix[i] = matrixA[i] - matrixB[i];
 }
 return;
}
//Function for multiplication
void multMatrix(int* matrixA, int* matrixB, int* prodMatrix, int rowA, int colA, int colB)
{
 for(int i=0; i<rowA; i++)
 {
  for(int j=0; j<colB; j++)
  {
   for(int k=0; k<colA; k++)
   {
    prodMatrix[INDEX(i,j,rowA,colB)] += matrixA[INDEX(i,k,rowA,colA)] * matrixB[INDEX(j,k, colA,colB)];
   }
  }
 }
   
 return;
}
//Function for transpose
void transMatrix(int* matrix, int* tMatrix, int row, int col)
{
 for(int i=0; i<col; i++)
 {
  for(int j=0; j<row; j++)
  {
   tMatrix[INDEX(i,j,row,col)] = matrix[INDEX(j,i,row,col)];
  }
 }
 return; 
}


