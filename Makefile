########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g -std=c++11

MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 


#First Program (ListTest)

Cpp_OBJ1=image.o DisjSets.o p1.o

PROGRAM_1=p1

$(PROGRAM_1): $(Cpp_OBJ1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ1) $(INCLUDES) $(LIBS_ALL)


#Second Program

Cpp_OBJ2=image.o DisjSets.o p2.o

PROGRAM_2=p2

$(PROGRAM_2): $(Cpp_OBJ2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ2) $(INCLUDES) $(LIBS_ALL)


#Third Program

Cpp_OBJ3=image.o DisjSets.o p3.o

PROGRAM_3=p3

$(PROGRAM_3): $(Cpp_OBJ3)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ3) $(INCLUDES) $(LIBS_ALL)


#Fourth Program

Cpp_OBJ4=image.o DisjSets.o p4.o

PROGRAM_4=p4

$(PROGRAM_4): $(Cpp_OBJ4)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ4) $(INCLUDES) $(LIBS_ALL)


all: 
	make $(PROGRAM_1) 
	make $(PROGRAM_2)
	make $(PROGRAM_3)
	make $(PROGRAM_4)


clean:
	(rm -f *.o; rm -f $(PROGRAM_1); rm -f $(PROGRAM_2); rm -f $(PROGRAM_3); rm -f $(PROGRAM_4))

(:
