#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 4
#define MIU 0.0002
#define RHO 0.5
#define G 0.75

int simulation(float *grid, float *grid_1, float *grid_2)
{
    // make a copy of the original input grid

    for(int y=0; y<GRID_SIZE; y++){
        for(int x=0; x<GRID_SIZE; x++){
            if(y==0&&x!=0&&x!=GRID_SIZE-1)
            {
                //boundary condition 1: y = 0
                float u1_x_1 = *(grid_1+GRID_SIZE*(y+1)+x);
                *(grid+GRID_SIZE*y+x) = G * u1_x_1;
            } 
            else if (y==GRID_SIZE-1&&x!=0&&x!=GRID_SIZE-1)
            {
                //boundary condition 2: y = GRID_SIZE-1
                float u1_x_y1 = *(grid_1+GRID_SIZE*(y-1)+x);
                *(grid+GRID_SIZE*y+x) = G * u1_x_y1;
            }
            else if(x==0&&y!=0&&y!=GRID_SIZE-1)
            {
                //boundary condition 3: x = 0
                float u1_1_y = *(grid_1+GRID_SIZE*y+(x+1));
                *(grid+GRID_SIZE*y+x) = G * u1_1_y;
            } 
            else if (x==GRID_SIZE-1&&y!=0&&y!=GRID_SIZE-1)
            {
                //boundary condition 4: x = GRID_SIZE-1
                float u1_x1_y = *(grid_1+GRID_SIZE*y+(x-1));
                *(grid+GRID_SIZE*y+x) = G * u1_x1_y;
            }
            else if (x==0&&y==0) 
            {
                // corner condition 1: x = y = 0
                float u1_1_0 = *(grid_1+GRID_SIZE*y+(x+1));
                *(grid+GRID_SIZE*y+x) = G * u1_1_0;
            }
            else if (x==0&&y==GRID_SIZE-1) 
            {
                // corner condition 2: x = 0, y = GRID_SIZE - 1
                float u1_0_y1 = *(grid_1+GRID_SIZE*(y-1)+x);
                *(grid+GRID_SIZE*y+x) = G * u1_0_y1;
            }
            else if (x==GRID_SIZE-1&&y==0) 
            {
                // corner condition 3: x = GRID_SIZE - 1, y = 0
                float u1_x1_0 = *(grid_1+GRID_SIZE*y+(x-1));
                *(grid+GRID_SIZE*y+x) = G * u1_x1_0;
            }
            else if (x==GRID_SIZE-1&&y==GRID_SIZE-1) 
            {
                // corner condition 4: x = y = GRID_SIZE - 1
                float u1_x_y1 = *(grid_1+GRID_SIZE*(y-1)+x);
                *(grid+GRID_SIZE*y+x) = G * u1_x_y1;
            }
            else
            {
                float u1_x_y = *(grid_1+GRID_SIZE*y+x);
                float u2_x_y = *(grid_2+GRID_SIZE*y+x);

                // center case: safe to compute
                float u1_1x_y = *(grid+GRID_SIZE*y+(x+1));
                float u1_x1_y = *(grid+GRID_SIZE*y+(x-1));

                float u1_x_1y = *(grid+GRID_SIZE*(y+1)+x);
                float u1_x_y1 = *(grid+GRID_SIZE*(y-1)+x);
                
                *(grid+GRID_SIZE*y+x) = (RHO*(u1_x1_y + u1_1x_y + u1_x_y1 + u1_x_1y - 4*u1_x_y) + 2*u1_x_y - (1-MIU)*u2_x_y )/(1+MIU);
            }
        }
    }

    memcpy(grid_2, grid_1, GRID_SIZE*GRID_SIZE*sizeof(float));
    memcpy(grid_1, grid, GRID_SIZE*GRID_SIZE*sizeof(float));

    return 0;

}

int print_grid(float *grid)
{
    printf("Size of gird: %d nodes\n", GRID_SIZE*GRID_SIZE);

    for(int y=0; y<GRID_SIZE; y++){
        for(int x=0; x<GRID_SIZE; x++){
            printf("(%d,%d): %f ", y, x, *(grid+GRID_SIZE*y+x));
        }
        printf("\n");
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        printf("RuntimeError: Wrong inputs\n");
        printf("Correct Format: ./grid_%d_%d_sequential <number of iterations>\n", GRID_SIZE, GRID_SIZE);
        return 1;
    }

    int iter = atoi(argv[1]);

    float *grid = (float *)calloc(GRID_SIZE*GRID_SIZE, sizeof(float));
    float *grid_1 = (float *)calloc(GRID_SIZE*GRID_SIZE, sizeof(float));
    float *grid_2 = (float *)calloc(GRID_SIZE*GRID_SIZE, sizeof(float));

    *(grid+GRID_SIZE*(GRID_SIZE/2-1)+(GRID_SIZE/2-1)) = 1.0f;

    for(int i = 0; i<iter; i++){
        simulation(grid, grid_1, grid_2);
        print_grid(grid);
    }

    grid=NULL;
    grid_1=NULL;
    grid_2=NULL;
    free(grid);
    free(grid_1);
    free(grid_2);

}