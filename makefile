SRC = main.cpp
EXEC = metaheuristic_project
SCRIPT = compare_methods.zsh
PYTHON_FILE = /Users/dawid/PycharmProjects/compareMethods/compare_method.py

CXX = g++
CXXFLAGS = -std=c++20

all: $(EXEC)

$(EXEC): $(SRC)
	@echo "\nKompilowanie programu...\n"
	$(CXX) -o $(EXEC) $(SRC) $(CXXFLAGS)

clean:
	@echo "\nUsuwanie pliku wykonywalnego...\n"
	rm -f $(EXEC)

script: $(SCRIPT)
	@echo "\nWybór metod\n"
	@zsh $(SCRIPT)

python:
	@echo "\nUruchamianie skryptu pythona\n"
	@python3 $(PYTHON_FILE)


.PHONY: all clean script python