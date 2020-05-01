PROGS = test
OBJ = ../bin/vm.o

$(PROGS) : % : %.cpp $(OBJ)
	make -C ../src
	g++ -o $@ $< $(OBJ)
