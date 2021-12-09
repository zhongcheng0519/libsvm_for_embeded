CC=gcc
CFLAGS=-I. -g
DEPS = svm_model.h
PREDICT_OBJ = main.o svm_model.o
MULTI_OBJ = main_multi.o svm_model.o

LIBS = -lm
.PHONY: clean svm_test multi_test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

svm_test: $(PREDICT_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

multi_test: $(MULTI_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf $(PREDICT_OBJ) $(MULTI_OBJ) svm_test multi_test
