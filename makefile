# invoke this makefile by using either "make debug" or "make release"
# add the option -j <max number of tasks> for running tasks in parallel.

# specify the path of the directories containing headers and source files
header_dir := headers
source_dir := src

# specify the directory where debug and release subfolders that contain .o files will be placed.
# directory will be created when it does not exists
lib_dir := lib

# specify any include paths for headers
include_directories := -IC:\\Users\\ivoms\\Documents\\programming\\cpp\\libraries\\sfml\\source\\include
include_directories += -Iheaders\\.

# specify any include paths for the linker
debug : link_directories := -LC:\\Users\\ivoms\\Documents\\programming\\cpp\\libraries\\sfml\\bin\\debug
release : link_directories := -LC:\\Users\\ivoms\\Documents\\programming\\cpp\\libraries\\sfml\\bin\\release

# specify external .a files to link against
link_options := -lopengl32

debug : link_options += -lsfml-main-d
debug : link_options += -lsfml-graphics-d
debug : link_options += -lsfml-system-d
debug : link_options += -lsfml-window-d
debug : link_options += -lsfml-audio-d

release : link_options += -lsfml-main
release : link_options += -lsfml-graphics
release : link_options += -lsfml-system
release : link_options += -lsfml-window
release : link_options += -lsfml-audio-d

# specify the name of the output executable
debug : output_file = application_debug
release : output_file = application

# Don't edit below this line
# --------------------------------------------------------------------

# specify the folder where .o files will be placed.
#debug : objects_dir := $(lib_dir)/debug
#release : objects_dir := $(lib_dir)/release
objects_dir := lib/debug

# search for all .hpp header files in headers directory + all .hpp header files in direct subdirectories.
# doing a full recursive search seems hard?
headers := $(wildcard $(header_dir)/*.cpp) $(wildcard $(header_dir)/*/*.hpp)

# search for all .cpp source files in source directory + all .cpp source files in direct subdirectories.
sources := $(wildcard $(source_dir)/*.cpp) $(wildcard $(source_dir)/*/*.cpp)

# make a list of all .o files that need to be generated.
# $(notdir ) strips the filesnames of the source files of their directory prefixes so all .o files will be places in objects_dir
objects := $(patsubst %.cpp,$(objects_dir)/%.o,$(notdir $(sources)))

# if you do not want to make a separate directory for lib files then we need
# objects := $(sources:.cpp=.o)
# also: in this case its easier to let the implicit rules do their work

# include any directory in which .cpp files where found in the vpath for .cpp
# this allows automatic rules to find .cpp files in subdirectories
vpath %.cpp : $(sort $(dir $(sources)))

# specify compiler to use (must be in directory that is included in PATH variable)
CXX := g++

# specify preprocessor flags
CPPFLAGS := 

debug : CPPFLAGS += -D _DEBUG

# specify compiler flags
CXXFLAGS := -std=c++17 $(include_directories)

debug : CXXFLAGS += -g -Wall -Werror -Og

release : CPPFLAGS += -O3

# specify a (canned) recipe for linking the final executable
define link-executable =
$(CXX) $(objects) $(link_directories) $(link_options) -o $(output_file)
endef

# rule for debug build
debug : $(objects)
	$(link-executable)

# rule for release build
release : $(objects)
	$(link-executable)

# automatic rule for building the .o files
$(objects_dir)/%.o : %.cpp $(headers) $(objects_dir)
	$(CXX) $< $(CPPFLAGS) $(CXXFLAGS) -c -o $@

# use this when implicit rule for compiling .cpp files is used.
#$(objects) : $(headers)

# create objects directory if it doesn't exist
# it turns out that a directory can also be specified as a requirement
# need to be carefull here gcc make defines paths with forward slashes
# however the windows command mkdir only works on paths with backslashes
# hence the replacement $(subst /,\,$(objects_dir)
$(objects_dir) :
	mkdir $(subst /,\,$(objects_dir))

	