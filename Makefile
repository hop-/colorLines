default:
	g++ $(debug_flag) -Wall -c logic.cpp
	g++ $(debug_flag) -Wall -c events.cpp
	g++ $(debug_flag) -Wall -c baseGui.cpp
	g++ $(debug_flag) -Wall -c gui.cpp
	g++ $(debug_flag) -Wall -c game.cpp
	g++ $(debug_flag) -Wall -c main.cpp
	g++ $(debug_flag) -Wall main.o logic.o baseGui.o gui.o events.o game.o -o cLines -lSDL2 -lSDL2_image -lSDL2_ttf
clean:
	rm -rf *.o cLines
