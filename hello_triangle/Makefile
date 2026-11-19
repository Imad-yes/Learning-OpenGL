INCLUDEPATH = ../include/
OBJS = hello.o glad.o
LIBFLAGS = -lGL -lglfw

main: $(OBJS)
	g++ $(LIBFLAGS) $(OBJS) -o main

%.o: %.cpp
	g++ -c -I$(INCLUDEPATH) $(LIBFLAGS) $< -o $@

%.o: %.c
	g++ -c -I$(INCLUDEPATH) $(LIBFLAGS) $< -o $@

clean:
	rm *.out *.o main


