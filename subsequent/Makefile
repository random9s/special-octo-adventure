CC=gcc
CFLAGS=-I.
_DEPS=deps/
DEPS = $(_DEPS)dyn_arr.h
_LIBS=libs/
OBJ = $(_LIBS)dyn_arr.o main.o 
SHARED = $(_LIBS)dyn_arr.so main.so
OUT=search

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

%.so: %.o $(OBJ)
	$(CC) -shared -o $@ $^ $(CFLAGS)

$(OUT): $(SHARED)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(_LIBS)*.so $(_LIBS)*.o *.so $(OUT)
