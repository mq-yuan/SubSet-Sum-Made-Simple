default: all
all: clean main dynamic


main:
	g++ main.cpp -o main.exe

dynamic:
	g++ dynamic.cpp -o dynamic.exe

clean:
	$(RM) dynamic.exe main.exe

