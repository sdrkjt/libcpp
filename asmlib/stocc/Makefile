MAKEFLAGS += -s
BOLD = tput bold
NORMAL = tput sgr0

THIS_SETUP = /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/setup.txt
SOURCEPATH = /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/
OBJECTPATH = /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/z_lib/z_obj/hometrungdunggnuradioWORKINGPROJECTasmlibstoccsetup.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/
LIBRARYPATH = /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/

FLAGS = -Wswitch-default -ggdb -DDEBUG
INCLUDES = -msse2 -fno-builtin -I$(SOURCEPATH)
LINKS =  -Wl,--start-group -lm -lpthread -Wl,--end-group

MACH = -msse2
CXX = ccache g++ $(MACH) $(PARAM)
THIS_LIB = $(LIBRARYPATH)libhometrungdunggnuradioWORKINGPROJECTasmlibstoccsetup.txt.a



OBJECT = $(OBJECTPATH)stoc1.o \
         $(OBJECTPATH)stoc2.o \
         $(OBJECTPATH)stoc3.o \
         $(OBJECTPATH)erfres.o \
         $(OBJECTPATH)fnchyppr.o \
         $(OBJECTPATH)wnchyppr.o \

SOURCE = $(SOURCEPATH)stoc1.cc \
         $(SOURCEPATH)stoc2.cc \
         $(SOURCEPATH)stoc3.cc \
         $(SOURCEPATH)erfres.cc \
         $(SOURCEPATH)fnchyppr.cc \
         $(SOURCEPATH)wnchyppr.cc \

PROG0   = ex_stoc
PROG0_l = $(OBJECTPATH)$(PROG0)
OBJECT0 = $(OBJECTPATH)$(PROG0).o
SOURCE0 = $(OBJECTPATH)$(PROG0).cc

all: checkdir /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/Makefile $(THIS_LIB) $(PROG0_l) progs cleanup

$(OBJECTPATH)%.d: /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/setup.txt
	rm -f $(OBJECTPATH)$(*F).d

-include $(PROG0_l).d
-include $(subst $(SOURCEPATH), $(OBJECTPATH), $(SOURCE:.cc=.d))
$(OBJECTPATH)%.o: $(SOURCEPATH)%.cc /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/setup.txt
	$(CXX) -MF $(OBJECTPATH)$(*F).d -MT $(OBJECTPATH)$(*F).o -MM $(FLAGS) $(INCLUDES) $<
	echo "Compiling $<"
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

$(THIS_LIB): $(OBJECT) /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/setup.txt
	echo "Archiving library $(THIS_LIB)"
	rm -f $(THIS_LIB); \
	ar rc $(THIS_LIB) $(OBJECT); \
	ranlib $(THIS_LIB)

.PHONY: progs
progs:
	rm -f $(PROG0)
	ln -s $(PROG0_l)

$(PROG0_l): $(OBJECT) $(OBJECT0) /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/Makefile
	echo "Building $(SOURCEPATH)$(PROG0)"
	$(CXX) -o $(PROG0_l) $(OBJECT) $(OBJECT0) $(LINKS)

.PHONY: clean
clean:
	echo "Cleaning everything in /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc"
	rm -f $(OBJECTPATH)*
	rm -f $(LIBRARYPATH)*
	rm -f $(PROG0)

.PHONY: clean_all
clean_all: clean

.PHONY: cleanup
cleanup:
	touch $(THIS_LIB)

.PHONY: checkdir
checkdir:
	if [ ! -d $(OBJECTPATH) ]; then mkdir -p $(OBJECTPATH); fi
	if [ ! -d $(LIBRARYPATH) ]; then mkdir -p $(LIBRARYPATH); fi

/home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/Makefile: /home/trungdung/gnuradio/WORKING/PROJECT/asmlib/stocc/setup.txt
	echo "setup or fpga file list has changed: call domakefile before compiling."
	exit 1

