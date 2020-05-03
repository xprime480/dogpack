PROGS = test
LIB = ../bin/libvm.a

$(PROGS) : % : %.cpp $(LIB)
	g++ -o $@ -I ../include $< -L ../bin -lvm
