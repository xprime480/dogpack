

LIB = ../lib/libk9common.a

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(SRC:.cpp=.d)

CPP = /usr/bin/g++
INC =  -I ../include
CPPFLAGS = -g -Wall $(INC)

all : $(LIB)

$(LIB) : $(OBJ)
	ar crf	$@ $(OBJ) $(SUBOBJ)

$(OBJ) : %.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $<

$(DEP) : %.d : %.cpp
	$(CPP) $(CPPFLAGS)  -MM $< -o $*.d

clean :
	-@rm -f *.o *.d *.a

include $(DEP)
