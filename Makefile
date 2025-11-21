all:
	gcc target.c -o target
	g++ needle.cpp -o needle

clean:
	rm -f target needle
