
py_z85.so: py_z85.c py_z85.h
	gcc -std=c99 -Wall -Wextra -Wpedantic -shared -fPIC $< -o $@

.PHONY: clean
clean:
	git clean -dxf
