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
    -Iapplications/estimator \
    -Ienergy \
    -Ilinklayer \
    -Imobility \
    -Inetworklayer \
    -Inodes \
    -Isensors \
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
    $O/applications/targetposvectorset.o \
    $O/applications/appbasestation.o \
    $O/applications/appsensor.o \
    $O/applications/meacoll.o \
    $O/applications/app.o \
    $O/applications/estimator/targetpos.o \
    $O/applications/estimator/lateration.o \
    $O/applications/estimator/estimator.o \
    $O/energy/energy.o \
    $O/linklayer/link802154.o \
    $O/mobility/mobility.o \
    $O/networklayer/net.o \
    $O/networklayer/netemrp.o \
    $O/sensors/asg.o \
    $O/sensors/measurement.o \
    $O/sensors/ass.o \
    $O/util/matrix.o \
    $O/util/statcollector.o \
    $O/util/worldutil.o \
    $O/util/gaussiannoise.o \
    $O/util/channelutil.o \
    $O/applications/messagetracking_m.o \
    $O/linklayer/packet802154_m.o \
    $O/linklayer/frame802154_m.o \
    $O/networklayer/packetemrp_m.o \
    $O/networklayer/packetcr_m.o \
    $O/networklayer/messagecr_m.o \
    $O/sensors/sensedresult_m.o \
    $O/sensors/sensereq_m.o \
    $O/sensors/sensedsignal_m.o

