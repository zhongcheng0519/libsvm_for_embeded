#!/usr/bin/env python
import libsvm.svmutil as svm
import sys


if __name__ == "__main__":
    model_path = "test.model"
    if len(sys.argv) == 2:
        model_path = sys.argv[1]

    model = svm.svm_load_model(model_path)
    for line in sys.stdin:
        line = line.strip()
        x = line.split(' ')
        if len(x) != 2:
            continue
        vx = [{1: float(x[0]), 2: float(x[1])}]
        print('vx = ', vx)
        p_label, p_acc, p_val = svm.svm_predict([], vx, model, '-q')
        print('p_val = ', p_val)
