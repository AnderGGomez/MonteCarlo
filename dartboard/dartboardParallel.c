#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <pthread.h>


struct data
{
    int start;
    int end;
    double factor;
    long A;
};



void limitar (int* limite_hilos,int num_t, int n, int tam){
    int c=n/num_t;
    for (int i = 0; i < num_t; i++)
    {
        limite_hilos[i]=i*c;
    }
    limite_hilos[tam]=n;
    
}

void *tiros (void * arg){
    struct data *p;
    p=(struct data *)arg;
    srand((int)clock());
    long hits=0;
    double x;
    double y;
    
    while(p->start < p->end){
        x = rand()*p->factor;
        y = rand()*p->factor;
        if(x*x + y*y < 1.0){
            ++hits;
        }
        p->start++;
    }
    p->A=hits;
}

int main(void)
{
    struct data t1;
    struct data t2;
    struct data t3;
    struct data t4;

    struct timespec begin, end;
    double tiempos[10];

    long k, hits=0;
    const double factor =1.0/RAND_MAX;

    int num_t=4;
    int limite_hilos[num_t+1];

    int n[]={100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 10000000};
    int tam = sizeof(n)/sizeof(int);
    int circuit = 0;

    while (circuit < 20){
        for (int i = 0; i < tam; i++)
        {
            limitar(limite_hilos, num_t, n[i], num_t);
                        
            t1.start=limite_hilos[0];
            t1.end=limite_hilos[1];
            t1.factor=factor;
            t1.A=0;

            t2.start=limite_hilos[1];
            t2.end=limite_hilos[2];
            t2.factor=factor;
            t2.A=0;

            t3.start=limite_hilos[2];
            t3.end=limite_hilos[3];
            t3.factor=factor;
            t3.A=0;

            t4.start=limite_hilos[3];
            t4.end=limite_hilos[4];
            t4.factor=factor;
            t4.A=0;
            /*for (int  i = 0; i < num_t+1; i++)
            {

                printf("[%d]-> %d",i,limite_hilos[i]);
            }*/
            /*
            for (int  i = 0; i < num_t; i++)
            {
                hits+=tiros(limite_hilos[i], limite_hilos[i+1], factor);
            }*/
            
            pthread_t h1;
            pthread_t h2;
            pthread_t h3;
            pthread_t h4;
            
            clock_gettime(CLOCK_REALTIME, &begin);
            pthread_create(&h1, NULL, tiros, (void *)&t1);
            pthread_create(&h2, NULL, tiros, (void *)&t2);
            pthread_create(&h3, NULL, tiros, (void *)&t3);
            pthread_create(&h4, NULL, tiros, (void *)&t4);

            pthread_join(h1, NULL);
            pthread_join(h2, NULL);
            pthread_join(h3, NULL);
            pthread_join(h4, NULL);
            clock_gettime(CLOCK_REALTIME, &end);


            long seconds = end.tv_sec - begin.tv_sec;
            long nanoseconds = end.tv_nsec - begin.tv_nsec;
            double elapsed = seconds + nanoseconds*1e-9;
            tiempos[i]=elapsed;
        }
        circuit++;

        FILE *fp;
        fp=fopen("dartboardParallel_O2.csv","ab");
        for (int i=0; i<tam; i++){
            fprintf(fp, "%.5f,", tiempos[i]);
        }
        fprintf(fp, "%s","\n");
        //fwrite(tiempos,sizeof(int), sizeof(tiempos), fp);
        fclose(fp);
        
    }
    
    return 0;
}
