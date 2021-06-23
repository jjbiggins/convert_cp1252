TARGET_EXEC := convert_cp1252
SRC_DIR := src
INC_DIR := include

$(TARGET_EXEC): 
	gcc -o $(TARGET_EXEC) $(SRC_DIR)/*.c -I $(INC_DIR) -Wall -Wextra -Wpedantic -std=c11

.PHONY: run
run: $(TARGET_EXEC) test
	./$(TARGET_EXEC) files/cp1251.txt cp1252 test/cp1251.txt

.PHONY: clean
clean:
	-rm $(TARGET_EXEC)

.PHONY: test
test:
	mkdir -p test
