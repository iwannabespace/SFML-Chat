Include = /opt/homebrew/Cellar/sfml/2.6.1/include
Lib = /opt/homebrew/Cellar/sfml/2.6.1/lib

all:
	g++ main.cpp src/*.cpp -I${Include} -L${Lib} -std=c++17 -o chat -lsfml-network -lsfml-system -lsfml-audio -lsfml-graphics -lsfml-window

run: all
	./chat