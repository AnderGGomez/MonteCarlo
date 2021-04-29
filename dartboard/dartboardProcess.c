#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
//Con la optimizacion de Gcc 02 se ignoran las Pipes.
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
/*
struct data
{
    int start;
    int end;
    double factor;
    long A;
};*/



void limitar (int* limite_hilos,int num_t, int n, int tam){
    int c=n/num_t;
    for (int i = 0; i < num_t; i++)
    {
        limite_hilos[i]=i*c;
    }
    limite_hilos[tam]=n;
    
}

//void *tiros (void * arg){
long tiros (int start, int end, double factor){
    //struct data *p;
    //p=(struct data *)arg;
    srand((int)clock());
    long hits=0;
    double x;
    double y;
    
    while(start < end){
        x = rand()*factor;
        y = rand()*factor;
        if(x*x + y*y < 1.0){
            ++hits;
        }
        start++;
    }
    return hits;
}

int main(void)
{
    pid_t pid1, pid2,pid3;
    int status1, status2, status3;
    
    int buf[1];
    int buf1[1];
    int buf2[1];
    int fd[2];
    pipe(fd);

    int n[]={100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 10000000};
    double tiempos[10];
    long k, hits=0;
    const double factor =1.0/RAND_MAX;
    int tam = sizeof(n)/sizeof(int);
    //int tam =10;
    int circuit =0;
    int num_t=4;
    int limite_hilos[num_t+1];
    struct timespec begin, end;

    while (circuit < 20)
    {
        for (int i = 0; i < tam; i++)
        {
            limitar(limite_hilos, num_t, n[i], num_t);
            clock_gettime(CLOCK_REALTIME, &begin);
            if ( (pid1=fork()) == 0 )
            { /* hijo */
                //printf("Soy el primer hijo (%d, hijo de %d)\n",  getpid(), getppid());
                long A1=tiros (limite_hilos[0], limite_hilos[1], factor);
                //calc_inter(limites_horizontales, coord_x,div_table, process_limites[0], process_limites[1]);
                close(fd[0]);
                write(fd[1], &A1,8);
                close(fd[1]);
                exit(0);
            }
            else
            { /*  padre */
                if ( (pid2=fork()) == 0 )
                { /* segundo hijo  */
                    //printf("Soy el segundo hijo (%d, hijo de %d)\n",  getpid(), getppid());
                    long A2=tiros (limite_hilos[1], limite_hilos[2], factor);
                    close(fd[0]);
                    write(fd[1], &A2,8);
                    close(fd[1]);
                    exit(0);
                }
                else{ /* padre */
                    if ((pid3=fork()) == 0){
                        //printf("Soy el tercer hijo (%d, hijo de %d)\n",  getpid(), getppid());
                        long A3=tiros (limite_hilos[2], limite_hilos[3], factor);
                        close(fd[0]);
                        write(fd[1], &A3,8);
                        close(fd[1]);
                        exit(0);
                        
                    }else{
                        //printf("Soy el padre (%d, hijo de %d)\n", getpid(), getppid());
                        int A4=tiros (limite_hilos[3], limite_hilos[4], factor);
                        close(fd[1]);
                        read(fd[0], buf, 8);
                        read(fd[0], buf1, 8);
                        read(fd[0], buf2, 8);
                        close(fd[0]);
                        
                        hits=A4+*buf+*buf1+*buf2;

                        clock_gettime(CLOCK_REALTIME, &end);
                        
                        long seconds = end.tv_sec - begin.tv_sec;
                        long nanoseconds = end.tv_nsec - begin.tv_nsec;
                        double elapsed = seconds + nanoseconds*1e-9;
                        tiempos[i]=elapsed;
                        /*printf("Tiempo de ejecucion : %.5f\n", elapsed);
                        //printf("needles: %d, A: %d, pi: %f\n",n,A,(2*n*sizeNeedles)/(A*div_distance));
                        double pi_aprox=4.0*hits /n[i];
                        printf("pi: %f\n",pi_aprox);*/
                    }
                }
            }
        }
        
        circuit++;

        FILE *fp;
        fp=fopen("dartboardProcess_O2.csv","ab");
        for (int i=0; i<tam; i++){
            fprintf(fp, "%.5f,", tiempos[i]);
        }
        fprintf(fp, "%s","\n");
        //fwrite(tiempos,sizeof(int), sizeof(tiempos), fp);
        fclose(fp);
    }
    
    return 0;
}
