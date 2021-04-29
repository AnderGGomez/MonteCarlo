#include <stdlib.h>
#include <stdio.h>
#include <alloca.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

struct data
{
    float* limites_horizontales;
    int **coord_x;
    int div_table;
    int start;
    int end;
    int A;
};


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

void lim_thread(int num_t, int n, int* hilos_limites, int tam){
    int c= n/num_t;
    for (int i = 0; i < num_t; i++)
    {
        hilos_limites[i]=i*c;
    }
    hilos_limites[tam]=n;
}

void *calc_inter(void * arg){

    struct data *p;

    p= (struct data *)arg;
    while (p->start < p->end){
        for (int j = 0; j < p->div_table; j++)
        {
            if ((p->limites_horizontales[j] >= p->coord_x[p->start][0]) && (p->limites_horizontales[j] < p->coord_x[p->start][1])){
                p->A+=1;
            }
        }
        p->start++;
    }
}



int main(int argc, char const *argv[])
{
    struct data t1;
    struct data t2;
    struct data t3;
    struct data t4;


    int div_table =10;
    int limit_max=60;
    float div_distance=limit_max/div_table;
    float sizeNeedles=5.0;
    struct timespec begin, end;
    int n[]={10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    int tam = sizeof(n)/sizeof(int);
    int circuit=0;
    double tiempos[10];
    int **coord_x;
    int **coord_y;
    float limites_horizontales[div_table+1];
    int num_t=4;
    int hilos_limites[num_t+1];
    int Acum;

    while (circuit < 10)
    {
        for (int j = 0; j < tam; j++)
        {
            limitar(div_table+1, div_distance, limites_horizontales);
            Acum=0;
            int a=n[j];
            coord_x = (int**)malloc(a*sizeof(int*));
            coord_y = (int**)malloc(a*sizeof(int*));
            for (int i = 0; i < a; i++)
            {
                coord_x[i]=(int *)malloc(2*sizeof(int));
                coord_y[i]=(int *)malloc(2*sizeof(int));
            }
            srand(time(0));
            makeNeedles(a, sizeNeedles, limit_max,coord_x, coord_y);
            
            lim_thread(num_t,a,hilos_limites, (sizeof(hilos_limites)/(sizeof(int)))-1);

            int g1=hilos_limites[0];
            int g2=hilos_limites[1];

            t1.limites_horizontales=limites_horizontales;
            t1.coord_x=coord_x;
            t1.div_table=div_table+1;
            t1.start=g1;
            t1.end=g2;
            t1.A=0;

            g1=hilos_limites[1];
            g2=hilos_limites[2];

            t2.limites_horizontales=limites_horizontales;
            t2.coord_x=coord_x;
            t2.div_table=div_table+1;
            t2.start=g1;
            t2.end=g2;
            t2.A=0;

            g1=hilos_limites[2];
            g2=hilos_limites[3];

            t3.limites_horizontales=limites_horizontales;
            t3.coord_x=coord_x;
            t3.div_table=div_table+1;
            t3.start=g1;
            t3.end=g2;
            t3.A=0;

            g1=hilos_limites[3];
            g2=hilos_limites[4];

            t4.limites_horizontales=limites_horizontales;
            t4.coord_x=coord_x;
            t4.div_table=div_table+1;
            t4.start=g1;
            t4.end=g2;
            t4.A=0;


            pthread_t h1;
            pthread_t h2;
            pthread_t h3;
            pthread_t h4;

            
            clock_gettime(CLOCK_REALTIME, &begin);
            //calc_inter(limites_horizontales, coord_x,div_table, hilos_limites[0], hilos_limites[4], &A);
            pthread_create(&h1, NULL, calc_inter, (void *)&t1);
            pthread_create(&h2, NULL, calc_inter, (void *)&t2);
            pthread_create(&h3, NULL, calc_inter, (void *)&t3);
            pthread_create(&h4, NULL, calc_inter, (void *)&t4);

            pthread_join(h1, NULL);
            pthread_join(h2, NULL);
            pthread_join(h3, NULL);
            pthread_join(h4, NULL);
            
            clock_gettime(CLOCK_REALTIME, &end);
            long seconds = end.tv_sec - begin.tv_sec;
            long nanoseconds = end.tv_nsec - begin.tv_nsec;
            double elapsed = seconds + nanoseconds*1e-9;

            tiempos[j]=elapsed;
            //Acum=t1.A+t2.A+t3.A+t4.A;
            //printf("Tiempo de ejecucion : %.5f\n", elapsed);
            //printf("needles: %d, A: %d, pi: %f\n",a,Acum,(2*a*sizeNeedles)/(Acum*div_distance));
            
            for (int i = 0; i < a;i++) {
                free(coord_x[i]);
                free(coord_y[i]);
            }
            free(coord_x);
            free(coord_y);
        }
        circuit++;
        FILE *fp;
        fp=fopen("needlesParallel_O2.csv","ab");
        for (int i=0; i<tam; i++){
            fprintf(fp, "%.9f,", tiempos[i]);
        }
        fprintf(fp, "%s","\n");
        //fwrite(tiempos,sizeof(int), sizeof(tiempos), fp);
        fclose(fp);
    }
    return 0;
}
