#include "svm_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    Model svm_model;
    float test_x[] = {1.f, 0.f};
    float result = 0.0;

    char buffer[256];
    
    init_svm_params(&svm_model);
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        int res;
        res = sscanf(buffer, "%f %f", &test_x[0], &test_x[1]);
        if (res > 0)
        {
            printf("x1 = %f, x2 = %f\n", test_x[0], test_x[1]);
            result = predict(&svm_model, test_x, 2);
            printf("result = %f\n", result);
        }
    }

    return 0;
}
