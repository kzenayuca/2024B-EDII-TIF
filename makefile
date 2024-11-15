# Compiler settings
CXX = g++
CXXFLAGS = $(shell pkg-config --cflags opencv4)
LDFLAGS = $(shell pkg-config --libs opencv4) -lssl -lcrypto

# Source files
SOURCES = main.cpp huffman.cpp mapasCaoticos.cpp AES.cpp LSB.cpp
EXECUTABLE = run

# Default target
all: $(EXECUTABLE)

# Linking rule
$(EXECUTABLE): $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) $(LDFLAGS) -o $(EXECUTABLE)

# Clean rule
clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean