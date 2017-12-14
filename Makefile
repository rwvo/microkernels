HCCBINDIR=/opt/rocm/hcc/bin

CXX = $(HCCBINDIR)/hcc
CXXFLAGS = $(shell $(HCCBINDIR)/hcc-config --cxxflags) -g
LDFLAGS = $(shell $(HCCBINDIR)/hcc-config --ldflags) -lhc_am

CXXFLAGS += -O3

BENCHMARKS = 00_show_accelerators \
             01_memcopy_bandwidth
COMMON_OBJS = utils.o 

OBJECTS = $(patsubst %,%.o,$(BENCHMARKS))
OBJECTS += $(COMMON_OBJS)
DEPS =  $(patsubst %.o,%.d,$(OBJECTS))

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
	$(CXX) -MM $(CXXFLAGS) $*.cpp -o $*.d

%: %.o
	$(CXX) $(LDFLAGS) $< $(COMMON_OBJS) -o $@

all: $(BENCHMARKS)

$(BENCHMARKS): $(OBJECTS)

clean:
	rm -f $(BENCHMARKS) *.o *.d *~

-include $(DEPS)
