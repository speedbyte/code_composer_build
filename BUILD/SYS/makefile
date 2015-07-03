# For a full build in cygnus bash shell type:
#   > make clean
#   > make

#
# Global project options are set in makeopts at top level of project tree
# Each lower level makefile must define TOP as the relative path to the top
# of the project tree.  This allows all tools to run from within the project
# structure (and to be included in the project for version control!).
# All lower make files should include a clean definition which deletes all
# buildable objects.
#
# Make will build the specified targets (all is the default) in the list of
# subdirectories provided. This Makefile is for GNU make.

# define top directory relative to this working directory
TOP = $(ROOTDIR)

# include global make options
include $(ROOTDIR)/BUILD/SYS/makeopts

# Define the subdirectories in which to run "make".
SUBDIRS = MOD

# Define targets
TARGETS = all clean

MAKEFILE = $(ROOTDIR)/BUILD/SYS/makefile
MODMAKE = $(ROOTDIR)/BUILD/MOD/makefile

$(TARGETS) : $(MAKEFILE) FORCE
	$(ECHO) Target: $@
	$(ECHO) Using makefile $<
	${MAKE} -f $< ${SUBDIRS} TARGET=$@

# Nested calls to make in the subdirectories.

${SUBDIRS} : FORCE
	$(ECHO) 
	$(ECHO) Working in directory $@
	$(ECHO) Target: $(TARGET)
	$(ECHO) Using makefile $(MODMAKE)
	cd $@; \
	${MAKE} -f $(MODMAKE) $(TARGET) \


FORCE:


