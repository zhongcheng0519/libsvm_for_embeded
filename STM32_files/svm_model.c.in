#include "svm_model.h"
#include "math.h"

#define MAX(x, y) ((x)>(y))?(x):(y)
#define MIN(x, y) ((x)<(y))?(x):(y)

typedef double (*KernelHandle)(const float *xi, const float *x, const Model *model);

static int32_t g_label[NR_CLASS] = {
@{label}
};

static float g_coefs[NR_CLASS-1][TOTAL_SV] = { 
@{coefs}
};

static float g_SVs[FEATURE_SIZE*TOTAL_SV] = {
@{SVs}
};

static float g_rhos[NR_CLASSIFER(NR_CLASS)] = {
@{rho}
};

static int32_t g_nr_sv[NR_CLASS] = {
@{nr_sv}
};

static float g_probA[NR_CLASSIFER(NR_CLASS)] = {
@{probA}
};

static float g_probB[NR_CLASSIFER(NR_CLASS)] = {
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
    svm_model->nr_class = NR_CLASS;
    svm_model->total_sv = TOTAL_SV;
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

static int32_t svm_predict_values(const Model *svm_model, const float *x, double* dec_values)
{
    int i = 0;
    int j = 0;
    KernelHandle kernel;
    int nr_class = svm_model->nr_class;
    int l = svm_model->total_sv;
    float kvalue[TOTAL_SV];
    switch (svm_model->kernel_type)
    {
    case KERNEL_LINEAR:
        kernel = linear_kernel;
        break;
    default:
        return -1;
    }

    for (i = 0; i < l; i++)
    {
        float* xi = svm_model->SVs + i * svm_model->vec_dim;
        kvalue[i] = kernel(xi, x, svm_model);
    }

    int start[NR_CLASS];
    start[0] = 0;
    for (i = 1; i < nr_class; i++)
        start[i] = start[i-1] + svm_model->nr_sv[i-1];
    
    int vote[NR_CLASS];
    for (i = 0; i < nr_class; i++)
        vote[i] = 0;

    int p = 0;
    for (i = 0; i < nr_class; i++)
    {
        for (j = i+1; j < nr_class; j++)
        {
            float sum = 0;
            int si = start[i];
            int sj = start[j];
            int ci = svm_model->nr_sv[i];
            int cj = svm_model->nr_sv[j];

            int k;
            float *coef1 = svm_model->coefs[j-1];
            float *coef2 = svm_model->coefs[i];
            for(k = 0; k < ci; k++)
            {
                sum += coef1[si+k] * kvalue[si+k];
            }
            for(k = 0; k < cj; k++)
                sum += coef2[sj+k] * kvalue[sj+k];
            sum -= svm_model->rhos[p];
            dec_values[p] = sum;

            if(dec_values[p] > 0)
                ++vote[i];
            else
                ++vote[j];
            p++;
        }
    }

    int vote_max_idx = 0;
    for(i=1;i<nr_class;i++)
        if(vote[i] > vote[vote_max_idx])
            vote_max_idx = i;

    return svm_model->label[vote_max_idx];
}

// Method 2 from the multiclass_prob paper by Wu, Lin, and Weng
static void multiclass_probability(int k, double (*r)[NR_CLASS], double *p)
{
    int t,j;
    int iter = 0, max_iter=MAX(100,k);
    double Q[k][k];
    double Qp[k];
    double pQp, eps=0.005/k;

    for (t = 0; t < k; t++)
    {
        p[t] = 1.0/k;  // Valid if k = 1
        Q[t][t] = 0;
        for (j = 0; j < t; j++)
        {
            Q[t][t] += r[j][t] * r[j][t];
            Q[t][j] = Q[j][t];
        }
        for (j = t+1; j < k; j++)
        {
            Q[t][t] += r[j][t]*r[j][t];
            Q[t][j] =  -r[j][t]*r[t][j];
        }
    }
    for (iter = 0; iter < max_iter; iter++)
    {
        // stopping condition, recalculate QP,pQP for numerical accuracy
        pQp = 0;
        for (t = 0; t < k; t++)
        {
            Qp[t] = 0;
            for (j = 0; j < k; j++)
                Qp[t] += Q[t][j]*p[j];
            pQp += p[t]*Qp[t];
        }
        double max_error=0;
        for (t = 0; t < k; t++)
        {
            double error = fabs(Qp[t]-pQp);
            if (error > max_error)
                max_error = error;
        }
        if (max_error < eps) break;

        for (t = 0; t < k; t++)
        {
            double diff = (-Qp[t] + pQp) / Q[t][t];
            p[t] += diff;
            pQp = (pQp + diff * (diff*Q[t][t] + 2*Qp[t])) / (1 + diff) / (1 + diff);
            for (j = 0; j < k; j++)
            {
                Qp[j] = (Qp[j] + diff*Q[t][j]) / (1 + diff);
                p[j] /= (1 + diff);
            }
        }
    }
    if (iter >= max_iter)
    {
#ifdef DEBUG_MODE
        printf("Exceeds max_iter in multiclass_prob\n");
#endif
    }
}

int32_t multi_predict_probability(const Model *svm_model, const float* x, int len, double* score)
{
    double dec_values[NR_CLASSIFER(NR_CLASS)];
    int32_t res = svm_predict_values(svm_model, x, dec_values);
    if (res < 0) return res;

    double min_prob=1e-7;
    double pairwise_prob[NR_CLASS][NR_CLASS];
    int i, j;
    int k=0;
    for (i = 0; i < NR_CLASS; i++)
    {
        for (j = i+1; j < NR_CLASS; j++)
        {
            pairwise_prob[i][j]=MIN(MAX(sigmoid_predict(dec_values[k],svm_model->probA[k],svm_model->probB[k]),min_prob),1-min_prob);
            pairwise_prob[j][i]=1-pairwise_prob[i][j];
            k++;
        }
    }
	multiclass_probability(NR_CLASS, pairwise_prob, score);

    int prob_max_idx = 0;
    for (i = 1; i < NR_CLASS; i++)
        if(score[i] > score[prob_max_idx])
            prob_max_idx = i;
    return svm_model->label[prob_max_idx];
}
