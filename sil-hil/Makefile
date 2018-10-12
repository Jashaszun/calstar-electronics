CC=cc
CXX=c++
RM=rm -f

CPPFLAGS= -g -std=c++11
LDFLAGS= -g
LDLIBS=

SRCS=Simulator.cc SIL.cc Harness.cc test_code.cc common.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: sil

sil: $(OBJS)
	$(CXX) $(LDFLAGS) -o sil $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)
