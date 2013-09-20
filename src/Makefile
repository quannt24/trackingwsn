#
# OMNeT++/OMNEST Makefile for TrackingWSN
#
# This file was generated with the command:
#  opp_makemake -f --deep
#

# Name of target to be created (-o option)
TARGET = TrackingWSN$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Iapplications \
    -Ienergy \
    -Ilinklayer \
    -Imobility \
    -Inetworklayer \
    -Inodes \
    -Iother \
    -Iutil

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/applications/appbasestation.o \
    $O/applications/appsensor.o \
    $O/energy/energy.o \
    $O/linklayer/link802154.o \
    $O/mobility/mobility.o \
    $O/networklayer/netemrp.o \
    $O/other/asg.o \
    $O/other/ass.o \
    $O/util/worldutil.o \
    $O/util/channelutil.o \
    $O/linklayer/packet802154_m.o \
    $O/linklayer/frame802154_m.o \
    $O/networklayer/packetemrp_m.o \
    $O/networklayer/messagecr_m.o \
    $O/other/sensereq_m.o \
    $O/other/sensedsignal_m.o

# Message files
MSGFILES = \
    linklayer/packet802154.msg \
    linklayer/frame802154.msg \
    networklayer/packetemrp.msg \
    networklayer/messagecr.msg \
    other/sensereq.msg \
    other/sensedsignal.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f TrackingWSN TrackingWSN.exe libTrackingWSN.so libTrackingWSN.a libTrackingWSN.dll libTrackingWSN.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f applications/*_m.cc applications/*_m.h
	$(Q)-rm -f energy/*_m.cc energy/*_m.h
	$(Q)-rm -f linklayer/*_m.cc linklayer/*_m.h
	$(Q)-rm -f mobility/*_m.cc mobility/*_m.h
	$(Q)-rm -f networklayer/*_m.cc networklayer/*_m.h
	$(Q)-rm -f nodes/*_m.cc nodes/*_m.h
	$(Q)-rm -f other/*_m.cc other/*_m.h
	$(Q)-rm -f util/*_m.cc util/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc applications/*.cc energy/*.cc linklayer/*.cc mobility/*.cc networklayer/*.cc nodes/*.cc other/*.cc util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/applications/appbasestation.o: applications/appbasestation.cc \
	applications/appbasestation.h
$O/applications/appsensor.o: applications/appsensor.cc \
	applications/appsensor.h \
	msgkind.h \
	networklayer/messagecr_m.h
$O/energy/energy.o: energy/energy.cc \
	energy/energy.h
$O/linklayer/frame802154_m.o: linklayer/frame802154_m.cc \
	linklayer/frame802154_m.h
$O/linklayer/link802154.o: linklayer/link802154.cc \
	energy/energy.h \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	util/channelutil.h \
	wsnexception.h
$O/linklayer/packet802154_m.o: linklayer/packet802154_m.cc \
	linklayer/packet802154_m.h
$O/mobility/mobility.o: mobility/mobility.cc \
	mobility/mobility.h
$O/networklayer/messagecr_m.o: networklayer/messagecr_m.cc \
	networklayer/messagecr_m.h
$O/networklayer/netemrp.o: networklayer/netemrp.cc \
	energy/energy.h \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	mobility/mobility.h \
	msgkind.h \
	networklayer/messagecr_m.h \
	networklayer/netemrp.h \
	networklayer/packetemrp_m.h \
	util/worldutil.h
$O/networklayer/packetemrp_m.o: networklayer/packetemrp_m.cc \
	linklayer/packet802154_m.h \
	networklayer/packetemrp_m.h
$O/other/asg.o: other/asg.cc \
	msgkind.h \
	other/asg.h \
	other/ass.h \
	other/sensedsignal_m.h \
	other/sensereq_m.h
$O/other/ass.o: other/ass.cc \
	mobility/mobility.h \
	msgkind.h \
	other/ass.h \
	other/sensedsignal_m.h \
	other/sensereq_m.h \
	util/worldutil.h
$O/other/sensedsignal_m.o: other/sensedsignal_m.cc \
	other/sensedsignal_m.h
$O/other/sensereq_m.o: other/sensereq_m.cc \
	other/ass.h \
	other/sensedsignal_m.h \
	other/sensereq_m.h
$O/util/channelutil.o: util/channelutil.cc \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	mobility/mobility.h \
	util/channelutil.h \
	util/worldutil.h
$O/util/worldutil.o: util/worldutil.cc \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	mobility/mobility.h \
	networklayer/messagecr_m.h \
	networklayer/netemrp.h \
	networklayer/packetemrp_m.h \
	util/worldutil.h

