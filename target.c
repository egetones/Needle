#include <stdio.h>
#include <unistd.h>

void main() {
    int i = 0;
    printf("Target Process PID: %d\n", getpid());
    printf("Ben masum bir programım, sadece sayıyorum...\n");
    
    while(1) {
        printf("Sayıyorum: %d\n", i++);
        sleep(2);
    }
}
