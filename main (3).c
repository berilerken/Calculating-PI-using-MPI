#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{

    double PI = 3.141592653589793238462643;
    int finish = 0, n, myid, numprocs, i;
    double my_pi, pi, h, sum, x;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    while (!finish)
    {
        if (myid == 0)
        {
            printf("Enter the number of intervals: (press 0 for quit) ");
            scanf_s("%d", &n);
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n == 0) break;

        h = 1.0 / (double)n;
        sum = 0.0;
        for (i = myid + 1; i <= n; i += numprocs) {
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x * x);
        }
        my_pi = h * sum;

        MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,
                   MPI_COMM_WORLD);

        if (myid == 0)
            printf("PI NUMBER: %.16f, Error: %.16f\n",
                   pi, fabs(pi - PI));
    }

    MPI_Finalize();
    return 0;

}
