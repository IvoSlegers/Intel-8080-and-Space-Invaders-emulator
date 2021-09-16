header_dir = headers
source_dir = src
lib_dir = lib

headers := $(wildcard $(header_dir)/*.cpp) $(wildcard $(header_dir)/*/*.hpp)
sources := $(wildcard $(source_dir)/*.cpp) $(wildcard $(source_dir)/*/*.cpp)
objects := $(patsubst $(source_dir)/%.cpp,$(lib_dir)/%.o,$(sources))

# use this variable to place the .o files in the same directory as the source files.
#objects := $(sources:.cpp=.o)

include_directories := -IC:\\Users\\ivoms\\Documents\\programming\\cpp\\libraries\\sfml\\source\\include
include_directories += -Iheaders\\.

debug : link_directories := -LC:\\Users\\ivoms\\Documents\\programming\\cpp\\libraries\\sfml\\bin\\debug
release : link_directories := -LC:\\Users\\ivoms\\Documents\\programming\\cpp\\libraries\\sfml\\bin\\release

CXX := g++
CPPFLAGS := 
CXXFLAGS := -std=c++17 $(include_directories)

debug : CPPFLAGS += -D _DEBUG
debug : CXXFLAGS += -g -Wall -Werror -Og

release : CPPFLAGS += -O3

link_options := -lopengl32

debug : link_options += -lsfml-main-d
debug : link_options += -lsfml-graphics-d
debug : link_options += -lsfml-system-d
debug : link_options += -lsfml-window-d

release : link_options += -lsfml-main
release : link_options += -lsfml-graphics
release : link_options += -lsfml-system
release : link_options += -lsfml-window

debug : $(objects)
	$(CXX) $(objects) $(link_directories) $(link_options) -o application_debug.exe

release : $(objects)
	$(CXX) $(objects) $(link_directories) $(link_options) -o application.exe

$(objects) : $(headers)
