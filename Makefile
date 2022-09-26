
coursework2: coursework2.o helper.o parking_bst.o
	g++ coursework2.o helper.o parking_bst.o -o coursework2

coursework2.o: coursework2.cpp
	g++ -c coursework2.cpp

helper.o: helper.h helper.cpp
	g++ -c helper.cpp

parking_bst.o: parking_bst.h parking_bst.cpp
	g++ -c parking_bst.cpp

clean:
	rm *.o *.log *.stackdump *.exe tests coursework2