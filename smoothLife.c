#include <math.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int HEIGHT = 99;
int WIDTH = 94;
char level[] = {' ', '.', '-', '=', 'c', 'o', 'a', 'A', '@', '#'};
int level_count = sizeof(level) / sizeof(level[0]);

void emptyGrid(float* grid);
void display(float* grid);
void updateGrid(float* grid);
float sigma1(float x, float a);
float sigma2(float x, float a, float b);
float sigmam (float x, float y, float m);
float s(float n, float m);
void clamp(float *x, float l, float h);



float ra = 11;
float alpha_n = 0.028;
float alpha_m = 0.147;
float b1 = 0.278;
float b2 = 0.365;
float d1 = 0.267;
float d2 = 0.445;
float dt = 0.1;
float perc = 0.4;


int main(void) {
    srand(time(NULL));
    float* grid = malloc(WIDTH * HEIGHT * sizeof(float));
    emptyGrid(grid);
    for(;;) {
        system("clear");
        display(grid);
        updateGrid(grid);
    }
    free(grid);
}

void display(float* grid) {
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            int index = i * HEIGHT + j;
            float f = *(grid + index);
            char c = (int)(f * level_count);
            if (c == level_count) {
                c--;
            }
            printf("%c ", level[c]);
            printf("%c ", level[c]);
        }
        printf("\n");
    }
}

void emptyGrid(float* grid) {
    int w = WIDTH * perc;
    int h = HEIGHT * perc;
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            int index = (i - h/2 + HEIGHT/2) * HEIGHT + (j - w/2 + WIDTH/2);
            float* c = (grid + index);
            float r = (float)rand() / (float)RAND_MAX;
            *c = r;
        }
}
}

void updateGrid(float* grid) {
    float ri = ra/3;
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            float n = 0.0;
            float m = 0.0;
            int M = 0;
            int N = 0;
            for(int dy = -ra; dy <= ra; dy++) {
                for(int dx = -ra; dx <= ra; dx++) {
                    if (dy*dy + dx*dx <= ra*ra) {
                        int y = i + dy;
                        int x = j + dx;
                        x = (x%WIDTH + WIDTH)% WIDTH;
                        y = (y%HEIGHT + HEIGHT)% HEIGHT;
                        if (dy*dy + dx*dx <= ri*ri) {
                            m += *(grid + y * HEIGHT + x);
                            M += 1;
                        }
                        else {
                            n += *(grid + y * HEIGHT + x);
                            N += 1;
                        }
                    }
                }
            }
            n /= N;
            m /= M;
            *(grid + (i * HEIGHT + j)) += dt*(2*s(n, m)-1);
            clamp((grid + (i * HEIGHT + j)), 0, 1);
        }
    }
}

void clamp(float *x, float l, float h)
{
    if (*x < l) *x = l;
    if (*x > h) *x = h;
}

float sigma(float x, float a, float alpha)
{
    return 1.0f/(1.0f + expf(-(x - a)*4/alpha));
}

float sigma_n(float x, float a, float b)
{
    return sigma(x, a, alpha_n)*(1 - sigma(x, b, alpha_n));
}

float sigma_m(float x, float y, float m)
{
    return x*(1 - sigma(m, 0.5f, alpha_m)) + y*sigma(m, 0.5f, alpha_m);
}

float s(float n, float m)
{
    return sigma_n(n, sigma_m(b1, d1, m), sigma_m(b2, d2, m));
}

