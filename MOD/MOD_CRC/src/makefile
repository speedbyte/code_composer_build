################################################################################


# For a full build in cygnus bash shell type :
#   > make clean
#   > make
#
# Global project options are set in makeopts at top level of project tree.
# Each lower level makefile must define TOP as the relative path to the top
# of the project tree.  This allows all tools to run from within the project
# structure (and to be included in the project for version control!).
# All lower make files should include a clean definition which deletes all
# buildable objects.
#
# Make will build the specified targets (all is the default) in the list of
# subdirectories provided. This Makefile is for GNU make.

# define top directory relative to this working directory
TOP       = ../../..
TEMPDIR = $(TOP)/TEMP
# include global make options
include $(TOP)/BUILD/SYS/makeopts
include $(TEMPDIR)/uCprojectopts

LIBRARY = $(LIB_MOD_CRC)
OBJS= \
	Crcmgr_C1.obj\
Crc_C1.obj\

all : $(LIBRARY)
$(LIBRARY): $(OBJS)
	$(MODULEMAKE)

$(DEPEND)


clean:
	$(MODULECLEAN)
