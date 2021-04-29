#include <stdlib.h>
#include <stdio.h>
#include <alloca.h>
#include <time.h>
#include <math.h>


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
    int contador=0;
    while (contador<n)
    {
        
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
                    contador++;
                    i++;
                }else{
                    coord_y[i][0]=b_y;
                    coord_y[i][1]=a_y;
                    contador++;
                    i++;
                } 
            }else{
            
                coord_x[i][0]=b_x;
                coord_x[i][1]=a_x;
                if (a_y < b_y)
                {
                    coord_y[i][0]=a_y;
                    coord_y[i][1]=b_y;
                    contador++;
                    i++;
                }else{
                    coord_y[i][0]=b_y;
                    coord_y[i][1]=a_y;
                    contador++;
                    i++;
                } 
            }
        }
    }
    
}


int main(int argc, char const *argv[])
{
    int div_table =10;
    int limit_max=60;
    float div_distance=limit_max/div_table;
    float sizeNeedles=5.0;
    struct timespec begin, end;

    int n[]={10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    //int n[]={10};
    int tam = sizeof(n)/sizeof(int);
    //int tam =10;
    //int n[]={100};
    int circuit =0;

    double tiempos[10];
    float limites_horizontales[div_table+1];
    int **coord_x;
    int **coord_y;
    int A;

    while (circuit < 10)
    {
        for (int j = 0; j < tam; j++)
        {
            int a=n[j];
            coord_x = (int**)malloc(a*sizeof(int*));
            coord_y = (int**)malloc(a*sizeof(int*));
            for (int i = 0; i < a; i++)
            {
                coord_x[i]=(int *)malloc(2*sizeof(int));
                coord_y[i]=(int *)malloc(2*sizeof(int));
            }
            
            limitar(div_table+1, div_distance, limites_horizontales);
            srand(time(0));
            makeNeedles(a, sizeNeedles, limit_max,coord_x, coord_y);
            clock_gettime(CLOCK_REALTIME, &begin);
            A=0;
            
            for (int i = 0; i < a; i++)
            {
                for (int j = 0; j < div_table+1; j++)
                {
                    if ((limites_horizontales[j] >= coord_x[i][0]) && (limites_horizontales[j] < coord_x[i][1])){
                        A+=1;
                    }
                } 
            }
            clock_gettime(CLOCK_REALTIME, &end);
            //printf("tam: %d -> j: %d\n", tam, j);
            long seconds = end.tv_sec - begin.tv_sec;
            long nanoseconds = end.tv_nsec - begin.tv_nsec;
            double elapsed = seconds + nanoseconds*1e-9;

            tiempos[j]=elapsed;
            /*printf("Tiempo de ejecucion : %.3f\n", elapsed);
            printf("needles: %d, A: %d, pi: %f\n",a,A,(2*a*sizeNeedles)/(A*div_distance));*/

            for (int i = 0; i < a;i++) {
                free(coord_x[i]);
                free(coord_y[i]);
            }
            free(coord_x);
            free(coord_y);
            
        }
        circuit++;
        FILE *fp;
        fp=fopen("needlesSec_O2.csv","ab");
        for (int i=0; i<tam; i++){
            fprintf(fp, "%.9f,", tiempos[i]);
        }
        fprintf(fp, "%s","\n");
        //fwrite(tiempos,sizeof(int), sizeof(tiempos), fp);
        fclose(fp);
    }
    
    


    
    
    

    





    
    
    return 0;
}
