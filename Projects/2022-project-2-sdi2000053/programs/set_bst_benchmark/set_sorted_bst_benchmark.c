#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ADTSet.h"
#include "ADTVector.h"

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

int main(void) {
    clock_t cl_start, cl_end, cl_t;
    cl_start = clock();

    FILE *fp = fopen("output_set_sorted.csv", "w+");
    Vector vec = vector_create(9000, NULL);
    
    for(int i = 0; i < 9000; i++)
        vector_insert_last(vec, create_int(i));

    Set set = set_create_from_sorted_values(compare_ints, NULL, vec);

    double avrg = set_steps(set)/9000;

    cl_end = clock();
    cl_t = (double)(cl_end - cl_start)/ CLOCKS_PER_SEC;

    printf("total time: %ld\n", cl_t);
    fprintf(fp, "steps = %f\n", set_steps(set));
    fprintf(fp, "<average steps: %f>\n", avrg);

    set_destroy(set);

    fclose(fp);
    
    return 0;
}