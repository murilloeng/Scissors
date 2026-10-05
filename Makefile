#compiler
CXX = g++
WARS = -Wall -Werror
INCS = -I .. -I /usr/include/freetype2
LIBS = -l openblas -l umfpack -l arpack -l quadrule -l fftw3 -l gmsh -l glfw
CXXFLAGS = -std=c++23 -fPIC -pipe -fopenmp -MT $@ -MMD -MP -MF $(subst .o,.d, $@) $(DEFS) $(INCS) $(WARS)

#mode
ifneq ($(m), r)
	mode = debug
	CXXFLAGS += -ggdb3
else
	mode = release
	CXXFLAGS += -Ofast
endif

#ouput
out = dist/$(mode)/scissors.out

#libraries
lib_fea = ../FEA/dist/$(mode)/libfea.so
lib_math = ../Math/dist/$(mode)/libmath.so
lib_canvas = ../Canvas/dist/$(mode)/libcanvas.so
lib_sections = ../Sections/dist/$(mode)/libsections.so
lib_materials = ../Materials/dist/$(mode)/libmaterials.so

#sources
src := $(sort $(shell find -path './src/*.cpp'))

#objects
obj = $(sort $(subst ./src/,build/$(mode)/,$(subst .cpp,.o,$(src))))

#dependencies
dep = $(subst .o,.d,$(obj))

#rules
all : exe

run : exe
	./$(out)

exe : fea $(out)
	@echo 'executable - $(mode): complete!'

fea : 
	+@cd ../FEA && $(MAKE) -f Makefile lib m=$m

$(out) : $(obj)
	@mkdir -p $(dir $@)
	@g++ -o $(out) $(obj) $(lib_fea) $(lib_sections) $(lib_materials) $(lib_math) $(lib_canvas) $(LIBS)
	@echo 'linking - $(mode): $@'

build/$(mode)/%.o : src/%.cpp build/$(mode)/%.d
	@mkdir -p $(dir $@)
	@echo 'compiling - $(mode): $<'
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(dep) : ;

-include $(dep)

clean :
	@rm -rf dist/$(mode)
	@rm -rf build/$(mode)
	@echo 'clean - $(mode): complete!'

print-% :
	@echo $* = $($*)

.PHONY : all clean print-%