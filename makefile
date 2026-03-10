# files
WINDOW = window.hpp
MAIN = main.cpp 

# binary
TARGET = rpg 

# CXXFLAGS
STD = -std=c++17
WARNINGS = -Wpedantic
SYMBOLS = -g 
SANITIZE = -fsanitize=undefined
CC = g++ 
CXXFLAGS = $(STD) $(WARNINGS) $(SYMBOLS) $(SANITIZE)

all: $(TARGET)

$(TARGET): $(WINDOW) $(MAIN)
	$(CC) $(CXXFLAGS) -o $@ $(MAIN)



clean:
	rm -f $(TARGET)


.PHONY: all clean
