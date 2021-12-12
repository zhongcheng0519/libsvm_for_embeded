#include "svm_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    Model svm_model;
    float test_x[FEATURE_SIZE] = {0.007375,0.049970,-1.372099,-0.095147,0.805052,0.529560,0.597682,0.049957,-0.394064,-0.035558};
    float score[NR_CLASS];
    int label;

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

        label = multi_predict_probability(&svm_model, test_x, FEATURE_SIZE, score, NR_CLASS);
        printf("label = %d\n", label);
        printf("scores = [");
        for (i = 0; i < NR_CLASS; i++)
        {
            printf("%f, ", (float)score[i]);
        }
        printf("]\n");
    }

    return 0;
}
