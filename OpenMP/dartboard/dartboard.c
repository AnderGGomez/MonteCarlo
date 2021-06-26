#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>



double launch (int n, double factor){
    long k, hits=0;
    //#pragma omp parallel for
    for (k=hits=0; k<n; ++k){
        double x = rand()*factor;
        double y = rand()*factor;

        if(x*x + y*y < 1.0){
            ++hits;
        }
    }
    return 4.0*hits /n;
}

int main(void)
{
    //long k, hits=0;
    int circuit=0;
    double tiempos[10];
    const double factor =1.0/RAND_MAX;
    struct timespec begin, end;
    double result=0;
    
    int n[]={100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000};
    //int n[]={10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    int tam = sizeof(n)/sizeof(int);
    while (circuit < 20)
    {
        #pragma omp parallel for private(result)
        for (int i=0; i<tam ; i++)
        {
            clock_gettime(CLOCK_REALTIME, &begin);
            srand((int)clock());

            //funcion
            result=launch(n[i], factor);
            

            clock_gettime(CLOCK_REALTIME, &end);
                
            long seconds = end.tv_sec - begin.tv_sec;
            long nanoseconds = end.tv_nsec - begin.tv_nsec;
            double elapsed = seconds + nanoseconds*1e-9;

            printf("result: %f, tiempo: %f, muestra: %d , i: %d\n", result, elapsed, n[i], i);
            tiempos[i]=elapsed;

        }
        
        circuit++;
        
        
        FILE *fp;
        fp=fopen("dartboard_OpenMP_O2.csv","ab");
        for (int i=0; i<tam; i++){
            fprintf(fp, "%.5f,", tiempos[i]);
        }
        fprintf(fp, "%s","\n");
        //fwrite(tiempos,sizeof(int), sizeof(tiempos), fp);
        fclose(fp);
    }
    
    
    //while (1)
    //{
    //n=10000;
        
/*
    
    return 0;*/
}
