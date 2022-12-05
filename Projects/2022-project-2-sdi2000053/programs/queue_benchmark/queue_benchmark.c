#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADTQueue.h"

int main(int argc, char* argv[]) {

    if(argc < 2) {
        printf("Error, choose \"real\" or \"amortized\". \n");
        return -1;
    }

    
    FILE *fp = fopen("output_queue.csv", "w+");
    Queue queue = queue_create(NULL);

    int N = 9000;
    int p = 0;
    double av;

	int* A = malloc(N * sizeof(*A));					

   
    // insert - remove 9000 στοιχεία
    for (int i = 1; i <= 9000; i++) {
        if(i % 3 != 0) {
            queue_insert_back(queue, &A[i]);

            if(strcmp("real", argv[1]) == 0) 
                fprintf(fp, "<steps for %d = %d>\n", i, queue_steps(queue));

            p += queue_steps(queue);
        }else {
            queue_remove_front(queue);

            if(strcmp("real", argv[1]) == 0)
                fprintf(fp, "<steps for %d = %d>\n", i, queue_steps(queue));

            p += queue_steps(queue);
        }
                
    
    } 
    av = p/9000;

    if(strcmp("amortized", argv[1]) == 0)
        fprintf(fp, "<average steps: %f> \n", av);
    
        
    queue_destroy(queue);
    fclose(fp);

    return 0;
}