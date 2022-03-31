#variable declaration :-
cc=gcc      
MAKE=make
RM =rm

#Flags
FLAG =-Wall

#targets .
all: tideman.c 
	$(cc) $(FLAG) -o tideman tideman.c -lbsd	

	
clean: tideman
	$(RM) *.o
