CC=cc
CXX=c++
RM=rm -f

CPPFLAGS= -g -std=c++11 -DSIL
LDFLAGS= -g
LDLIBS=

OBJDIR = build

SRCS=Simulator.cc SIL.cc Harness.cc common.cc Output.cc Sensors.cc code_stage1.cc code_stage2.cc
OBJS=$(addprefix $(OBJDIR)/, $(subst .cc,.o,$(SRCS)))

all: builddir sil

sil: $(OBJS)
	$(CXX) $(LDFLAGS) -o sil $(OBJS) $(LDLIBS)

$(OBJDIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) -c -o $@ $<

builddir:
	mkdir -p $(OBJDIR)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) -r $(OBJDIR)
	$(RM) sil
