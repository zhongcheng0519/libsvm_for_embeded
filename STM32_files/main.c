#include "svm_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    Model svm_model;
    float test_x[FEATURE_SIZE];
    double result = 0.0;

    char buffer[256];
    int i;
    
    init_svm_params(&svm_model);
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        char* psub;
        if (strlen(buffer) < 3)
            break;
        test_x[0] = atof(strtok(buffer, " "));
        for (i = 1; i < FEATURE_SIZE; i++)
        {
            test_x[i] = atof(strtok(NULL, " "));
        }

        printf("(");
        for (i = 0; i < FEATURE_SIZE; i++)
        {
            printf("%f,", test_x[i]);
        }
        printf(")\n");
        result = predict(&svm_model, test_x, FEATURE_SIZE);
        printf("result = %f\n", result);
    }

    return 0;
}
