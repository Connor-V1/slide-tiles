
all: compile link run clean

compile:
	g++ -c *.cpp -std=c++17 -g -Wall -m64 -I "C:/SFML-2.5.1/include" -DSFML_STATIC 
link:
	g++ *.o -o main -L C:/SFML-2.5.1/lib  -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-network-s -lsfml-audio-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32

clean:
	del *.o
	
run:
	./main