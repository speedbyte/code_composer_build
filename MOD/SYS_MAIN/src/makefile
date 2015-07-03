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
TOP		= ../../..
TEMPDIR = $(TOP)/TEMP
MODDIR = $(TOP)/MOD
# include global make options
include $(TOP)/BUILD/SYS/makeopts
include $(TEMPDIR)/uCprojectopts


# Define Build image
IMAGE	= UhvNar

# Define debug directory
DEBUGDIR = ../../debug/
# Define output image directory
OUTDIR = ../../../OUT/

#-----------------------------------------------------------------------------
#
#-----------------------------------------------------------------------------
LOUT = $(IMAGE)

OBJSTARTUP = Startup.obj

OBJS =  SYS_Main.obj        \
		intvect.obj        \
		vectors.obj

all	:	$(LOUT)

$(LOUT)	: $(OBJSTARTUP) $(OBJS)
	$(ECHO)
	$(ECHO) Linking $(LOUT) Software
# Link order is importent do not change
	$(ECHO) $(LN) $(LFLAGS) $(OBJSTARTUP) $(CONTROLLERDIR)/$(LINKOPTSFILE) $(OBJS) $(LIBS) -m$(OUTDIR)$(IMAGE).map -o$(OUTDIR)$(IMAGE).out
	$(LN) $(LFLAGS) $(OBJSTARTUP) $(TEMPDIR)/$(LINKOPTSFILE) $(OBJS) $(LIBS) -m$(OUTDIR)$(IMAGE).map -o$(OUTDIR)$(IMAGE).out
	$(TRID) folder $(MODDIR) $(OUTDIR)$(IMAGE).tr $(OUTDIR)$(IMAGE).enum
# generate hex file
	$(F2H) $(F2HFLAGS) $(TEMPDIR)/$(ROMOPTSFILE) $(OUTDIR)$(IMAGE).out -o $(OUTDIR)$(IMAGE).hex
	$(FULLH) $(OUTDIR)$(IMAGE).hex $(BOLO) $(OUTDIR)$(IMAGE)_Prod.hex
	$(ECHO) End of Linking uC
	$(ECHO) Copy Part
	$(CP) *.c $(DEBUGDIR)
	$(CP) *.asm $(DEBUGDIR)
	$(CP) $(OUTDIR)$(IMAGE).out $(DEBUGDIR)
	$(ECHO) End of Copy Part
	$(ECHO) CRC Calculation Part
#calculate CRC
# Format Input_File, Output_File, Start_Checksum_Address, End_Checksum_Address, Checksum_Location
	$(CRCSIGN) $(OUTDIR)$(IMAGE).hex $(OUTDIR)$(IMAGE).hex 0x00008024 0x000BFFFB 0x000BFFFC  
	$(CRCSIGN) $(OUTDIR)$(IMAGE)_Prod.hex $(OUTDIR)$(IMAGE)_Prod.hex 0x00008024 0x000BFFFB 0x000BFFFC  
	$(ECHO) End of CRC Part

$(OBJS)	: $(@:.obj=.c) ../inc/*.h

$(OBJSTARTUP)	: $(@:.obj=.c) ../inc/*.h

clean:
	$(ECHO)
	$(ECHO) Cleaning...
	$(RM) -f $(OBJSTARTUP)
	$(RM) -f $(OBJS)
	$(RM) -f *.bak
	$(RM) -f *.id
	$(RM) -f *.tr
	$(RM) -f *.enum
	$(RM) -f *~
	$(RM) -f $(DEBUGDIR)*.c
	$(RM) -f $(DEBUGDIR)*.asm
	$(RM) -f $(DEBUGDIR)$(IMAGE).map
	$(RM) -f $(DEBUGDIR)$(IMAGE).out
	$(RM) -f $(DEBUGDIR)$(IMAGE).hex
	$(RM) -f $(OUTDIR)$(IMAGE).map
	$(RM) -f $(OUTDIR)$(IMAGE).out
	$(RM) -f $(OUTDIR)$(IMAGE).tr
	$(RM) -f $(OUTDIR)$(IMAGE).hex
	$(RM) -f $(OUTDIR)$(IMAGE)_Prod.hex
	$(ECHO) Finished cleaning



