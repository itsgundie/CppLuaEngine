NAME=MythOfHilda.game

CFLAGS = g++ -g -Wall -Wfatal-errors -std=c++17

LIBS_HEADERS = -I "./inc/" \
				-I "./src/ECS/" \
				-I "./src/ECS/Components/" \
				-I "./src/ECS/Systems/" \
				-I "./lib/SDL2/include/" \
				-I "./lib/SDL2_image/" \
				-I "./lib/SDL2_ttf/" \
				-I "./lib/sol2-develop/include/" \
				-I "./lib/lua-5.3.0/install/" \
				-I "./lib/glm-0.9.9.8/" \
				-I "./lib/imgui-1.81/" \

LIBS = 	-L "./lib/lua-5.3.0/install/lib/" \
		-llua \
		-L "./lib/SDL2/build" \
		-lSDL2 \
		-L "./lib/SDL2_image/" \
		-lSDL2_image \
		-L "./lib/SDL2_ttf" \
		-lSDL2_ttf


SRCS = ./src/*.cpp \
		./src/ECS/*.cpp

INCS = ./inc/*.h \
./inc/*.hpp


.PHONY: build
build:
	$(CFLAGS) $(SRCS) $(LIBS_HEADERS) $(LIBS) -o $(NAME)

libs:
	sh installSDL2.sh

clean:
	rm $(NAME)

run:
	@./$(NAME);
