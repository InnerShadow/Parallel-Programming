
static void FromITOJ (int i, int j, int rakn, int n, int val){

    val = (i + 3) * n + (j + 2) * n;

    if (i < n && j < n){
    if (i < j)
    {
      if (rank == i)

        value = 0;
      MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    if (rank == j)
    {
      MPI_Recv(&value, 1, MPI_Init, rank - 1, 0, MPI_COMM_WORLD, &status);
      //value++;
    }

    if (rank > i && rank < j)
    {
      MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
      //value++;
      MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

    }

  }

  else
  {
    if (rank == i)
    {
      //value = 0;
      MPI_Send(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

    }

    if (rank == j)
    {
      MPI_Recv(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
      //value++;

    }
    if (rank > j && rank < i)
    {
      MPI_Recv(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
      //value++;
      MPI_Send(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

    }
         
  }
}

int main(int argc, char* argv[]){
  int rank, value = 333, n;
  MPI_STATUS status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n);

  int i = 3;
  int j = 1;

    for(int j = 0; j < n; j++){
        if(j != i){
            FromITOJ(i, j, rank, n, value);
        }
    }


}