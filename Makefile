DEBUG=-O4
CPP=g++
CFLAGS=-Wall -fmessage-length=0  -std=c++0x  -Wextra -pedantic -pedantic-errors $(DEBUG)
LDFLAGS=

#

# SOURCES.
SOURCES=DynGraph.cpp UDynGraph.cpp DynGraphUtils.cpp DSAlgs.cpp DynDSAlg.cpp DynDSAlgAddRem.cpp GraphScheduler.cpp Stats.cpp
BINARY_SOURCES=ExecAdd.cpp ExecAddRem.cpp 

# OBJECTS.
OBJECTS=$(SOURCES:.cpp=.o)
BINARY_OBJECTS=$(BINARY_SOURCES:.cpp=.o)
ALL_LOCAL_OBJECTS=$(OBJECTS) $(BINARY_OBJECTS)

# DEPENDENCIES.
DEPS=$(patsubst %.o,%.d,$(ALL_LOCAL_OBJECTS))

# BINARIES.
BINARIES=$(BINARY_SOURCES:.cpp=)

# RULES.

all: $(BINARIES) $(OBJECTS)

$(ALL_LOCAL_OBJECTS): %.o: %.cpp
	$(CPP) -MMD -MP $(CFLAGS) -c $< -o $@
	@sed -i -e '1s,\($*\)\.o[ :]*,\1.o $*.d: ,' $*.d

$(BINARIES): %: %.o $(OBJECTS)
	$(CPP) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(DEPS) $(ALL_LOCAL_OBJECTS) $(BINARIES)

-include $(DEPS)
