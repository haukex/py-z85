
spec_32: spec_32.c
	gcc -std=c99 -Wall -Wextra -Wpedantic $< -o $@

.PHONY: clean
clean:
	git clean -dxf
