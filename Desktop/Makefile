TARGET := ardusdl
VPATH := ../CP437
PKGCONFIG := $(shell which pkg-config)
CXXFLAGS := -g $(shell $(PKGCONFIG) --cflags sdl2)
LDLIBS := $(shell $(PKGCONFIG) --libs sdl2)
SOURCES := $(wildcard $(VPATH)/*.cpp *.cpp)
OBJECTS := $(SOURCES:%.cpp=%.o)
INCLUDES := $(wildcard $(VPATH)/*.h *.h)

# show:
# 	@echo SOURCES $(SOURCES)
# 	@echo OBJECTS $(OBJECTS)
# 	@echo INCLUDES $(INCLUDES)

all: $(TARGET)

%.o : %.cpp $(INCLUDES)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LDLIBS)


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS) Font5x7.o
