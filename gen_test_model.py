#!/usr/bin/env python
import matplotlib.pyplot as plt
import numpy as np
import libsvm.svmutil as svm

total_samples = 4


def decision_func(x1, x2):
    return np.sign(x2-x1)

def svm_data_format(x1, x2):
    return [{1: x1[i], 2:x2[i]} for i in range(len(x1))]

if __name__ == "__main__":
    print('total samples: %d' % total_samples)
    x1 = np.random.rand(total_samples)
    x2 = np.random.rand(total_samples)
    label = decision_func(x1, x2)
    pos_loc = np.where(label > 0)
    neg_loc = np.where(label < 0)
    pos_x1 = x1[pos_loc]
    pos_x2 = x2[pos_loc]
    neg_x1 = x1[neg_loc]
    neg_x2 = x2[neg_loc]

    plt.plot(pos_x1, pos_x2, 'b^')
    plt.plot(neg_x1, neg_x2, 'ro')
    plt.xlim(0, 1)
    plt.ylim(0, 1)
    ax = plt.gca()
    ax.set_aspect(1)
    plt.show()

    y = list(label)
    x = svm_data_format(x1, x2)
    prob = svm.svm_problem(y, x)
    param = svm.svm_parameter('-t 0 -c 1')
    model = svm.svm_train(prob, param)
    svm.svm_save_model('test.model', model)

    # test
    y0 = [-1]
    x0 = [{1: 1, 2: 0}]
    p_label, p_acc, p_val = svm.svm_predict(y0, x0, model)
    print('p_label = ', p_label)
    print('p_acc = ', p_acc)
    print('p_val = ', p_val)

