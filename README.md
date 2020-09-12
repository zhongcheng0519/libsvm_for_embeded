# libsvm for embeded

[libsvm](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) is the famous SVM library, which provides interfaces to almost all the programming languages. However, it seems friendly only for the environments with file systems. Currently, I want to apply libsvm result, which is named as `*.model` to STM32, and I don't want to port FATFS to it. So the easy way is convert `*.model` file to `svm_model.h` and `svm_model.c` which can be compiled to STM32.

