TARGET = bin/main
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	$(TARGET) -n -f ./newFile.db


default: $(TARGET)

clean:
	rm -f obj/*.o
# 	rm -f src/*

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude
