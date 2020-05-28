################################################################################
#
# Makefile for C++ projects.
#
# It compile and links C/C++ sources and links them to the main C++ file
#
# Version valid for UNIX systems
#
# "make" compiles and links the specified main programs and modules
# and produces the executables
# 
# "make clean" removes all files created by "make"
#
################################################################################

all: rmxeq mkdep cmpsc mkxeq # rmpmk
.PHONY: all

#
# main programs and required modules
#

MAIN = PONI  PONIpattern

# modules and C++ classes

GRN = grnfunc  poni

CXXMODULES = $(GRN)


# modules in C

RANDOM = ranlxs ranlxd gauss

START = start utils

CMODULES = $(RANDOM) $(START)



# search path for modules

MDIR = ../modules

VPATH = $(MDIR)/grn:$(MDIR)/random:$(MDIR)/start



# additional include directories

INCPATH = ../include /usr/include
# /usr/include/mpich-x86_64


# additional libraries to be included 

CLIBS = m

CXXLIBS = mpi

LIBPATH =


# scheduling and optimization options (such as -DSSE -DSSE2 -DP4)
 
CFLAGS = -std=c99 -lm -O3 -g -DSSE4 -Wall -pedantic

CXXFLAGS = -O3 -g -DSSE4 -fPIC  -Wall -pedantic
 

SHELL=/bin/bash

CC= gcc  
# mpicc

CXX= g++ -std=c++11 
# mpic++

LD= g++ -std=c++11 
# mpic++


############################## do not change ###################################


CXXSRCS= $(MAIN) $(CXXMODULES)	# this contains only the C++ sources (to compile)

PGMS= $(CXXSRCS) $(CMODULES) 	# this contains all sources (for linking)

INCDIRS = $(addprefix -I,$(INCPATH))

OBJECTS = $(addsuffix .o,$(CMODULES)) $(addsuffix .o,$(CXXMODULES))

LDFLAGS = $(addprefix -L,$(LIBPATH)) $(addprefix -l,$(LIBS))

-include $(addsuffix .d,$(PGMS))


# rule to make dependencies

$(addsuffix .d,$(CMODULES)): %.d: %.c Makefile		# only C sources
	@ $(CC) -MM $(INCDIRS) $< -o $@  # -ansi

$(addsuffix .d,$(CXXMODULES)): %.d: %.cc Makefile	# only C++ modules/classes
	@ $(CXX) -MM $(INCDIRS) $< -o $@

$(addsuffix .d,$(MAIN)): %.d: %.cpp Makefile		# only C++ main programs
	@ $(CXX) -MM $(INCDIRS) $< -o $@



# rule to compile sources

$(addsuffix .o,$(CMODULES)): %.o: %.c Makefile		# only C sources
	$(CC) $< -c $(CFLAGS) $(INCDIRS) -o $@

$(addsuffix .o,$(CXXMODULES)): %.o: %.cc Makefile	# only C++ modules/classes
	$(CXX) $< -c $(CXXFLAGS) $(INCDIRS) -o $@

$(addsuffix .o,$(MAIN)): %.o: %.cpp Makefile		# only C++ main programs
	$(CXX) $< -c $(CXXFLAGS) $(INCDIRS) -o $@



# rule to link object files

$(MAIN): %: %.o $(OBJECTS) Makefile
	$(LD) $< $(OBJECTS) $(CXXFLAGS) $(LDFLAGS) -o $@



# produce executables

mkxeq: $(MAIN)
	@ printf "\n"
	@ echo -e "produced executables: \e[1;31m$(MAIN)\e[0m"


# compile sources

cmpsc: $(addsuffix .o,$(PGMS))
	@ printf "\n"
	@ echo "link compiled C/C++ sources"


# remove old executables and old error log file

rmxeq:
	@ -rm -f $(MAIN); \
        echo "delete old executables"



# make dependencies

mkdep:  $(addsuffix .d,$(PGMS))
	@ printf "\n"
	@ echo "generate dependencies and compile sources"


# clean directory and executables only remain

rmpmk:
	@ -rm -rf *.d *.o .tmp
	@ echo "remove temporary files"
	@ printf "\n GOOD!!\n"


# clean directory 

clean:
	@ -rm -rf *.{d,o,tmp} #$(MAIN)
	@ echo "removed all build files"
.PHONY: clean

################################################################################
