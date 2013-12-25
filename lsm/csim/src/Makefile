#
# CSIM Version
#
VERSION_STRING = "1.1"

#
# Build tools
#
MEX    = mex
REGGEN = ../../develop/reggen/bin/reggen

#
#  Default compile and link options
#
#COMPILEOPTS = -O -cxx
#LINKOPTS    = -O -cxx

#
# Options for Matlab 6.5, GCC-3.4, GCC-4.0.2 
#
# You have to modify the CXXFLAGS in the file csim_mexopts.sh in the glnx86 - section
# Insert the correct path to the libstdc++.a of your gcc version (default /usr/local/lib or /usr/lib )
#
COMPILEOPTS = -O -cxx -f csim_mexopts.sh
LINKOPTS    = -O -cxx -f csim_mexopts.sh

#
# Options for Matlab 7.2, GCC-3.4, GCC-4.0.2 
#
# You have to modify the CXXFLAGS in the file csim_mexopts.sh in the glnx86 - section
# Insert the correct path to the libstdc++.a of your gcc version (default /usr/local/lib or /usr/lib )
#
#COMPILEOPTS = -O -cxx -f csim_mex72_opts.sh
#LINKOPTS    = -O -cxx -f csim_mex72_opts.sh

#
# here are all the objects
#
OBJECTS = csimclass.o \
	csimmex.o \
	randgen.o \
	advanceable.o \
	analogteacher.o \
	cbhhouneuron.o \
	cbhhouineuron.o \
	staticspikingsynapse.o \
	dynamicspikingsynapse.o \
	dynamicglutamatesynapse.o \
	dynamicglutamatesynapsesynchan.o \
	staticglutamatesynapse.o \
	staticglutamatesynapsesynchan.o \
	glutamatesynapse.o \
	glutamatesynapsesynchan.o \
	staticstdpsynapse.o \
	dynamicstdpsynapse.o \
	stdpsynapse.o \
	analogneuron.o \
	analogsynapse.o \
	staticanalogsynapse.o \
	lifneuron.o \
	lifneuronsynchan.o \
	network.o \
	neuron.o \
	spikingneuron.o \
	spikinginputneuron.o \
	spikingsynapse.o \
	spikingteacher.o \
	synapse.o \
	teacher.o \
	recorder.o \
	mexrecorder.o \
	csimerror.o \
	analoginputneuron.o \
	sigmoidalneuron.o \
	linearneuron.o \
	cbneuron.o \
	ionchannel.o \
	viongate.o \
	conciongate.o \
	iongate.o \
	activechannel.o \
        ahp_channel.o \
	synapticchannel.o \
	mexnetwork.o \
	forceable.o \
	hh_squid_channels.o \
	hh_neuron.o \
	extinputneuron.o \
	extoutsigmoidalneuron.o \
        extoutlinearneuron.o \
        generic_ion_gates.o \
        specific_ion_channels.o \
	specific_neurons.o \
	specificouneurons.o \
	activecachannel.o \
	cbneuronst.o \
	extoutlifneuron.o \
	csiminputclass.o \
	membranepatch.o \
	membranepatchsimple.o \
	ionbuffer.o \
	ifbneuron.o \
	izhi_neuron.o \
	lifburstneuron.o \
	filterfunction.o \
	analogfilter.o \
	triangularanalogfilter.o \
	gaussiananalogfilter.o \
	useranalogfilter.o \
	spikefilter.o \
	expspikefilter.o \
	alphaspikefilter.o \
	countspikefilter.o \
	preprocessor.o \
	pcapreprocessor.o \
	linearpreprocessor.o \
	discretizationpreprocessor.o \
	mean_std_preprocessor.o \
	algorithm.o \
	linear_regression.o \
	linear_classification.o \
	physicalmodel.o \
	armmodel.o \
	readout.o \
	analogfeedbackneuron.o \
	cbstouneuron.o \
	staticanalogcbsynapse.o \
	staticspikingcbsynapse.o \
	dynamicspikingcbsynapse.o \
        TraubsHHNeuron.o

#
# the default rule
#
default: version.i filepermissions csim.mexglx install.m

