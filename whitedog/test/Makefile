PROGS = test

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(SRC:.cpp=.d)

CPP = /usr/bin/g++
INC =  -I ../../common/include -I ../include
LNK = -L ../../common/lib -L ../lib
CPPFLAGS = -g -Wall $(INC)

LIBS = ../lib/libvm.a ../../common/lib/libk9common.a
.PHONY : all 

all : 
	make -C ../../common/src all
	make -C ../src all
	make main

main: $(PROGS) $(LIBS)
	./test

$(PROGS) : % : %.cpp $(LIBS)
	g++ -o $@ $(INC) $<  $(LNK) -lvm -lk9common

$(DEP) : %.d : %.cpp
	$(CPP) $(CPPFLAGS)  -MM $< -o $*.d

clean :
	-@rm -f *.o *.d *.a

include $(DEP)
