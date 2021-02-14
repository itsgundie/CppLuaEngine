build:
	g++ -g -w -std=c++14 \
	./src/*.cpp \
	-o game \
	-I "./lib/lua-5.4.2" \
	-I "./inc/" \
	-L "./lib/lua-5.4.2" \
	-llua \
	-I "./lib/SDL2/include" \
	-I "./lib/SDL2_image/" \
	-I "./lib/SDL2_ttf/" \
	-L "./lib/SDL2/build" \
	-lSDL2 \
	-L "./lib/SDL2_image/" \
	-lSDL2_image \
	-L "./lib/lSDL2_ttf" \
	-lSDL2_ttf

sdl2:
	sh installSDL2.sh

clean:
	rm ./game:

run:
	@./game;
