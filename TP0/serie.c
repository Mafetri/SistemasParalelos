#define _GNU_SOURCE // sched_getcpu(3) is glibc-specific
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

int main( int argc, char *argv[]) {
   char hostname[128];
   gethostname(hostname, sizeof(hostname));

   printf("Ejecutando en nodo %s, core %d\n", hostname, sched_getcpu());

   //while(1){};

   return 0;
}
