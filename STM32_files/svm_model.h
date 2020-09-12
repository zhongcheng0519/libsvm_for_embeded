#ifndef __LIBSVM_MODEL_H__
#define __LIBSVM_MODEL_H__

#include <stdint.h>

typedef enum {
    KERNEL_LINEAR = 0,
    KERNEL_POLYNOMIAL
} KernelType;

typedef struct {
    KernelType kernel_type;
    //-- for polynomial kernel -->
    uint8_t degree;
    float gamma;
    float coef0;
    //<---------------------------
    uint8_t nr_class;
    uint32_t total_sv;
    float rho;
    int32_t* label;
    float probA;
    float probB;

    float* coefs;
    uint32_t vec_dim;
    float* SVs;
   
} Model;

void init_svm_params(Model *svm_model);

float predict(const Model *svm_model, const float* x, int len);

#endif