version.i: Makefile Makefile.win
	echo #define _VERSION_STRING_ $(VERSION_STRING) > version.i

filepermissions:
	chmod 755 makedependencies

#
# the *.i files
#
classlist.i switch.i registerclasses.i: *.h
	$(REGGEN) RegGenFile
	@rm -rf def html

field-files:
	$(REGGEN) RegGenFile
	@rm -rf def html

#
# the rule how to make an *.o file from an *.cpp file
#
$(OBJECTS): %.o: %.cpp
	$(MEX) $(COMPILEOPTS) -c $<

#
# how to make the mex file
#
csim.mexglx: $(OBJECTS) csim.cpp
	$(MEX) -D_VERSION_STRING_\#\\\"${versionstring}\\\" $(LINKOPTS) csim.cpp $(OBJECTS);

#
# some default targets
#
clean:
	rm -f *.o csim.mexglx *.i *.obj objectlist.txt

mrproper:
	rm -f *.o csim.mexglx *.i *.obj install.m fields-* objectlist.txt

distclean:
	rm -f *.o *.obj fields-*

build: clean filepermissions depend csim.mexglx install.m

#
# make the dependencies
#
depend:
	./makedependencies

#
# make a command to compile the whole stuff from within matlab (for windows!)
#
install.m: Makefile install.tmpl objectlist.txt
	@(sed -e s#_OBJECTS_#"`cat objectlist.txt`"# install.tmpl > install.m;)

objectlist.txt: Makefile
	@echo ${OBJECTS} | tr ' ' '\n' |  sed -e s/\\.o/\\.cpp/ | tr '\n' ' ' > objectlist.txt

# DO NOT DELETE THIS LINE -- make depend depends on it.
# the following dependencies are generated automatically by ./makedependencies

activecachannel.o: activecachannel.cpp

activechannel.o: activechannel.cpp activechannel.h ionchannel.h \
  globaldefinitions.h advanceable.h csimclass.h membranepatchsimple.h \
  spikingneuron.h neuron.h forceable.h csimlist.h synapsetarget.h \
  spikingsynapse.h synapse.h iongate.h cbneuron.h membranepatch.h \
  ionbuffer.h csimerror.h analoginputneuron.h analogneuron.h \
  analogsynapse.h csiminputclass.h hh_squid_channels.h viongate.h

advanceable.o: advanceable.cpp advanceable.h csimclass.h \
  globaldefinitions.h

ahp_channel.o: ahp_channel.cpp ahp_channel.h ionchannel.h \
  globaldefinitions.h advanceable.h csimclass.h membranepatchsimple.h \
  spikingneuron.h neuron.h forceable.h csimlist.h synapsetarget.h \
  spikingsynapse.h synapse.h cbneuron.h membranepatch.h ionbuffer.h \
  analogneuron.h analogsynapse.h csimerror.h

algorithm.o: algorithm.cpp algorithm.h csimerror.h csimclass.h \
  globaldefinitions.h advanceable.h

alphaspikefilter.o: alphaspikefilter.cpp alphaspikefilter.h spikefilter.h \
  filterfunction.h csimerror.h advanceable.h csimclass.h \
  globaldefinitions.h spikingneuron.h neuron.h forceable.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h

analogfeedbackneuron.o: analogfeedbackneuron.cpp analogfeedbackneuron.h \
  analoginputneuron.h analogneuron.h neuron.h forceable.h advanceable.h \
  csimclass.h globaldefinitions.h csimlist.h synapsetarget.h \
  analogsynapse.h synapse.h csiminputclass.h csimerror.h

analogfilter.o: analogfilter.cpp analogfilter.h filterfunction.h \
  csimerror.h advanceable.h csimclass.h globaldefinitions.h

analoginputneuron.o: analoginputneuron.cpp analoginputneuron.h \
  analogneuron.h neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h analogsynapse.h \
  synapse.h csiminputclass.h csimerror.h

analogneuron.o: analogneuron.cpp analogneuron.h neuron.h forceable.h \
  advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h analogsynapse.h synapse.h csimerror.h

analogsynapse.o: analogsynapse.cpp analogsynapse.h synapse.h \
  advanceable.h csimclass.h globaldefinitions.h neuron.h forceable.h \
  csimlist.h synapsetarget.h

analogteacher.o: analogteacher.cpp analogteacher.h teacher.h \
  advanceable.h csimclass.h globaldefinitions.h csimlist.h analogneuron.h \
  neuron.h forceable.h synapsetarget.h analogsynapse.h synapse.h \
  csiminputclass.h csimerror.h

armmodel.o: armmodel.cpp armmodel.h physicalmodel.h csimerror.h \
  csimclass.h globaldefinitions.h advanceable.h modelinput.h \
  analogsynapse.h synapse.h neuron.h forceable.h csimlist.h \
  synapsetarget.h readout.h analogfilter.h filterfunction.h spikefilter.h \
  spikingneuron.h spikingsynapse.h preprocessor.h algorithm.h \
  /usr/local/matlab/extern/include/mat.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h

cbhhouineuron.o: cbhhouineuron.cpp randgen.h cbhhouineuron.h cbneuron.h \
  spikingneuron.h neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h ionchannel.h membranepatchsimple.h membranepatch.h \
  ionbuffer.h specific_ion_channels.h viongate.h iongate.h conciongate.h \
  activechannel.h activecachannel.h csimerror.h

cbhhouneuron.o: cbhhouneuron.cpp randgen.h cbhhouneuron.h cbneuron.h \
  spikingneuron.h neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h ionchannel.h membranepatchsimple.h membranepatch.h \
  ionbuffer.h specific_ion_channels.h viongate.h iongate.h conciongate.h \
  activechannel.h activecachannel.h csimerror.h

cbneuron.o: cbneuron.cpp randgen.h cbneuron.h spikingneuron.h neuron.h \
  forceable.h advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h ionchannel.h \
  membranepatchsimple.h membranepatch.h ionbuffer.h

cbneuronst.o: cbneuronst.cpp randgen.h cbneuronst.h cbneuron.h \
  spikingneuron.h neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h ionchannel.h membranepatchsimple.h membranepatch.h \
  ionbuffer.h

cbstouneuron.o: cbstouneuron.cpp randgen.h cbstouneuron.h cbneuronst.h \
  cbneuron.h spikingneuron.h neuron.h forceable.h advanceable.h \
  csimclass.h globaldefinitions.h csimlist.h synapsetarget.h \
  spikingsynapse.h synapse.h ionchannel.h membranepatchsimple.h \
  membranepatch.h ionbuffer.h

conciongate.o: conciongate.cpp conciongate.h advanceable.h csimclass.h \
  globaldefinitions.h membranepatch.h spikingneuron.h neuron.h \
  forceable.h csimlist.h synapsetarget.h spikingsynapse.h synapse.h \
  ionchannel.h membranepatchsimple.h ionbuffer.h iongate.h csimerror.h

countspikefilter.o: countspikefilter.cpp countspikefilter.h spikefilter.h \
  filterfunction.h csimerror.h advanceable.h csimclass.h \
  globaldefinitions.h spikingneuron.h neuron.h forceable.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h

csimclass.o: csimclass.cpp csimclass.h globaldefinitions.h csimerror.h \
  classlist.i registerclasses.i

csim.o: csim.cpp csimmex.h /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h globaldefinitions.h \
  csimerror.h

csimerror.o: csimerror.cpp csimerror.h

csiminputclass.o: csiminputclass.cpp csiminputclass.h globaldefinitions.h \
  csimlist.h csimerror.h

csimlist.o: csimlist.cpp csimlist.h

csimmex.o: csimmex.cpp csimmex.h /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h globaldefinitions.h \
  classlist.i csimerror.h mexnetwork.h network.h csimlist.h \
  spikingsynapse.h synapse.h advanceable.h csimclass.h neuron.h \
  forceable.h synapsetarget.h switch.i

discretizationpreprocessor.o: discretizationpreprocessor.cpp \
  discretizationpreprocessor.h preprocessor.h csimerror.h csimclass.h \
  globaldefinitions.h advanceable.h

dynamicanalogsynapse.o: dynamicanalogsynapse.cpp randgen.h \
  dynamicanalogsynapse.h analogsynapse.h synapse.h advanceable.h \
  csimclass.h globaldefinitions.h neuron.h forceable.h csimlist.h \
  synapsetarget.h

dynamicglutamatesynapse.o: dynamicglutamatesynapse.cpp \
  dynamicglutamatesynapse.h glutamatesynapse.h stdpsynapse.h \
  spikingsynapse.h synapse.h advanceable.h csimclass.h \
  globaldefinitions.h neuron.h forceable.h csimlist.h synapsetarget.h \
  needspostspikesynapse.h spikingneuron.h dynamicsynapse.h

dynamicglutamatesynapsesynchan.o: dynamicglutamatesynapsesynchan.cpp \
  dynamicglutamatesynapsesynchan.h glutamatesynapsesynchan.h \
  stdpsynapse.h spikingsynapse.h synapse.h advanceable.h csimclass.h \
  globaldefinitions.h neuron.h forceable.h csimlist.h synapsetarget.h \
  needspostspikesynapse.h spikingneuron.h dynamicsynapse.h \
  lifneuronsynchan.h lifneuron.h internalvoltageneuron.h

dynamicspikingcbsynapse.o: dynamicspikingcbsynapse.cpp \
  dynamicspikingcbsynapse.h dynamicspikingsynapse.h spikingsynapse.h \
  synapse.h advanceable.h csimclass.h globaldefinitions.h neuron.h \
  forceable.h csimlist.h synapsetarget.h spikingneuron.h dynamicsynapse.h \
  cbneuron.h ionchannel.h membranepatchsimple.h membranepatch.h \
  ionbuffer.h csimerror.h

dynamicspikingsynapse.o: dynamicspikingsynapse.cpp \
  dynamicspikingsynapse.h spikingsynapse.h synapse.h advanceable.h \
  csimclass.h globaldefinitions.h neuron.h forceable.h csimlist.h \
  synapsetarget.h spikingneuron.h dynamicsynapse.h

dynamicstdpsynapse.o: dynamicstdpsynapse.cpp dynamicstdpsynapse.h \
  stdpsynapse.h spikingsynapse.h synapse.h advanceable.h csimclass.h \
  globaldefinitions.h neuron.h forceable.h csimlist.h synapsetarget.h \
  needspostspikesynapse.h spikingneuron.h dynamicsynapse.h

expspikefilter.o: expspikefilter.cpp expspikefilter.h spikefilter.h \
  filterfunction.h csimerror.h advanceable.h csimclass.h \
  globaldefinitions.h spikingneuron.h neuron.h forceable.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h

extinputneuron.o: extinputneuron.cpp csimerror.h extinputneuron.h \
  neuron.h forceable.h advanceable.h csimclass.h globaldefinitions.h \
  csimlist.h synapsetarget.h analoginputneuron.h analogneuron.h \
  analogsynapse.h synapse.h csiminputclass.h

extoutlifneuron.o: extoutlifneuron.cpp csimerror.h extoutlifneuron.h \
  lifneuron.h spikingneuron.h neuron.h forceable.h advanceable.h \
  csimclass.h globaldefinitions.h csimlist.h synapsetarget.h \
  spikingsynapse.h synapse.h internalvoltageneuron.h

extoutlinearneuron.o: extoutlinearneuron.cpp extoutlinearneuron.h \
  linearneuron.h analogneuron.h neuron.h forceable.h advanceable.h \
  csimclass.h globaldefinitions.h csimlist.h synapsetarget.h \
  analogsynapse.h synapse.h csimerror.h

extoutsigmoidalneuron.o: extoutsigmoidalneuron.cpp \
  extoutsigmoidalneuron.h sigmoidalneuron.h analogneuron.h neuron.h \
  forceable.h advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h analogsynapse.h synapse.h csimerror.h

filterfunction.o: filterfunction.cpp filterfunction.h csimerror.h \
  advanceable.h csimclass.h globaldefinitions.h

forceable.o: forceable.cpp forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimerror.h teacher.h csimlist.h

gaussiananalogfilter.o: gaussiananalogfilter.cpp gaussiananalogfilter.h \
  analogfilter.h filterfunction.h csimerror.h advanceable.h csimclass.h \
  globaldefinitions.h

generic_ion_gates.o: generic_ion_gates.cpp generic_ion_gates.h viongate.h \
  advanceable.h csimclass.h globaldefinitions.h membranepatchsimple.h \
  spikingneuron.h neuron.h forceable.h csimlist.h synapsetarget.h \
  spikingsynapse.h synapse.h ionchannel.h iongate.h csimerror.h

glutamatesynapse.o: glutamatesynapse.cpp glutamatesynapse.h stdpsynapse.h \
  spikingsynapse.h synapse.h advanceable.h csimclass.h \
  globaldefinitions.h neuron.h forceable.h csimlist.h synapsetarget.h \
  needspostspikesynapse.h spikingneuron.h lifneuron.h \
  internalvoltageneuron.h csimerror.h

glutamatesynapsesynchan.o: glutamatesynapsesynchan.cpp \
  glutamatesynapsesynchan.h stdpsynapse.h spikingsynapse.h synapse.h \
  advanceable.h csimclass.h globaldefinitions.h neuron.h forceable.h \
  csimlist.h synapsetarget.h needspostspikesynapse.h spikingneuron.h \
  lifneuronsynchan.h lifneuron.h internalvoltageneuron.h csimerror.h

hh_neuron.o: hh_neuron.cpp hh_neuron.h cbneuron.h spikingneuron.h \
  neuron.h forceable.h advanceable.h csimclass.h globaldefinitions.h \
  csimlist.h synapsetarget.h spikingsynapse.h synapse.h ionchannel.h \
  membranepatchsimple.h membranepatch.h ionbuffer.h hh_squid_channels.h \
  viongate.h iongate.h activechannel.h

hh_squid_channels.o: hh_squid_channels.cpp hh_squid_channels.h viongate.h \
  advanceable.h csimclass.h globaldefinitions.h membranepatchsimple.h \
  spikingneuron.h neuron.h forceable.h csimlist.h synapsetarget.h \
  spikingsynapse.h synapse.h ionchannel.h iongate.h activechannel.h

ifbneuron.o: ifbneuron.cpp randgen.h ifbneuron.h spikingneuron.h neuron.h \
  forceable.h advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h mexnetwork.h \
  /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h network.h csimerror.h

ionbuffer.o: ionbuffer.cpp ionbuffer.h ionchannel.h globaldefinitions.h \
  advanceable.h csimclass.h membranepatchsimple.h spikingneuron.h \
  neuron.h forceable.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h

ionchannel.o: ionchannel.cpp ionchannel.h globaldefinitions.h \
  advanceable.h csimclass.h membranepatchsimple.h spikingneuron.h \
  neuron.h forceable.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h cbneuron.h membranepatch.h ionbuffer.h analoginputneuron.h \
  analogneuron.h analogsynapse.h csiminputclass.h csimerror.h

iongate.o: iongate.cpp iongate.h advanceable.h csimclass.h \
  globaldefinitions.h membranepatchsimple.h spikingneuron.h neuron.h \
  forceable.h csimlist.h synapsetarget.h spikingsynapse.h synapse.h \
  ionchannel.h csimerror.h

izhi_neuron.o: izhi_neuron.cpp randgen.h lifneuron.h spikingneuron.h \
  neuron.h forceable.h advanceable.h csimclass.h globaldefinitions.h \
  csimlist.h synapsetarget.h spikingsynapse.h synapse.h \
  internalvoltageneuron.h mexnetwork.h \
  /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h network.h csimerror.h \
  izhi_neuron.h

lifburstneuron.o: lifburstneuron.cpp randgen.h lifburstneuron.h \
  spikingneuron.h neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h mexnetwork.h /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h network.h csimerror.h

lifneuron.o: lifneuron.cpp randgen.h lifneuron.h spikingneuron.h neuron.h \
  forceable.h advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h internalvoltageneuron.h \
  csimerror.h

lifneuronsynchan.o: lifneuronsynchan.cpp randgen.h lifneuronsynchan.h \
  csimclass.h globaldefinitions.h lifneuron.h spikingneuron.h neuron.h \
  forceable.h advanceable.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h internalvoltageneuron.h csimerror.h

linear_classification.o: linear_classification.cpp \
  linear_classification.h algorithm.h csimerror.h csimclass.h \
  globaldefinitions.h advanceable.h

linearneuron.o: linearneuron.cpp randgen.h linearneuron.h analogneuron.h \
  neuron.h forceable.h advanceable.h csimclass.h globaldefinitions.h \
  csimlist.h synapsetarget.h analogsynapse.h synapse.h

linearpreprocessor.o: linearpreprocessor.cpp linearpreprocessor.h \
  preprocessor.h csimerror.h csimclass.h globaldefinitions.h \
  advanceable.h

linear_regression.o: linear_regression.cpp linear_regression.h \
  algorithm.h csimerror.h csimclass.h globaldefinitions.h advanceable.h

mean_std_preprocessor.o: mean_std_preprocessor.cpp \
  mean_std_preprocessor.h preprocessor.h csimerror.h csimclass.h \
  globaldefinitions.h advanceable.h

membranepatch.o: membranepatch.cpp randgen.h membranepatch.h \
  spikingneuron.h neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h ionchannel.h membranepatchsimple.h ionbuffer.h \
  activecachannel.h activechannel.h

membranepatchsimple.o: membranepatchsimple.cpp randgen.h \
  membranepatchsimple.h spikingneuron.h neuron.h forceable.h \
  advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h ionchannel.h

mexnetwork.o: mexnetwork.cpp mexnetwork.h \
  /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h globaldefinitions.h \
  network.h csimlist.h spikingsynapse.h synapse.h advanceable.h \
  csimclass.h neuron.h forceable.h synapsetarget.h mexrecorder.h \
  recorder.h csimerror.h csimmex.h readout.h analogfilter.h \
  filterfunction.h spikefilter.h spikingneuron.h preprocessor.h \
  algorithm.h modelinput.h physicalmodel.h analogsynapse.h classlist.i \
  switch.i

mexrecorder.o: mexrecorder.cpp mexrecorder.h \
  /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h recorder.h advanceable.h \
  csimclass.h globaldefinitions.h spikingneuron.h neuron.h forceable.h \
  csimlist.h synapsetarget.h spikingsynapse.h synapse.h

network.o: network.cpp globaldefinitions.h network.h csimlist.h \
  spikingsynapse.h synapse.h advanceable.h csimclass.h neuron.h \
  forceable.h synapsetarget.h csimerror.h spikingneuron.h analogsynapse.h \
  teacher.h recorder.h readout.h analogfilter.h filterfunction.h \
  spikefilter.h preprocessor.h algorithm.h modelinput.h physicalmodel.h \
  randgen.h csiminputclass.h

neuron.o: neuron.cpp neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h synapse.h csimerror.h

pcapreprocessor.o: pcapreprocessor.cpp pcapreprocessor.h preprocessor.h \
  csimerror.h csimclass.h globaldefinitions.h advanceable.h

physicalmodel.o: physicalmodel.cpp physicalmodel.h csimerror.h \
  csimclass.h globaldefinitions.h advanceable.h modelinput.h \
  analogsynapse.h synapse.h neuron.h forceable.h csimlist.h \
  synapsetarget.h readout.h analogfilter.h filterfunction.h spikefilter.h \
  spikingneuron.h spikingsynapse.h preprocessor.h algorithm.h

preprocessor.o: preprocessor.cpp preprocessor.h csimerror.h csimclass.h \
  globaldefinitions.h advanceable.h

randgen.o: randgen.cpp randgen.h

readout.o: readout.cpp readout.h advanceable.h csimclass.h \
  globaldefinitions.h analogfilter.h filterfunction.h csimerror.h \
  spikefilter.h spikingneuron.h neuron.h forceable.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h preprocessor.h algorithm.h \
  modelinput.h

recorder.o: recorder.cpp csimclass.h globaldefinitions.h recorder.h \
  advanceable.h csimerror.h spikingneuron.h neuron.h forceable.h \
  csimlist.h synapsetarget.h spikingsynapse.h synapse.h

sigmoidalneuron.o: sigmoidalneuron.cpp sigmoidalneuron.h analogneuron.h \
  neuron.h forceable.h advanceable.h csimclass.h globaldefinitions.h \
  csimlist.h synapsetarget.h analogsynapse.h synapse.h randgen.h

specific_ion_channels.o: specific_ion_channels.cpp \
  specific_ion_channels.h viongate.h advanceable.h csimclass.h \
  globaldefinitions.h membranepatchsimple.h spikingneuron.h neuron.h \
  forceable.h csimlist.h synapsetarget.h spikingsynapse.h synapse.h \
  ionchannel.h iongate.h conciongate.h membranepatch.h ionbuffer.h \
  activechannel.h activecachannel.h csimerror.h cbneuron.h \
  analoginputneuron.h analogneuron.h analogsynapse.h csiminputclass.h

specific_neurons.o: specific_neurons.cpp specific_neurons.h cbneuronst.h \
  cbneuron.h spikingneuron.h neuron.h forceable.h advanceable.h \
  csimclass.h globaldefinitions.h csimlist.h synapsetarget.h \
  spikingsynapse.h synapse.h ionchannel.h membranepatchsimple.h \
  membranepatch.h ionbuffer.h ahp_channel.h specific_ion_channels.h \
  viongate.h iongate.h conciongate.h activechannel.h activecachannel.h \
  csimerror.h mexnetwork.h /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h network.h

specificouneurons.o: specificouneurons.cpp specificouneurons.h \
  cbstouneuron.h cbneuronst.h cbneuron.h spikingneuron.h neuron.h \
  forceable.h advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h ionchannel.h \
  membranepatchsimple.h membranepatch.h ionbuffer.h ahp_channel.h \
  specific_ion_channels.h viongate.h iongate.h conciongate.h \
  activechannel.h activecachannel.h csimerror.h mexnetwork.h \
  /usr/local/matlab/extern/include/mex.h \
  /usr/local/matlab/extern/include/matrix.h \
  /usr/local/matlab/extern/include/tmwtypes.h network.h

spikefilter.o: spikefilter.cpp spikefilter.h filterfunction.h csimerror.h \
  advanceable.h csimclass.h globaldefinitions.h spikingneuron.h neuron.h \
  forceable.h csimlist.h synapsetarget.h spikingsynapse.h synapse.h

spikinginputneuron.o: spikinginputneuron.cpp spikinginputneuron.h \
  spikingneuron.h neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h csiminputclass.h csimerror.h

spikingneuron.o: spikingneuron.cpp spikingneuron.h neuron.h forceable.h \
  advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h stdpsynapse.h \
  needspostspikesynapse.h network.h

spikingsynapse.o: spikingsynapse.cpp randgen.h spikingneuron.h neuron.h \
  forceable.h advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  synapsetarget.h spikingsynapse.h synapse.h csimerror.h lifneuron.h \
  internalvoltageneuron.h

spikingteacher.o: spikingteacher.cpp spikingteacher.h teacher.h \
  advanceable.h csimclass.h globaldefinitions.h csimlist.h \
  spikingneuron.h neuron.h forceable.h synapsetarget.h spikingsynapse.h \
  synapse.h csiminputclass.h csimerror.h

staticanalogcbsynapse.o: staticanalogcbsynapse.cpp \
  staticanalogcbsynapse.h analogsynapse.h synapse.h advanceable.h \
  csimclass.h globaldefinitions.h neuron.h forceable.h csimlist.h \
  synapsetarget.h lifneuron.h spikingneuron.h spikingsynapse.h \
  internalvoltageneuron.h cbneuron.h ionchannel.h membranepatchsimple.h \
  membranepatch.h ionbuffer.h randgen.h csimerror.h

staticanalogsynapse.o: staticanalogsynapse.cpp staticanalogsynapse.h \
  analogsynapse.h synapse.h advanceable.h csimclass.h globaldefinitions.h \
  neuron.h forceable.h csimlist.h synapsetarget.h randgen.h

staticglutamatesynapse.o: staticglutamatesynapse.cpp \
  staticglutamatesynapse.h glutamatesynapse.h stdpsynapse.h \
  spikingsynapse.h synapse.h advanceable.h csimclass.h \
  globaldefinitions.h neuron.h forceable.h csimlist.h synapsetarget.h \
  needspostspikesynapse.h

staticglutamatesynapsesynchan.o: staticglutamatesynapsesynchan.cpp \
  staticglutamatesynapsesynchan.h glutamatesynapsesynchan.h stdpsynapse.h \
  spikingsynapse.h synapse.h advanceable.h csimclass.h \
  globaldefinitions.h neuron.h forceable.h csimlist.h synapsetarget.h \
  needspostspikesynapse.h

staticspikingcbsynapse.o: staticspikingcbsynapse.cpp \
  staticspikingcbsynapse.h staticspikingsynapse.h globaldefinitions.h \
  spikingsynapse.h synapse.h advanceable.h csimclass.h neuron.h \
  forceable.h csimlist.h synapsetarget.h cbneuron.h spikingneuron.h \
  ionchannel.h membranepatchsimple.h membranepatch.h ionbuffer.h \
  csimerror.h

staticspikingsynapse.o: staticspikingsynapse.cpp staticspikingsynapse.h \
  globaldefinitions.h spikingsynapse.h synapse.h advanceable.h \
  csimclass.h neuron.h forceable.h csimlist.h synapsetarget.h

staticstdpsynapse.o: staticstdpsynapse.cpp staticstdpsynapse.h \
  stdpsynapse.h spikingsynapse.h synapse.h advanceable.h csimclass.h \
  globaldefinitions.h neuron.h forceable.h csimlist.h synapsetarget.h \
  needspostspikesynapse.h

stdpsynapse.o: stdpsynapse.cpp stdpsynapse.h spikingsynapse.h synapse.h \
  advanceable.h csimclass.h globaldefinitions.h neuron.h forceable.h \
  csimlist.h synapsetarget.h needspostspikesynapse.h spikingneuron.h \
  csimerror.h

synapse.o: synapse.cpp synapse.h advanceable.h csimclass.h \
  globaldefinitions.h neuron.h forceable.h csimlist.h synapsetarget.h \
  csimerror.h

synapticchannel.o: synapticchannel.cpp synapticchannel.h ionchannel.h \
  globaldefinitions.h advanceable.h csimclass.h membranepatchsimple.h \
  spikingneuron.h neuron.h forceable.h csimlist.h synapsetarget.h \
  spikingsynapse.h synapse.h

teacher.o: teacher.cpp teacher.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h forceable.h csimerror.h network.h \
  spikingsynapse.h synapse.h neuron.h synapsetarget.h

TraubsHHNeuron.o: TraubsHHNeuron.cpp TraubsHHNeuron.h cbneuron.h \
  spikingneuron.h neuron.h forceable.h advanceable.h csimclass.h \
  globaldefinitions.h csimlist.h synapsetarget.h spikingsynapse.h \
  synapse.h ionchannel.h membranepatchsimple.h membranepatch.h \
  ionbuffer.h viongate.h iongate.h activechannel.h

triangularanalogfilter.o: triangularanalogfilter.cpp \
  triangularanalogfilter.h analogfilter.h filterfunction.h csimerror.h \
  advanceable.h csimclass.h globaldefinitions.h

useranalogfilter.o: useranalogfilter.cpp useranalogfilter.h \
  analogfilter.h filterfunction.h csimerror.h advanceable.h csimclass.h \
  globaldefinitions.h

viongate.o: viongate.cpp viongate.h advanceable.h csimclass.h \
  globaldefinitions.h membranepatchsimple.h spikingneuron.h neuron.h \
  forceable.h csimlist.h synapsetarget.h spikingsynapse.h synapse.h \
  ionchannel.h iongate.h csimerror.h

