MAKEFLAGS += -s
BOLD = tput bold
NORMAL = tput sgr0

THIS_SETUP = /home/sdr/WORKING/libsig/setup.txt
SOURCEPATH = /home/sdr/WORKING/libsig/
OBJECTPATH = /home/sdr/WORKING/libsig/z_lib/z_obj/homesdrWORKINGlibsigsetup.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/
LIBRARYPATH = /home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/

FLAGS = -Wswitch-default -ggdb -DDEBUG
INCLUDES = -msse2 -O3 -fno-builtin -O3 -I$(SOURCEPATH) $(BLOCK0_I) $(USER0_I) $(USER1_I) $(USER2_I) $(USER3_I) $(USER4_I) $(USER5_I)
LINKS =  -L$(BLOCK0_L) -Wl,--start-group -lm -lpthread -lhomesdrWORKINGlibsigsetup_lib.txt -lQtGui -lQtCore -lpthread -lqwt -Wl,--end-group

MACH = -msse2
CXX = ccache g++ $(MACH) $(PARAM)
THIS_LIB = $(LIBRARYPATH)libhomesdrWORKINGlibsigsetup.txt.a

USER0_I = -I/home/sdr/WORKING/libgui
USER1_I = -I/home/sdr/WORKING/asmlib
USER2_I = -I/usr/lib/qt4/mkspecs/linux-g++
USER3_I = -I/usr/include/QtCore
USER4_I = -I/usr/include/QtGui
USER5_I = -I/usr/include/qwt

BLOCK0_I = -I/home/sdr/WORKING/libsig
BLOCK0_L = /home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)

DEP_LIBS = /home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/libhomesdrWORKINGlibsigsetup_lib.txt.a \

PROG0   = t_AlignedArray
PROG0_l = $(OBJECTPATH)$(PROG0)
OBJECT0 = $(OBJECTPATH)$(PROG0).o
SOURCE0 = $(OBJECTPATH)$(PROG0).cc

all: checkdir /home/sdr/WORKING/libsig/Makefile $(DEP_LIBS) $(PROG0_l) progs cleanup

homesdrWORKINGlibsigsetup_lib.txt_deps = $(wildcard /home/sdr/WORKING/libsig/*.h) $(wildcard /home/sdr/WORKING/libsig/*.hh) $(wildcard /home/sdr/WORKING/libsig/*.cc) $(wildcard /home/sdr/WORKING/libgui/*.h) $(wildcard /home/sdr/WORKING/libgui/*.hh)
/home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/libhomesdrWORKINGlibsigsetup_lib.txt.a: $(homesdrWORKINGlibsigsetup_lib.txt_deps) /home/sdr/WORKING/libsig/setup_lib.txt
	$(BOLD) && echo "Making in /home/sdr/WORKING/libsig using this setup: setup_lib.txt" && $(NORMAL)
	$(MAKE) -C /home/sdr/WORKING/libsig -f /home/sdr/WORKING/libsig/z_lib/z_obj/homesdrWORKINGlibsigsetup_lib.txt.mk
	$(BOLD) && echo "Back to /home/sdr/WORKING/libsig" && $(NORMAL)

$(OBJECTPATH)%.d: /home/sdr/WORKING/libsig/setup.txt
	rm -f $(OBJECTPATH)$(*F).d

-include $(PROG0_l).d
-include $(subst $(SOURCEPATH), $(OBJECTPATH), $(SOURCE:.cc=.d))
$(OBJECTPATH)%.o: $(SOURCEPATH)%.cc /home/sdr/WORKING/libsig/setup.txt
	$(CXX) -MF $(OBJECTPATH)$(*F).d -MT $(OBJECTPATH)$(*F).o -MM $(FLAGS) $(INCLUDES) $<
	echo "Compiling $<"
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

$(THIS_LIB): $(OBJECT) /home/sdr/WORKING/libsig/setup.txt
	echo "Archiving library $(THIS_LIB)"
	rm -f $(THIS_LIB); \
	ar rc $(THIS_LIB) $(OBJECT); \
	ranlib $(THIS_LIB)

.PHONY: progs
progs:
	rm -f $(PROG0)
	ln -s $(PROG0_l)

$(PROG0_l): $(OBJECT) $(OBJECT0) $(DEP_LIBS) /home/sdr/WORKING/libsig/Makefile
	echo "Building $(SOURCEPATH)$(PROG0)"
	$(CXX) -o $(PROG0_l) $(OBJECT) $(OBJECT0) $(LINKS)

.PHONY: clean
clean:
	echo "Cleaning everything in /home/sdr/WORKING/libsig"
	rm -f $(OBJECTPATH)*
	rm -f $(LIBRARYPATH)*
	rm -f $(PROG0)

.PHONY: clean_all
clean_all: clean
	echo "Cleaning everything in /home/sdr/WORKING/libsig"
	rm -f /home/sdr/WORKING/libsig/z_lib/z_obj/homesdrWORKINGlibsigsetup_lib.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/*
	rm -f /home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/*
	echo "Cleaning everything in /home/sdr/WORKING/libgui"
	rm -f /home/sdr/WORKING/libgui/z_lib/z_obj/homesdrWORKINGlibguisetup_lib.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/*
	rm -f /home/sdr/WORKING/libgui/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/*

.PHONY: cleanup
cleanup:
	touch $(THIS_LIB)

.PHONY: checkdir
checkdir:
	if [ ! -d $(OBJECTPATH) ]; then mkdir -p $(OBJECTPATH); fi
	if [ ! -d $(LIBRARYPATH) ]; then mkdir -p $(LIBRARYPATH); fi

/home/sdr/WORKING/libsig/Makefile: /home/sdr/WORKING/libsig/setup.txt /home/sdr/WORKING/libsig/setup_lib.txt /home/sdr/WORKING/libgui/setup_lib.txt
	echo "setup or fpga file list has changed: call domakefile before compiling."
	exit 1

