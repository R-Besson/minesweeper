standard: compile link clean
debug: dbcompile link clean

compile:
	g++ -c src/sweeper.cpp -O3
	g++ -c src/main.cpp -I src/include -O3
dbcompile:
	g++ -c src/sweeper.cpp -g -O0
	g++ -c src/main.cpp -I src/include -g -O0
link:
	g++ main.o sweeper.o -o minesweeper -lsfml-graphics -lsfml-window -lsfml-system
clean:
	rm *.o
	rm -rf build
	mkdir build
	mv minesweeper build/minesweeper
	cp -r src/assets build/assets
