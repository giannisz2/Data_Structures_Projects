#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADTVector.h"

int main(int argc, char* argv[]) {

    if(argc < 2) {
        printf("Error, choose \"real\" or \"amortized\". \n");
        return -1;
    }

    FILE *fp = fopen("output_vector.csv", "w+");
    Vector vec = vector_create(10, NULL);

    int M = 9000;
    int k = 0;
    double avg;

	int* B = malloc(M * sizeof(*B));					

    
    // insert - remove 9000 στοιχεία
    for (int i = 1; i <= 9000; i++) {
        if(i % 3 != 0 && i > 3) {
            vector_insert_last(vec, &B[i]);

            if(strcmp("real", argv[1]) == 0)
                fprintf(fp, "<steps for %d = %d>\n", i, vector_steps(vec));

            k += vector_steps(vec);
        }else {
            vector_remove_last(vec);

            if(strcmp("real", argv[1]) == 0)
                fprintf(fp, "<steps for %d = %d>\n", i, vector_steps(vec));

            k += vector_steps(vec);
        }
                    
	}
    

    avg = k/9000;

    if(strcmp("amortized", argv[1]) == 0) 
        fprintf(fp, "<average steps: %f\n>", avg);


    vector_destroy(vec);
    fclose(fp);

    return 0;
}