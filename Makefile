
spec_32.so: spec_32.c spec_32.h
	gcc -std=c99 -Wall -Wextra -Wpedantic -shared -fPIC $< -o $@

.PHONY: clean
clean:
	git clean -dxf
