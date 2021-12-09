#ifndef __LIBSVM_MODEL_H__
#define __LIBSVM_MODEL_H__

#include <stdint.h>
#define FEATURE_SIZE @{vec_dim}
#define NR_CLASSIFER(nc) (nc*(nc-1)/2)

typedef enum {
    KERNEL_UNKNOWN = 0,
    KERNEL_LINEAR, 
    KERNEL_POLYNOMIAL,
    KERNEL_RBF
} KernelType;

typedef struct {
    KernelType kernel_type;
    //-- for polynomial kernel -->
    uint8_t degree;
    float gamma;
    float coef0;
    //<---------------------------
    uint8_t nr_class;    /* number of classes, = 2 in regression/one class svm */
    uint32_t total_sv;   /* total #SV */
    float* rhos;          /* constants in decision functions (rho[k*(k-1)/2]) */
    int32_t* label;      /* label of each class (label[k]) */
    float* probA;        /* pariwise probability information */
    float* probB;

    float** coefs;       /* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
    uint32_t vec_dim;
    float* SVs;          /* SVs (SV[l]) */
    int32_t* nr_sv;      /* number of SVs for each class (nSV[k]) */
                         /* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
} Model;

void init_svm_params(Model *svm_model);

double predict(const Model *svm_model, const float* x, int len);

double predict_probability(const Model *svm_model, const float* x, int len);


#endif
