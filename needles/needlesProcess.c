#include <stdlib.h>
#include <stdio.h>
#include <alloca.h>
#include <time.h>
#include <math.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);

void limitar(int div, float distance, float *limites_horizontales){
    for (int i = 0; i < div; i++)
    {
        limites_horizontales[i]=(i*distance);
    }
    
}


void makeNeedles(int n, float sizeNeedles, int limit_max, int *coord_x[], int *coord_y[]){
    int a_x=0;
    int a_y=0;
    int b_x=0;
    int b_y=0;
    double raiz=0.0;
    int r=limit_max+(sizeNeedles-1);
    int l=-(sizeNeedles-1);
    int i=0;
    while (coord_x[n-1][0]==0)
    {
        //printf("%d \n",coord_x[n-1][0]);
        a_x=rand() % (r - l + 1) + l;
        a_y=rand() % (r - l + 1) + l;

        b_x=rand() % (r - l + 1) + l;
        b_y=rand() % (r - l + 1) + l;

        raiz=sqrt( pow(a_x-b_x,2) + pow(a_y-b_y, 2));
        if (raiz ==sizeNeedles){
            if (a_x < b_x){
                coord_x[i][0]=a_x;
                coord_x[i][1]=b_x;
                if (a_y < b_y)
                {
                    coord_y[i][0]=a_y;
                    coord_y[i][1]=b_y;
                    i++;
                }else{
                    coord_y[i][0]=b_y;
                    coord_y[i][1]=a_y;
                    i++;
                } 
            }else{
                coord_x[i][0]=b_x;
                coord_x[i][1]=a_x;
                if (a_y < b_y)
                {
                    coord_y[i][0]=a_y;
                    coord_y[i][1]=b_y;
                    i++;
                }else{
                    coord_y[i][0]=b_y;
                    coord_y[i][1]=a_y;
                    i++;
                } 
            }
        }
    }
    
}

void lim_process(int num_h, int n, int* process_limites){
    int c= n/num_h;
    for (int i = 0; i < num_h; i++)
    {
        process_limites[i]=i*c;
    }
    process_limites[num_h]=n;
}

int calc_inter(float* limites_horizontales, int* coord_x[], int div_table, int start, int end){
    int A=0;
    while (start < end){
        for (int j = 0; j < div_table; j++)
        {
            if ((limites_horizontales[j] >= coord_x[start][0]) && (limites_horizontales[j] < coord_x[start][1])){
                A+=1;
            }
        }
        start++;
    }
    return A;
}


int main(int argc, char const *argv[])
{

    pid_t pid1, pid2,pid3;
    int status1, status2, status3;

    int buf[1];
    int buf1[1];
    int buf2[1];
    int fd[2];
    pipe(fd);
    
    int div_table =10;
    int limit_max=60;
    float div_distance=limit_max/div_table;
    float sizeNeedles=5.0;
    struct timespec begin, end;
    float limites_horizontales[div_table+1];
    int num_h=4;
    int A=0;
    int process_limites[num_h+1];
    limitar(div_table+1, div_distance, limites_horizontales);
    //int n=10000;
    double tiempos[10];
    int n[]={10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    int tam = sizeof(n)/sizeof(int);
    int circuit =0;
    /*printf("digile valor: ");
    scanf("%d", &n);*/

    while(circuit < 10)
    {
        for (int j = 0; j < tam; j++)
        {
            int **coord_x;
            int **coord_y;
            int a=n[j];
            coord_x = (int**)malloc(a*sizeof(int*));
            coord_y = (int**)malloc(a*sizeof(int *));
            for (int i = 0; i < a; i++)
            {
                coord_x[i]=(int *)malloc(2*sizeof(int));
                coord_y[i]=(int *)malloc(2*sizeof(int));
            }
            srand(time(0));
            makeNeedles(a, sizeNeedles, limit_max,coord_x, coord_y);
            lim_process(num_h,a,process_limites);

            clock_gettime(CLOCK_REALTIME, &begin);

            //calc_inter(limites_horizontales, coord_x,div_table, hilos_limites[0], hilos_limites[4]);
            if ( (pid1=fork()) == 0 )
            { /* hijo */
                //printf("Soy el primer hijo (%d, hijo de %d)\n",  getpid(), getppid());
                int A1=calc_inter(limites_horizontales, coord_x,div_table, process_limites[0], process_limites[1]);
                close(fd[0]);
                write(fd[1], &A1,4);
                close(fd[1]);
                exit(0);
            }else
            { /*  padre */
                if ( (pid2=fork()) == 0 )
                { /* segundo hijo  */
                    //printf("Soy el segundo hijo (%d, hijo de %d)\n",  getpid(), getppid());
                    int A2=calc_inter(limites_horizontales, coord_x,div_table, process_limites[1], process_limites[2]);
                    close(fd[0]);
                    write(fd[1], &A2,4);
                    close(fd[1]);
                    exit(0);
                }
                else{ /* padre */
                    if ((pid3=fork()) == 0){
                        //printf("Soy el tercer hijo (%d, hijo de %d)\n",  getpid(), getppid());
                        int A3=calc_inter(limites_horizontales, coord_x,div_table, process_limites[2], process_limites[3]);
                        close(fd[0]);
                        write(fd[1], &A3,4);
                        close(fd[1]);
                        exit(0);
                        
                    }else{
                        //printf("Soy el padre (%d, hijo de %d)\n", getpid(), getppid());
                        int A4=calc_inter(limites_horizontales, coord_x,div_table, process_limites[3], process_limites[4]);
                        
                        close(fd[1]);
                        read(fd[0], buf, 4);
                        read(fd[0], buf1, 4);
                        read(fd[0], buf2, 4);
                        //printf("buf ->%d: \n buf1 ->%d:\nbuf2 ->%d:", *buf,*buf1,*buf2);
                        close(fd[0]);
                        A=A4+*buf+*buf1+*buf2;
                        clock_gettime(CLOCK_REALTIME, &end);
                        long seconds = end.tv_sec - begin.tv_sec;
                        long nanoseconds = end.tv_nsec - begin.tv_nsec;
                        double elapsed = seconds + nanoseconds*1e-9;
                        tiempos[j]=elapsed;
                        //printf("Tiempo de ejecucion : %.5f\n", elapsed);
                        //printf("needles: %d, A: %d, pi: %f\n",n,A,(2*n*sizeNeedles)/(A*div_distance));
                        for (int i = 0; i < a;i++) {
                            free(coord_x[i]);
                            free(coord_y[i]);
                        }
                        free(coord_x);
                        free(coord_y);
                    }
                }
            }

        }
        
        circuit++;
        FILE *fp;
        fp=fopen("needlesProcess_O2.csv","ab");
        for (int i=0; i<tam; i++){
            fprintf(fp, "%.9f,", tiempos[i]);
        }
        fprintf(fp, "%s","\n");
        //fwrite(tiempos,sizeof(int), sizeof(tiempos), fp);
        fclose(fp);
        
    }

    

    return 0;
    
    
    
}

