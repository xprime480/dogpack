PROGS = test
LIB = ../bin/libvm.a

$(PROGS) : % : %.cpp $(LIB)
	g++ -o $@ $< -L ../bin -lvm
