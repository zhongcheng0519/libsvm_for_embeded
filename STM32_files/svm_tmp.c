#include "svm_model.h"
#include "math.h"

typedef double (*KernelHandle)(const float *xi, const float *x, const Model *model);

static int32_t g_label[@{nr_class}] = {
@{label}
};

static float g_coefs[@{nr_class}-1][@{total_sv}] = { 
@{coefs}
};

static float g_SVs[FEATURE_SIZE*@{total_sv}] = {
@{SVs}
};

static float g_rhos[NR_CLASSIFER(@{nr_class})] = {
@{rho}
};

static int32_t g_nr_sv[@{nr_class}] = {
@{nr_sv}
};

static float g_probA[NR_CLASSIFER(@{nr_class})] = {
@{probA}
};

static float g_probB[NR_CLASSIFER(@{nr_class})] = {
@{probB}
};

static double sigmoid_predict(double decision_value, double A, double B)
{
	double fApB = decision_value*A+B;
	// 1-p used later; avoid catastrophic cancellation
	if (fApB >= 0)
		return exp(-fApB)/(1.0+exp(-fApB));
	else
		return 1.0/(1+exp(fApB)) ;
}

double linear_kernel(const float *xi, const float *x, const Model *model)
{
    double sum = 0.f;
    int i = 0;
    int len = model->vec_dim;
    for (i = 0; i < len; i++)
    {
        sum += xi[i] * x[i];
    }
    return sum;
}

double polynomial_kernel(const float *xi, const float *x, const Model *model)
{
    double sum = 0.f;
    int i = 0;
    int len = model->vec_dim;
    double res = 0;

    for (i = 0; i < len; i++)
    {
        sum += xi[i] * x[i];
    }
    
    res = (model->gamma * sum + model->coef0);
    return res * res;
}

double rbf_kernel(const float *xi, const float *x, const Model *model)
{
    double sum = 0.f;
    int i = 0;
    int len = model->vec_dim;
    double res = 0.;
    double temp = 0.;
    for (i = 0; i < len; i++)
    {
        temp = xi[i] - x[i];
        sum += temp*temp;
    }
    
    res = exp(-model->gamma*sum);
    return res;
}

void init_svm_params(Model *svm_model)
{
    svm_model->kernel_type = @{kernel_type};
    svm_model->degree   = @{degree};
    svm_model->gamma    = @{gamma};
    svm_model->coef0    = @{coef0};
    svm_model->nr_class = @{nr_class};
    svm_model->total_sv = @{total_sv};
    svm_model->rhos     = g_rhos;
    svm_model->label    = g_label;
    svm_model->probA    = g_probA;
    svm_model->probB    = g_probB;

    // support vectors
    svm_model->coefs    = g_coefs;
    svm_model->vec_dim  = @{vec_dim};
    svm_model->SVs      = g_SVs;
}


double predict(const Model *svm_model, const float* x, int len)
{
    int i = 0;
    double sum = 0.f;
    KernelHandle kernel;
    if (len != svm_model->vec_dim)
    {
        // len == feature num == vector dim
        return 0;
    }
    switch (svm_model->kernel_type)
    {
    case KERNEL_LINEAR:
        kernel = linear_kernel;
        break;
    case KERNEL_POLYNOMIAL:
        kernel = polynomial_kernel;
        break;
    case KERNEL_RBF:
        kernel = rbf_kernel;
        break;
    default:
        return 0;
    }

    for (i = 0; i < svm_model->total_sv; i++)
    {
        float* xi = svm_model->SVs + i * svm_model->vec_dim;
        sum += svm_model->coefs[0][i] * kernel(xi, x, svm_model);
    }

    return sum-svm_model->rhos[0];
}

double predict_probability(const Model *svm_model, const float* x, int len)
{
    double score = predict(svm_model, x, len);
    return sigmoid_predict(score, svm_model->probA[0], svm_model->probB[0]);
}
