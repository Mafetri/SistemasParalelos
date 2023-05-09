#include <stdio.h>
#define N 1000
int main (void) {
    int i, x[1000], y[1001], z[1000];

    for (i = N - 2; i >= 0; i--) {
        x[i] = x[i] + y[i+1];
        y[i] = y[i] + z[i];
    }
}