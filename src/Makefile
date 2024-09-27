GCFLAGS := -Wall -Werror -Wextra -std=c11

TETRIS_DIR := brick_game/tetris
TESTS_DIR := tests
GUI_DIR := gui/cli
BUILD_DIR := game
DIST_DIR = Tetris
CHECK_PATH = $(shell brew --prefix check)
INCLUDES = -I$(CHECK_PATH)/include
GCFLAGS += $(INCLUDES)
SOURCES := $(TETRIS_DIR)/backend.c $(GUI_DIR)/frontend.c main.c
OBJECTS := $(TETRIS_DIR)/*.o $(GUI_DIR)/*.o

ifeq ($(shell uname -s), Linux)
	TEST_FLAGS = -lcheck -lpthread -pthread -lm  -lrt -lsubunit
	OPEN = xdg-open
	LDFLAGS := -lncursesw
else
	TEST_FLAGS = -L$(CHECK_PATH)/lib -lcheck -lpthread -pthread -lm 
	LDFLAGS := -lncurses
	OPEN = open
endif

.PHONY: all clean install uninstall dvi dist test gcov_report start_game

all: clean start_game

install:
	mkdir -p $(BUILD_DIR)
	gcc $(GCFLAGS) $(SOURCES) -o $(BUILD_DIR)/tetris $(LDFLAGS)

uninstall:
	rm -rf $(BUILD_DIR) $(OBJECTS)

# цель для запуска проекта
start_game:
	@mkdir -p $(BUILD_DIR)
	gcc $(GCFLAGS) $(SOURCES) -o $(BUILD_DIR)/tetris $(LDFLAGS)
	./$(BUILD_DIR)/tetris

# цель для открытия html документации
dvi:
	$(OPEN) Documentation.html

# цель для создания дистрибутивного пакета проекта
dist: clean
	@mkdir -p $(DIST_DIR)
	cp -a brick_game $(DIST_DIR)
	cp -a gui $(DIST_DIR)
	cp -a tests $(DIST_DIR)
	cp -a *.c $(DIST_DIR)
	cp -a Documentation.html $(DIST_DIR)
	tar -czf s21_Tetris.tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)

test:
	@mkdir -p $(BUILD_DIR)
	gcc $(GCFLAGS) -g $(TETRIS_DIR)/backend.c $(TESTS_DIR)/*.c -o $(BUILD_DIR)/tetris $(LDFLAGS)  $(TEST_FLAGS)
	./$(BUILD_DIR)/tetris


gcov_report: clean
	@mkdir -p $(BUILD_DIR)  
	gcc $(TETRIS_DIR)/backend.c $(TESTS_DIR)/*.c -o $(BUILD_DIR)/tetris $(LDFLAGS) $(GCFLAGS) $(TEST_FLAGS) -fprofile-arcs -ftest-coverage 
	./$(BUILD_DIR)/tetris
	lcov -t "$(TETRIS_DIR)/tetris.c" -o rep.info -c -d .
	genhtml -o report rep.info
	open ./report/index.html
	rm -rf *.gcno *.gcda



style_fix:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -style=Google -i */*/*.c */*/*.h */*.c *.c
	rm .clang-format

style:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -style=Google -n */*/*.c */*/*.h */*.c *.c
	rm .clang-format

docker_build:
	cp ../materials/linters/.clang-format ./
	docker build -t vault:v1 .
	docker create -it --name decimal_vault vault:v1

docker_start:
	docker start -i decimal_vault

docker_delete:
	docker rm decimal_vault
	docker image rm vault:v1

valgrind: rebuild
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=leaks_log.txt ./test.out
	echo --- Valgrind summary --- && cat leaks_log.txt | grep 'total heap usage' && cat leaks_log.txt | grep 'ERROR SUMMARY' 


rebuild: clean install

clean:
	rm -rf $(BUILD_DIR) $(OBJECTS) *.info gcov_report s21_Tetris.tar.gz report *.gcno *.gcda