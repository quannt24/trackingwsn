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
    -Iapplications/app \
    -Iapplications/tracking \
    -Iapplications/tracking/appbasestation \
    -Iapplications/tracking/appsensor \
    -Iapplications/tracking/estimator \
    -Ienergy \
    -Ientities \
    -Ilinklayer \
    -Ilinklayer/link802154 \
    -Ilinklayer/linkxtmac \
    -Imobility \
    -Inetworklayer \
    -Inetworklayer/arpees \
    -Inetworklayer/emrp \
    -Inetworklayer/net \
    -Inetworks \
    -Isensors \
    -Isensors/asg \
    -Isensors/ass \
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
    $O/applications/app/app.o \
    $O/applications/tracking/targetposvectorset.o \
    $O/applications/tracking/meacoll.o \
    $O/applications/tracking/appbasestation/appbasestation.o \
    $O/applications/tracking/appsensor/appsensor.o \
    $O/applications/tracking/estimator/targetpos.o \
    $O/applications/tracking/estimator/lateration.o \
    $O/applications/tracking/estimator/estimator.o \
    $O/energy/energy.o \
    $O/linklayer/link802154/link802154.o \
    $O/linklayer/linkxtmac/linkxtmac.o \
    $O/mobility/mobility.o \
    $O/networklayer/arpees/netarpees.o \
    $O/networklayer/emrp/netemrp.o \
    $O/networklayer/net/net.o \
    $O/sensors/asg/asg.o \
    $O/sensors/ass/measurement.o \
    $O/sensors/ass/ass.o \
    $O/util/matrix.o \
    $O/util/statcollector.o \
    $O/util/worldutil.o \
    $O/util/gaussiannoise.o \
    $O/util/channelutil.o \
    $O/applications/tracking/messagetracking_m.o \
    $O/linklayer/link802154/packet802154_m.o \
    $O/linklayer/link802154/frame802154_m.o \
    $O/networklayer/packetcr_m.o \
    $O/networklayer/messagecr_m.o \
    $O/networklayer/arpees/packetarpees_m.o \
    $O/networklayer/emrp/packetemrp_m.o \
    $O/sensors/sensereq_m.o \
    $O/sensors/sensedsignal_m.o \
    $O/sensors/ass/sensedresult_m.o

