MAKEFLAGS += -s
BOLD = tput bold
NORMAL = tput sgr0

THIS_SETUP = /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/setup.txt
SOURCEPATH = /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/
OBJECTPATH = /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/z_lib/z_obj/hometrungdunggnuradioWORKINGPROJECTDVBTreed-solomon-3.1.1_modifsetup.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/
LIBRARYPATH = /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/

FLAGS = -Wswitch-default -ggdb -DDEBUG
INCLUDES = -msse2 -O3 -fno-builtin -O3 -I$(SOURCEPATH)
LINKS =  -Wl,--start-group -lm -lpthread -Wl,--end-group

MACH = -msse2
CXX = ccache g++ $(MACH) $(PARAM)
THIS_LIB = $(LIBRARYPATH)libhometrungdunggnuradioWORKINGPROJECTDVBTreed-solomon-3.1.1_modifsetup.txt.a



OBJECT = $(OBJECTPATH)exercise.o \
         $(OBJECTPATH)encode_rs.o \
         $(OBJECTPATH)encode_rs_ccsds.o \
         $(OBJECTPATH)decode_rs.o \
         $(OBJECTPATH)decode_rs_ccsds.o \
         $(OBJECTPATH)init_rs.o \
         $(OBJECTPATH)ccsds_tab.o \
         $(OBJECTPATH)ccsds_tal.o \

SOURCE = $(SOURCEPATH)exercise.cc \
         $(SOURCEPATH)encode_rs.cc \
         $(SOURCEPATH)encode_rs_ccsds.cc \
         $(SOURCEPATH)decode_rs.cc \
         $(SOURCEPATH)decode_rs_ccsds.cc \
         $(SOURCEPATH)init_rs.cc \
         $(SOURCEPATH)ccsds_tab.cc \
         $(SOURCEPATH)ccsds_tal.cc \

PROG0   = rstest
PROG0_l = $(OBJECTPATH)$(PROG0)
OBJECT0 = $(OBJECTPATH)$(PROG0).o
SOURCE0 = $(OBJECTPATH)$(PROG0).cc

PROG1   = gen_ccsds
PROG1_l = $(OBJECTPATH)$(PROG1)
OBJECT1 = $(OBJECTPATH)$(PROG1).o
SOURCE1 = $(OBJECTPATH)$(PROG1).cc

PROG2   = gen_ccsdstal
PROG2_l = $(OBJECTPATH)$(PROG2)
OBJECT2 = $(OBJECTPATH)$(PROG2).o
SOURCE2 = $(OBJECTPATH)$(PROG2).cc

all: checkdir /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/Makefile $(THIS_LIB) $(PROG0_l) $(PROG1_l) $(PROG2_l) progs cleanup

$(OBJECTPATH)%.d: /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/setup.txt
	rm -f $(OBJECTPATH)$(*F).d

-include $(PROG0_l).d
-include $(PROG1_l).d
-include $(PROG2_l).d
-include $(subst $(SOURCEPATH), $(OBJECTPATH), $(SOURCE:.cc=.d))
$(OBJECTPATH)%.o: $(SOURCEPATH)%.cc /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/setup.txt
	$(CXX) -MF $(OBJECTPATH)$(*F).d -MT $(OBJECTPATH)$(*F).o -MM $(FLAGS) $(INCLUDES) $<
	echo "Compiling $<"
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

$(THIS_LIB): $(OBJECT) /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/setup.txt
	echo "Archiving library $(THIS_LIB)"
	rm -f $(THIS_LIB); \
	ar rc $(THIS_LIB) $(OBJECT); \
	ranlib $(THIS_LIB)

.PHONY: progs
progs:
	rm -f $(PROG0)
	ln -s $(PROG0_l)
	rm -f $(PROG1)
	ln -s $(PROG1_l)
	rm -f $(PROG2)
	ln -s $(PROG2_l)

$(PROG0_l): $(OBJECT) $(OBJECT0) /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/Makefile
	echo "Building $(SOURCEPATH)$(PROG0)"
	$(CXX) -o $(PROG0_l) $(OBJECT) $(OBJECT0) $(LINKS)

$(PROG1_l): $(OBJECT) $(OBJECT1) /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/Makefile
	echo "Building $(SOURCEPATH)$(PROG1)"
	$(CXX) -o $(PROG1_l) $(OBJECT) $(OBJECT1) $(LINKS)

$(PROG2_l): $(OBJECT) $(OBJECT2) /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/Makefile
	echo "Building $(SOURCEPATH)$(PROG2)"
	$(CXX) -o $(PROG2_l) $(OBJECT) $(OBJECT2) $(LINKS)

.PHONY: clean
clean:
	echo "Cleaning everything in /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif"
	rm -f $(OBJECTPATH)*
	rm -f $(LIBRARYPATH)*
	rm -f $(PROG0)
	rm -f $(PROG1)
	rm -f $(PROG2)

.PHONY: clean_all
clean_all: clean

.PHONY: cleanup
cleanup:
	touch $(THIS_LIB)

.PHONY: checkdir
checkdir:
	if [ ! -d $(OBJECTPATH) ]; then mkdir -p $(OBJECTPATH); fi
	if [ ! -d $(LIBRARYPATH) ]; then mkdir -p $(LIBRARYPATH); fi

/home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/Makefile: /home/trungdung/gnuradio/WORKING/PROJECT/DVBT/reed-solomon-3.1.1_modif/setup.txt
	echo "setup or fpga file list has changed: call domakefile before compiling."
	exit 1

