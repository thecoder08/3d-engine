.PHONY: run clean

run: demo
	LD_LIBRARY_PATH=. ./$^ examples/torus.obj

demo: demo.o lib3dengine.so
	gcc $^ -o $@ -L. -l3dengine -lxgfx

lib3dengine.so: rotate.o engine.o
	gcc -shared $^ -lxgfx -lm -o $@

%.o: %.c
	gcc -c $^ -fPIC -o $@ -g

clean:
	rm -f *.o demo lib3dengine.so
