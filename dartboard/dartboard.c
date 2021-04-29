#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

int main(void)
{
    long k, hits=0;
    int circuit=0;
    double tiempos[10];
    const double factor =1.0/RAND_MAX;
    struct timespec begin, end;
    
    int n[]={100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 10000000};
    int tam = sizeof(n)/sizeof(int);
    while (circuit < 20)
    {
       for (int i = 0; i < tam; i++)
        {
            clock_gettime(CLOCK_REALTIME, &begin);
            srand((int)clock());

            for (k=hits=0; k<n[i]; ++k){
                double x = rand()*factor;
                double y = rand()*factor;

                if(x*x + y*y < 1.0){
                    ++hits;
                }
            }
            double pi_aprox=4.0*hits /n[i];
            clock_gettime(CLOCK_REALTIME, &end);
                
            long seconds = end.tv_sec - begin.tv_sec;
            long nanoseconds = end.tv_nsec - begin.tv_nsec;
            double elapsed = seconds + nanoseconds*1e-9;
            tiempos[i]=elapsed;
        }
        circuit++;
        

        FILE *fp;
        fp=fopen("dartboard_O2.csv","ab");
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
