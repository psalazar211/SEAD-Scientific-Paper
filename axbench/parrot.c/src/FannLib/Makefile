FANN_TYPE=FIXEDFANN
FANN_TYPE=DOUBLEFANN
FANN_TYPE=FLOATFANN

CC=gcc
CFLAGS=-O3 -MD

INCLUDES=#-I../ParrotLib -I../Npu 

LFLAGS=# -L/home/newhall/lib  -L../lib

LIBS=# -lmylib -lm

SRCS=$(wildcard *.c)

# $(name:string1=string2)
# For each word in 'name' replace 'string1' with 'string2'
OBJS=$(SRCS:.c=.o)

OUT_DIR=.
OUT=libFann.a

.PHONY: depend clean

all: $(OUT)

#Note: the library must start with the three letter lib.
$(OUT): $(OBJS)
	ar rcs $(OUT_DIR)/$(OUT) *.o
	ar -t $(OUT_DIR)/$(OUT)

# This is a suffix replacement rule for building .o's from .c's.
# It uses 'automatic variables' $<: the name of the prerequisite of
# the rule (a .c file) and $@: the name of the target of the rule
# (a .o file) 
%.o: %.c
	$(CC) $(CFLAGS) -D$(FANN_TYPE) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *.d

dist: clean
	$(RM) $(OUT_DIR)/*.a

