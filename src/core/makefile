all : utils.o exact greedy grasp tabu
.PHONY : all
utils.o:utils.cpp utils.hpp
	g++ -Wall -std=c++17 utils.cpp -c -lstdc++fs -o utils.o
exact:exact.cpp utils.o
	g++ -Wall -std=c++17 utils.o exact.cpp -lstdc++fs -o exact
greedy:greedy.cpp utils.o
	g++ -Wall -std=c++17 utils.o greedy.cpp -lstdc++fs -o greedy
grasp:grasp.cpp utils.o
	g++ -Wall -std=c++17 utils.o grasp.cpp -lstdc++fs -o grasp
tabu:tabu.cpp utils.o
	g++ -Wall -g -std=c++17 utils.o tabu.cpp -lstdc++fs -o tabu
.PHONY:clean
clean:
	rm -f exact greedy grasp utils.o tabu