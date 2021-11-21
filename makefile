TARGET = run
LIBS = -lm #Math Library
HEADERS = Node.h
SRCS = main.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(SRCS))
CXX = g++
CXX_FLAGS = -Wall -std=c++14

.PHONY: default all clean

all: depend $(TARGET)

#Rules to recompile template headers
depend: .depend
.depend: $(HEADERS)
	rm -f ./.depend
	$(CXX) $(CXX_FLAGS) -MM $^ > ./.depend;
include .depend

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f ./.depend
	-rm -f $(TARGET)
