# New makefile to build the Cyclone physics engine for Linux.

# Determine architecture (Linux or Mac OS X).
PLATFORM = $(shell uname)

ifeq ($(PLATFORM), Linux)
    LDFLAGS = -lGL -lGLU -lglut
else
    $(error This OS is not Ubuntu Linux. Aborting)
endif

# Demo files path.
DEMOPATH = ./src/demos2/

# Demo core files.
DEMOCOREFILES = $(DEMOPATH)main2.cpp $(DEMOPATH)app2.cpp $(DEMOPATH)timing2.cpp

# Demo files.
DEMOLIST = ballistic blob bridge fireworks platform 

# Cyclone core files.
CYCLONEFILES =  ./src/cyclone2cpp/core2.cpp ./src/cyclone2cpp/particle2.cpp ./src/cyclone2cpp/pcontacts2.cpp ./src/cyclone2cpp/pfgen2.cpp ./src/cyclone2cpp/plinks2.cpp ./src/cyclone2cpp/pworld2.cpp ./src/cyclone2cpp/random2.cpp 

.PHONY: clean

all: $(DEMOLIST)

$(DEMOLIST):
	g++ -O2 -Iinclude $(DEMOCOREFILES) $(CYCLONEFILES) $(DEMOPATH)$@/$@.cpp -o $@ $(LDFLAGS) 

clean:
	rm $(DEMOLIST)
