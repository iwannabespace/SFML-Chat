Include = /opt/homebrew/Cellar/sfml/2.6.1/include
Lib = /opt/homebrew/Cellar/sfml/2.6.1/lib
Flags = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused

all:
	g++ main.cpp src/*.cpp -I${Include} -L${Lib} -std=c++17 -o chat-server -lsfml-network -lsfml-system -lsfml-audio -lsfml-graphics ${Flags}

run: all
	./chat-server