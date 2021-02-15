.PHONY: build
build:
	g++ -g -w -std=c++17 \
	./src/*.cpp \
	-o game \
	-I "inc/" \
	-I "./lib/SDL2_image/" \
	-I "./lib/SDL2/include/" \
	-I "./lib/SDL2_ttf/" \
	-I "./lib/sol2-develop/include/" \
	-I "./lib/lua-5.3.0/install/" \
	-I "./lib/glm-0.9.9.8/" \
	-I "./lib/imgui-1.81/" \
	-L "./lib/lua-5.3.0/install/lib/" \
	-llua \
	-L "./lib/SDL2/build" \
	-lSDL2 \
	-L "./lib/SDL2_image/" \
	-lSDL2_image \
	-L "./lib/lSDL2_ttf" \
	-lSDL2_ttf

libs:
	sh installSDL2.sh

clean:
	rm game

run:
	@./game;
