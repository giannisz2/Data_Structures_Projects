#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ADTSet.h"

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

int main(void) {
    FILE *fp = fopen("output_set.csv", "w+");

    clock_t cl_start, cl_end, cl_total;

    cl_start = clock();
    Set set = set_create(compare_ints, NULL);

    int L = 9001;
    int s = 0;
    double avrg = 0;

	int* C = malloc(L * sizeof(*C));					

    
    // insert - remove 9000 στοιχεία
    for (int i = 1; i <= 9000; i++) {
        set_insert(set, &C[i]);
        fprintf(fp, "<steps for %d = %lf>\n", i, set_steps(set));
        s += set_steps(set);    
    }
    
    avrg = s/9000;

    cl_end = clock();
    cl_total = (double)(cl_end - cl_start) / CLOCKS_PER_SEC;
    
    fprintf(fp, "total time: %ld", cl_total);
    fprintf(fp, "<average steps: %f>\n", avrg);

    set_destroy(set);

    fclose(fp);
    
    return 0;
}