# Message files
MSGFILES = \
    applications/tracking/messagetracking.msg \
    linklayer/link802154/packet802154.msg \
    linklayer/link802154/frame802154.msg \
    networklayer/packetcr.msg \
    networklayer/messagecr.msg \
    networklayer/arpees/packetarpees.msg \
    networklayer/emrp/packetemrp.msg \
    sensors/sensereq.msg \
    sensors/sensedsignal.msg \
    sensors/ass/sensedresult.msg

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
	$(Q)-rm -f applications/app/*_m.cc applications/app/*_m.h
	$(Q)-rm -f applications/tracking/*_m.cc applications/tracking/*_m.h
	$(Q)-rm -f applications/tracking/appbasestation/*_m.cc applications/tracking/appbasestation/*_m.h
	$(Q)-rm -f applications/tracking/appsensor/*_m.cc applications/tracking/appsensor/*_m.h
	$(Q)-rm -f applications/tracking/estimator/*_m.cc applications/tracking/estimator/*_m.h
	$(Q)-rm -f energy/*_m.cc energy/*_m.h
	$(Q)-rm -f entities/*_m.cc entities/*_m.h
	$(Q)-rm -f linklayer/*_m.cc linklayer/*_m.h
	$(Q)-rm -f linklayer/link802154/*_m.cc linklayer/link802154/*_m.h
	$(Q)-rm -f linklayer/linkxtmac/*_m.cc linklayer/linkxtmac/*_m.h
	$(Q)-rm -f mobility/*_m.cc mobility/*_m.h
	$(Q)-rm -f networklayer/*_m.cc networklayer/*_m.h
	$(Q)-rm -f networklayer/arpees/*_m.cc networklayer/arpees/*_m.h
	$(Q)-rm -f networklayer/emrp/*_m.cc networklayer/emrp/*_m.h
	$(Q)-rm -f networklayer/net/*_m.cc networklayer/net/*_m.h
	$(Q)-rm -f networks/*_m.cc networks/*_m.h
	$(Q)-rm -f sensors/*_m.cc sensors/*_m.h
	$(Q)-rm -f sensors/asg/*_m.cc sensors/asg/*_m.h
	$(Q)-rm -f sensors/ass/*_m.cc sensors/ass/*_m.h
	$(Q)-rm -f util/*_m.cc util/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc applications/*.cc applications/app/*.cc applications/tracking/*.cc applications/tracking/appbasestation/*.cc applications/tracking/appsensor/*.cc applications/tracking/estimator/*.cc energy/*.cc entities/*.cc linklayer/*.cc linklayer/link802154/*.cc linklayer/linkxtmac/*.cc mobility/*.cc networklayer/*.cc networklayer/arpees/*.cc networklayer/emrp/*.cc networklayer/net/*.cc networks/*.cc sensors/*.cc sensors/asg/*.cc sensors/ass/*.cc util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/applications/app/app.o: applications/app/app.cc \
	applications/app/app.h
$O/applications/tracking/meacoll.o: applications/tracking/meacoll.cc \
	applications/tracking/meacoll.h \
	sensors/ass/measurement.h
$O/applications/tracking/messagetracking_m.o: applications/tracking/messagetracking_m.cc \
	applications/tracking/estimator/targetpos.h \
	applications/tracking/messagetracking_m.h \
	networklayer/messagecr_m.h \
	sensors/ass/measurement.h
$O/applications/tracking/targetposvectorset.o: applications/tracking/targetposvectorset.cc \
	applications/tracking/targetposvectorset.h
$O/applications/tracking/appbasestation/appbasestation.o: applications/tracking/appbasestation/appbasestation.cc \
	applications/app/app.h \
	applications/tracking/appbasestation/appbasestation.h \
	applications/tracking/estimator/targetpos.h \
	applications/tracking/messagetracking_m.h \
	applications/tracking/targetposvectorset.h \
	linklayer/link.h \
	linklayer/link802154/frame802154_m.h \
	linklayer/link802154/link802154.h \
	linklayer/link802154/packet802154_m.h \
	networklayer/messagecr_m.h \
	sensors/ass/measurement.h \
	util/statcollector.h
$O/applications/tracking/appsensor/appsensor.o: applications/tracking/appsensor/appsensor.cc \
	applications/app/app.h \
	applications/tracking/appsensor/appsensor.h \
	applications/tracking/estimator/estimator.h \
	applications/tracking/estimator/targetpos.h \
	applications/tracking/meacoll.h \
	applications/tracking/messagetracking_m.h \
	energy/energy.h \
	linklayer/link.h \
	linklayer/link802154/frame802154_m.h \
	linklayer/link802154/link802154.h \
	linklayer/link802154/packet802154_m.h \
	mobility/mobility.h \
	msgkind.h \
	networklayer/messagecr_m.h \
	sensors/ass/measurement.h \
	sensors/ass/sensedresult_m.h \
	util/statcollector.h \
	util/worldutil.h
$O/applications/tracking/estimator/estimator.o: applications/tracking/estimator/estimator.cc \
	applications/tracking/estimator/estimator.h \
	applications/tracking/estimator/targetpos.h \
	sensors/ass/measurement.h
$O/applications/tracking/estimator/lateration.o: applications/tracking/estimator/lateration.cc \
	applications/tracking/estimator/estimator.h \
	applications/tracking/estimator/lateration.h \
	applications/tracking/estimator/targetpos.h \
	sensors/ass/measurement.h \
	util/matrix.h
$O/applications/tracking/estimator/targetpos.o: applications/tracking/estimator/targetpos.cc \
	applications/tracking/estimator/targetpos.h
$O/energy/energy.o: energy/energy.cc \
	applications/app/app.h \
	energy/energy.h
$O/linklayer/link802154/frame802154_m.o: linklayer/link802154/frame802154_m.cc \
	linklayer/link802154/frame802154_m.h
$O/linklayer/link802154/link802154.o: linklayer/link802154/link802154.cc \
	applications/app/app.h \
	energy/energy.h \
	linklayer/link.h \
	linklayer/link802154/frame802154_m.h \
	linklayer/link802154/link802154.h \
	linklayer/link802154/packet802154_m.h \
	networklayer/packetcr_m.h \
	util/channelutil.h \
	util/statcollector.h \
	wsnexception.h
$O/linklayer/link802154/packet802154_m.o: linklayer/link802154/packet802154_m.cc \
	linklayer/link802154/packet802154_m.h
$O/linklayer/linkxtmac/linkxtmac.o: linklayer/linkxtmac/linkxtmac.cc \
	applications/app/app.h \
	linklayer/link.h \
	linklayer/link802154/frame802154_m.h \
	linklayer/link802154/link802154.h \
	linklayer/link802154/packet802154_m.h \
	linklayer/linkxtmac/linkxtmac.h \
	networklayer/packetcr_m.h \
	util/channelutil.h \
	util/statcollector.h
$O/mobility/mobility.o: mobility/mobility.cc \
	mobility/mobility.h
$O/networklayer/messagecr_m.o: networklayer/messagecr_m.cc \
	networklayer/messagecr_m.h
$O/networklayer/packetcr_m.o: networklayer/packetcr_m.cc \
	linklayer/link802154/packet802154_m.h \
	networklayer/packetcr_m.h
$O/networklayer/arpees/netarpees.o: networklayer/arpees/netarpees.cc \
	energy/energy.h \
	linklayer/link802154/packet802154_m.h \
	mobility/mobility.h \
	msgkind.h \
	networklayer/arpees/netarpees.h \
	networklayer/arpees/packetarpees_m.h \
	networklayer/messagecr_m.h \
	networklayer/net/net.h \
	networklayer/packetcr_m.h \
	util/statcollector.h \
	util/worldutil.h
$O/networklayer/arpees/packetarpees_m.o: networklayer/arpees/packetarpees_m.cc \
	linklayer/link802154/packet802154_m.h \
	networklayer/arpees/packetarpees_m.h \
	networklayer/packetcr_m.h
$O/networklayer/emrp/netemrp.o: networklayer/emrp/netemrp.cc \
	applications/tracking/estimator/targetpos.h \
	applications/tracking/messagetracking_m.h \
	energy/energy.h \
	linklayer/link.h \
	linklayer/link802154/packet802154_m.h \
	mobility/mobility.h \
	msgkind.h \
	networklayer/emrp/netemrp.h \
	networklayer/emrp/packetemrp_m.h \
	networklayer/messagecr_m.h \
	networklayer/net/net.h \
	networklayer/packetcr_m.h \
	sensors/ass/measurement.h \
	util/statcollector.h \
	util/worldutil.h
$O/networklayer/emrp/packetemrp_m.o: networklayer/emrp/packetemrp_m.cc \
	linklayer/link802154/packet802154_m.h \
	networklayer/emrp/packetemrp_m.h \
	networklayer/packetcr_m.h
$O/networklayer/net/net.o: networklayer/net/net.cc \
	applications/app/app.h \
	linklayer/link.h \
	linklayer/link802154/frame802154_m.h \
	linklayer/link802154/link802154.h \
	linklayer/link802154/packet802154_m.h \
	mobility/mobility.h \
	networklayer/net/net.h \
	util/worldutil.h
$O/sensors/sensedsignal_m.o: sensors/sensedsignal_m.cc \
	sensors/sensedsignal_m.h
$O/sensors/sensereq_m.o: sensors/sensereq_m.cc \
	mobility/mobility.h \
	sensors/ass/ass.h \
	sensors/ass/measurement.h \
	sensors/sensedsignal_m.h \
	sensors/sensereq_m.h \
	util/gaussiannoise.h
$O/sensors/asg/asg.o: sensors/asg/asg.cc \
	mobility/mobility.h \
	msgkind.h \
	sensors/asg/asg.h \
	sensors/ass/ass.h \
	sensors/ass/measurement.h \
	sensors/sensedsignal_m.h \
	sensors/sensereq_m.h \
	util/gaussiannoise.h \
	util/worldutil.h
$O/sensors/ass/ass.o: sensors/ass/ass.cc \
	energy/energy.h \
	mobility/mobility.h \
	msgkind.h \
	sensors/ass/ass.h \
	sensors/ass/measurement.h \
	sensors/ass/sensedresult_m.h \
	sensors/sensedsignal_m.h \
	sensors/sensereq_m.h \
	util/gaussiannoise.h \
	util/worldutil.h
$O/sensors/ass/measurement.o: sensors/ass/measurement.cc \
	sensors/ass/measurement.h
$O/sensors/ass/sensedresult_m.o: sensors/ass/sensedresult_m.cc \
	sensors/ass/measurement.h \
	sensors/ass/sensedresult_m.h
$O/util/channelutil.o: util/channelutil.cc \
	linklayer/link.h \
	linklayer/link802154/frame802154_m.h \
	linklayer/link802154/link802154.h \
	linklayer/link802154/packet802154_m.h \
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
	linklayer/link.h \
	linklayer/link802154/frame802154_m.h \
	linklayer/link802154/link802154.h \
	linklayer/link802154/packet802154_m.h \
	mobility/mobility.h \
	networklayer/emrp/netemrp.h \
	networklayer/emrp/packetemrp_m.h \
	networklayer/messagecr_m.h \
	networklayer/net/net.h \
	networklayer/packetcr_m.h \
	util/worldutil.h

