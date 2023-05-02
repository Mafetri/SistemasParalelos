#include <stdio.h>
#include <unistd.h>

void trabajo(int id){
    printf("%d: Trabajo muy duro...\n", id);
    sleep(1);
    printf("%d: como un esclavo!\n", id);
    sleep(1);
}

int main(){
    int i;

    #pragma omp parallel for num_threads(8)  
    for(i = 0; i < 8; i++){
        trabajo(i);
    }

    return 0;
}
