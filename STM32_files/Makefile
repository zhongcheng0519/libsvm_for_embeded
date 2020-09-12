CC=gcc
CFLAGS=-I.
DEPS = svm_model.h
OBJ = main.o svm_model.o
TARGET = svm_test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(OBJ) $(TARGET)
