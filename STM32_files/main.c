#include "svm_model.h"
#include <stdio.h>

int main()
{
    Model svm_model;
    float test_x[] = {1.f, 0.f};
    float result = 0.0;

    init_svm_params(&svm_model);
    result = predict(&svm_model, test_x, 2);

    printf("x1 = %f, x2 = %f\n", test_x[0], test_x[1]);
    printf("result = %f\n", result);
    return 0;
}