# Message files
MSGFILES = \
    applications/messagetracking.msg \
    linklayer/packet802154.msg \
    linklayer/frame802154.msg \
    networklayer/packetemrp.msg \
    networklayer/packetcr.msg \
    networklayer/messagecr.msg \
    sensors/sensedresult.msg \
    sensors/sensereq.msg \
    sensors/sensedsignal.msg

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
	$(Q)-rm -f applications/estimator/*_m.cc applications/estimator/*_m.h
	$(Q)-rm -f energy/*_m.cc energy/*_m.h
	$(Q)-rm -f linklayer/*_m.cc linklayer/*_m.h
	$(Q)-rm -f mobility/*_m.cc mobility/*_m.h
	$(Q)-rm -f networklayer/*_m.cc networklayer/*_m.h
	$(Q)-rm -f nodes/*_m.cc nodes/*_m.h
	$(Q)-rm -f sensors/*_m.cc sensors/*_m.h
	$(Q)-rm -f util/*_m.cc util/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc applications/*.cc applications/estimator/*.cc energy/*.cc linklayer/*.cc mobility/*.cc networklayer/*.cc nodes/*.cc sensors/*.cc util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/applications/app.o: applications/app.cc \
	applications/app.h
$O/applications/appbasestation.o: applications/appbasestation.cc \
	applications/app.h \
	applications/appbasestation.h \
	applications/estimator/targetpos.h \
	applications/messagetracking_m.h \
	applications/targetposvectorset.h \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	networklayer/messagecr_m.h \
	sensors/measurement.h
$O/applications/appsensor.o: applications/appsensor.cc \
	applications/app.h \
	applications/appsensor.h \
	applications/estimator/estimator.h \
	applications/estimator/targetpos.h \
	applications/meacoll.h \
	applications/messagetracking_m.h \
	energy/energy.h \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	mobility/mobility.h \
	msgkind.h \
	networklayer/messagecr_m.h \
	sensors/measurement.h \
	sensors/sensedresult_m.h \
	util/statcollector.h \
	util/worldutil.h
$O/applications/meacoll.o: applications/meacoll.cc \
	applications/meacoll.h \
	sensors/measurement.h
$O/applications/messagetracking_m.o: applications/messagetracking_m.cc \
	applications/estimator/targetpos.h \
	applications/messagetracking_m.h \
	networklayer/messagecr_m.h \
	sensors/measurement.h
$O/applications/targetposvectorset.o: applications/targetposvectorset.cc \
	applications/targetposvectorset.h
$O/applications/estimator/estimator.o: applications/estimator/estimator.cc \
	applications/estimator/estimator.h \
	applications/estimator/targetpos.h \
	sensors/measurement.h
$O/applications/estimator/lateration.o: applications/estimator/lateration.cc \
	applications/estimator/estimator.h \
	applications/estimator/lateration.h \
	applications/estimator/targetpos.h \
	sensors/measurement.h \
	util/matrix.h
$O/applications/estimator/targetpos.o: applications/estimator/targetpos.cc \
	applications/estimator/targetpos.h
$O/energy/energy.o: energy/energy.cc \
	applications/app.h \
	energy/energy.h
$O/linklayer/frame802154_m.o: linklayer/frame802154_m.cc \
	linklayer/frame802154_m.h
$O/linklayer/link802154.o: linklayer/link802154.cc \
	applications/app.h \
	energy/energy.h \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	util/channelutil.h \
	util/statcollector.h \
	wsnexception.h
$O/linklayer/packet802154_m.o: linklayer/packet802154_m.cc \
	linklayer/packet802154_m.h
$O/mobility/mobility.o: mobility/mobility.cc \
	mobility/mobility.h
$O/networklayer/messagecr_m.o: networklayer/messagecr_m.cc \
	networklayer/messagecr_m.h
$O/networklayer/net.o: networklayer/net.cc \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	mobility/mobility.h \
	networklayer/net.h \
	util/worldutil.h
$O/networklayer/netemrp.o: networklayer/netemrp.cc \
	applications/app.h \
	applications/estimator/targetpos.h \
	applications/messagetracking_m.h \
	energy/energy.h \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	mobility/mobility.h \
	msgkind.h \
	networklayer/messagecr_m.h \
	networklayer/net.h \
	networklayer/netemrp.h \
	networklayer/packetcr_m.h \
	networklayer/packetemrp_m.h \
	sensors/measurement.h \
	util/worldutil.h
$O/networklayer/packetcr_m.o: networklayer/packetcr_m.cc \
	linklayer/packet802154_m.h \
	networklayer/packetcr_m.h
$O/networklayer/packetemrp_m.o: networklayer/packetemrp_m.cc \
	linklayer/packet802154_m.h \
	networklayer/packetcr_m.h \
	networklayer/packetemrp_m.h
$O/sensors/asg.o: sensors/asg.cc \
	mobility/mobility.h \
	msgkind.h \
	sensors/asg.h \
	sensors/ass.h \
	sensors/measurement.h \
	sensors/sensedsignal_m.h \
	sensors/sensereq_m.h \
	util/gaussiannoise.h \
	util/worldutil.h
$O/sensors/ass.o: sensors/ass.cc \
	energy/energy.h \
	mobility/mobility.h \
	msgkind.h \
	sensors/ass.h \
	sensors/measurement.h \
	sensors/sensedresult_m.h \
	sensors/sensedsignal_m.h \
	sensors/sensereq_m.h \
	util/gaussiannoise.h \
	util/worldutil.h
$O/sensors/measurement.o: sensors/measurement.cc \
	sensors/measurement.h
$O/sensors/sensedresult_m.o: sensors/sensedresult_m.cc \
	sensors/measurement.h \
	sensors/sensedresult_m.h
$O/sensors/sensedsignal_m.o: sensors/sensedsignal_m.cc \
	sensors/sensedsignal_m.h
$O/sensors/sensereq_m.o: sensors/sensereq_m.cc \
	mobility/mobility.h \
	sensors/ass.h \
	sensors/measurement.h \
	sensors/sensedsignal_m.h \
	sensors/sensereq_m.h \
	util/gaussiannoise.h
$O/util/channelutil.o: util/channelutil.cc \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	mobility/mobility.h \
	util/channelutil.h \
	util/worldutil.h
$O/util/gaussiannoise.o: util/gaussiannoise.cc \
	util/gaussiannoise.h
$O/util/matrix.o: util/matrix.cc \
	util/matrix.h
$O/util/statcollector.o: util/statcollector.cc \
	energy/energy.h \
	mobility/mobility.h \
	util/statcollector.h
$O/util/worldutil.o: util/worldutil.cc \
	linklayer/frame802154_m.h \
	linklayer/link802154.h \
	linklayer/packet802154_m.h \
	mobility/mobility.h \
	networklayer/messagecr_m.h \
	networklayer/net.h \
	networklayer/netemrp.h \
	networklayer/packetcr_m.h \
	networklayer/packetemrp_m.h \
	util/worldutil.h

