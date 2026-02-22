TARGET = out

IDIR = .
SDIR = .
ODIR = .

SRC    = $(wildcard $(SDIR)/*.cpp)
OBJ    = $(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o)
DEPS   = $(OBJ:.o=.d)

CXX      = g++
CXXFLAGS = -I$(IDIR) -g -O0 -Iinclude -MMD -MP
CFLAGS   = -Wall
LIBS     = -lm -lsystemc -lstdc++

# SystemC path — override via env var: SCPATH=/path/to/systemc make
SCPATH ?= /usr/local/systemc-2.3.3

# Platform detection
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    SCLIB = lib-macosx64
else
    SCLIB = lib-linux64
endif

LDFLAGS = -L$(SCPATH)/$(SCLIB) -Wl,-rpath $(SCPATH)/$(SCLIB)

# Build only
all: $(TARGET)

# Build + run + regression check
run: all
	./$(TARGET)
	@$(MAKE) cmp_result

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(SCPATH)/include $(LDFLAGS) $^ $(LIBS) -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -I$(SCPATH)/include -c $< -o $@

-include $(DEPS)

clean:
	$(RM) $(TARGET) $(OBJ) $(DEPS) output.dat

GOLD_DIR  = ./golden
GOLD_FILE = $(GOLD_DIR)/ref_output.dat

cmp_result:
	@echo "**********************************************"
	@if diff -w $(GOLD_FILE) ./output.dat ; then \
		echo "Simulation passed"; \
	else \
		echo "Simulation failed"; \
	fi
	@echo "**********************************************"
