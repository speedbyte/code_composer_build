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

# suppress warnings
CFLAGS	:= $(CFLAGS) --diag_suppress=190 --diag_suppress=193
#CFLAGS	:= $(CFLAGS) --display_error_number


LIBRARY = $(LIB_MOD_BAP)
OBJS= \
	BAP-SW_UHVhigh_NAR_Phone_PB_MDI.obj\
bap_bal.obj\
bap_balutil.obj\
bap_bcl.obj\
bap_bclcan.obj\
bap_bclcan_vector_TMS470struct.obj\
bap_bclutil.obj\
bap_bpl.obj\
bap_bplutil.obj\
bap_controller.obj\
bap_debug.obj\
bap_util.obj\

all : $(LIBRARY)
$(LIBRARY): $(OBJS)
	$(MODULEMAKE)

$(DEPEND)


clean:
	$(MODULECLEAN)
