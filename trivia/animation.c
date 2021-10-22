#include <stdio.h>
#include <time.h>
#include "animation.h"

void loading(void){ //load stops when start == stop
    int msec = 0;
    const int trigger = 500; // ms
    const int printWidth = 4;
    int counter = 0;
    clock_t before = clock();

    int wait = 0;
    while (wait < 60)
    {
        fputs("Training", stdout);
        for(int tmp=0; tmp < 60;tmp++){
            fputs("|",stdout);
        }
        clock_t difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        if (msec >= trigger)
        {
            counter++;
            msec = 0;
            before = clock();
        }
        ++wait;
        for (int i = 0; i < counter; ++i)
        {            
            fputc('.', stdout);
        }
        for (int i = 0; i < printWidth - counter; ++i)
        {
            fputc(' ', stdout);
        }
        fputc('\r', stdout);
        fflush(stdout);

        if (counter == printWidth)
        {
            counter = 0;
        }
    }
}
/*
int main(void)
{   
    loading();
    return 0;
}*/