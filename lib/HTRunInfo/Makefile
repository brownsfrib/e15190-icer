CFLAGS    :=`root-config --cflags --libs`

DllSuf    := so
SrcSuf    := cxx
ObjSuf    := o
ObjDir    := ../obj/
LibDir    := ../lib/

INCLUDES  := -I./

CXXFLAGS  += $(INCLUDES) -std=c++11 -fPIC -O3 -Wall -Wpedantic

HTRUNINFO_LIB := libHTRunInfo.$(DllSuf)
SRCS = $(wildcard *.$(SrcSuf))
OBJS = $(patsubst %.$(SrcSuf), %.$(ObjSuf), $(SRCS))
OBJS := $(addprefix $(ObjDir), $(OBJS))
DEPS := $(_OBJS:.$(ObjSuf)=.$(DepSuf))

$(ObjDir)%.$(ObjSuf): %.$(SrcSuf) $(DEPS)
	@$(CXX) $(CXXFLAGS) -c -o $@ $< $(SYSLIB) $(CFLAGS)
	@echo "$(CXX) $(CXXFLAGS) -c -o $@ $< $(SYSLIB) $(CFLAGS)"

all: $(LibDir)$(HTRUNINFO_LIB)

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

$(LibDir)$(HTRUNINFO_LIB): $(OBJS)
	@$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(CFLAGS)
	@echo "$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(CFLAGS)"

install:

.PHONY: distclean
distclean:
	@rm -f $(LibDir)$(HTRUNINFO_LIB)
	@echo "rm -f $(LibDir)$(HTRUNINFO_LIB)"

.PHONY: clean
clean:
	@$(RM) -f $(LIB) $(OBJS)
	@echo "$(RM) -f $(LIB) $(OBJS)"
	@$(RM) -f *.expand

.PHONY: lint
lint:
	$(LINT) $(INC_SRCH_PATH) $(SRCS)
