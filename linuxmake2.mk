PLATFORM = $(shell uname)

ifeq ($(PLATFORM), Linux)
    LDFLAGS = -lGL -lGLU -lglut
else
    $(error This OS is not Ubuntu Linux. Aborting)
endif

DEMOPATH = ./src/demos2/
BINDIR = $(DEMOPATH)bin

DEMOCOREFILES = $(DEMOPATH)main2.cpp $(DEMOPATH)app2.cpp $(DEMOPATH)timing2.cpp
DEMOLIST = ballistic blob bridge fireworks platform

CYCLONEFILES = ./src/cyclone2cpp/core2.cpp ./src/cyclone2cpp/particle2.cpp ./src/cyclone2cpp/pcontacts2.cpp \
               ./src/cyclone2cpp/pfgen2.cpp ./src/cyclone2cpp/plinks2.cpp ./src/cyclone2cpp/pworld2.cpp ./src/cyclone2cpp/random2.cpp

.PHONY: all clean

# Main build rule: only care about bin/* binaries
all: | $(BINDIR) $(addprefix $(BINDIR)/,$(DEMOLIST))

# Create bin/ if missing
$(BINDIR):
	mkdir -p $(BINDIR)

# Pattern rule: build each binary into bin/
$(BINDIR)/%:
	g++ -O2 -Iinclude $(DEMOCOREFILES) $(CYCLONEFILES) $(DEMOPATH)$*/$*.cpp -o $@ $(LDFLAGS)

# Clean only bin/
clean:
	rm -f $(BINDIR)/*