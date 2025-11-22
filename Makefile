TARGET = bin/game
SRC = $(wildcard code/src/*.cpp)
OBJ = $(patsubst code/src/%.cpp, code/obj/%.o, $(SRC))
###################################
# Build Everything
###################################
default: from_scratch

from_scratch: clean build
###################################

###################################
# Build Game
###################################
build: $(TARGET)

$(TARGET): $(OBJ)
	g++ -o $@ $? -std=c++20 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

code/obj/%.o : code/src/%.cpp
	g++ -c $< -o $@ -Icode/include -Icode/libs/include

clean:
	rm -f code/obj/*.o
	rm -f $(TARGET)*
###################################

###################################
# Run Game
###################################
run:
	./$(TARGET)
###################################
