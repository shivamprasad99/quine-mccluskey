all: A B

A: taskA.o
	g++ taskA.o -o A

B: taskB.o
	g++ taskB.o -o B

taskA.o: taskA.cpp
	g++ -c taskA.cpp

taskB.o: taskB.cpp
	g++ -c taskB.cpp

clean:
	rm *o A B output.txt outputB.txt
