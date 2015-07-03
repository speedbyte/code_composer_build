# -*- coding: iso-8859-1 -*-
# Copyright (C) 2008  S1nn GmbH
# Author: hajo.butz@s1nn (Hajo Butz)

import os
import sys
import string
import shutil
from IncludeHeaderFiles import ExternalHeaderFilesList 

rootdir = os.getenv( "rootdir" )
print "rootdir: <%s>" % rootdir

if rootdir == None or rootdir == "":
    print "ERROR: Cannot find build root directory"
    sys.exit(1)

builddir = rootdir + "\\BUILD"

# root search directory for makefile
RootPoint = "%s\\MOD" % rootdir
print "RootPoint: <%s>" % RootPoint

#Target Directory for generate files
TargetDirectory = "%s\\TEMP" % rootdir
print "TargetDirectory: <%s>" % TargetDirectory

#makeoptsinc file definition
MakeOptsIncFile = "makeoptsinc"

#makeoptsinc file definition
MakeFileForMOD_RekursiveSubList = "MOD_RekursiveSubList"

#uCprojectopts file definition
MakeFileForuCprojectopts = "uCprojectopts"

#search the prozessor ID
ucOptsFile = "%s\\BUILD\\SYS\\ucopts" % rootdir
print "ucopts: <%s>" % ucOptsFile

# global working directory list
WorkingDirList=[]
MakeFileWorkingDirList=[]

def SearchDefinitionAndCopyFile():
    #open file
    ucopts = open( ucOptsFile, 'r' )
    #read lines and convert to string
    for lines in ucopts:
        #search define in ucopts
        index = lines.find( "-d PROCESSOR" )
        if ( index != -1 ):
            text = lines
            if ( '#' in  text[0:index]):
                continue
            else:
                #print "Processor Found %sat index %d " %  (lines,index)
                break
    #check if PROCESSOR_55BA defined or undefined
    #and copied one of both linker files and renamed the file
    
    if ( index == -1 ) :
        ucopts.close()
        return -1
    
    if text.find("TMS470R1VF45AA") > -1:
        print "Selected Processor: TMS470R1VF45AA"
        shutil.copyfile( builddir + "\\MOD\\VF45AA_link32.cmd", rootdir + "\\Temp\\linker32.cmd" ) 
        shutil.copyfile( builddir + "\\MOD\\VF45AA_roms32.cmf", rootdir + "\\Temp\\roms32.cmf" )
    elif text.find( "TMS470R1VF55BA") > -1:
        print "Selected Processor: TMS470R1VF55BA"
        shutil.copyfile( builddir + "\\MOD\\VF55B_link32.cmd", rootdir + "\\Temp\\linker32.cmd" )
        shutil.copyfile( builddir + "\\MOD\\VF55B_roms32.cmf", rootdir + "\\Temp\\roms32.cmf" )
    elif text.find( "TMS470R1SF55BA") > -1:
        print "Selected Processor: TMS470R1SF55BA"
        shutil.copyfile( builddir + "\\MOD\\SF55B_link32.cmd", rootdir + "\\Temp\\linker32.cmd" )
        shutil.copyfile( builddir + "\\MOD\\SF55B_roms32.cmf", rootdir + "\\Temp\\roms32.cmf" )
    elif text.find( "TMS570LS20216") > -1:
        print "Selected Processor: TMS570LS20216"
        shutil.copyfile( builddir + "\\MOD\\TMS570LS20216_sys_link.cmd", rootdir + "\\Temp\\linker32.cmd" )
        shutil.copyfile( builddir + "\\MOD\\TMS570LS20216_roms32.cmf", rootdir + "\\Temp\\roms32.cmf" )
    elif text.find( "MPC5603B") > -1:
        print "Selected Processor: MPC5603B"
        shutil.copyfile( builddir + "\\MOD\\MPC5603B_sys_link.lcf", rootdir + "\\Temp\\linker32.lcf" )
    else:
        ucopts.close()
        return -1
    return

def GetNecessaryInformation():
    # look for directories
    AllList=os.listdir(RootPoint)
    for list in AllList:
        # only directories (no hide)
        if ((os.path.isdir( RootPoint + "/" + list )) & ( list.find(".") == -1 )):
            WorkingDirList.append(list)
    #print AllList
    #print WorkingDirList
    return

def LookForMakefiles():
    print "Search for make files\n"
    for list in WorkingDirList:
        if (os.path.isfile( RootPoint + "/" + list + "/makefile" )):
            MakeFileWorkingDirList.append(list)
    print MakeFileWorkingDirList
    return

def GenerateBuildHelpFiles():
    print "GenerateHelpFiles\n"
    #generate makeoptsinc file (include information for headerfiles)
    f = open( TargetDirectory + "/" + MakeOptsIncFile, 'w' )
    GeneratedLine = "# include informattion for headerfiles"
    f.write( GeneratedLine + '\n')
    GeneratedLine = "CINC = \\"
    f.write( GeneratedLine + '\n')
    # now collect information
    for list in MakeFileWorkingDirList:
        GeneratedLine = " -I$(CONTROLLERDIR)/" + list + "/inc\\"
        f.write(GeneratedLine + '\n')
        GeneratedLine = " -I$(CONTROLLERDIR)/" + list + "/src\\"
        f.write(GeneratedLine + '\n')
    # last line
    for list in ExternalHeaderFilesList:
        GeneratedLine = " -I$(CONTROLLERDIR)/" + list + "/inc\\"
        f.write(GeneratedLine + '\n')
        GeneratedLine = " -I$(CONTROLLERDIR)/" + list + "/src\\"
        f.write(GeneratedLine + '\n')
    f.write( '\n' )
    f.close()
    
    #generate MOD_RekursiveSubList(Rekursiv File list for subdirectories file)
    f = open( TargetDirectory + "/" + MakeFileForMOD_RekursiveSubList, 'w' )
    GeneratedLine = "# Define the subdirectories in which to run <make>"
    f.write( GeneratedLine + '\n' )
    GeneratedLine = "SUBDIRS = \\"
    f.write( GeneratedLine + '\n' )
    # now collect information but not SYS_MAIN need special handling- has to be last step to be compile and link
    for list in MakeFileWorkingDirList:
        if ( list.find( "SYS_MAIN" ) == -1 ):
            GeneratedLine = " " + list + "\\"
            f.write( GeneratedLine + '\n' )
    f.write( "SYS_MAIN\\" )
    # last line
    f.write( '\n' )
    f.close()
    
    #generate uCprojectopts file (Rekursiv File list for subdirectories file)
    f = open( TargetDirectory + "/" + MakeFileForuCprojectopts, 'w' )
    GeneratedLine = "# uC Project options"
    f.write( GeneratedLine + '\n\n' )
    # Part 1 - Libary build
    GeneratedLine = "# Define Libraries"
    f.write( GeneratedLine + '\n' )
    # now collect information but not SYS_MAIN need special handling
    for list in MakeFileWorkingDirList:
        if ( list.find( "SYS_MAIN" ) == -1 ):
            GeneratedLine = "LIB_" + list +  " = " + list +".lib"
            f.write( GeneratedLine + '\n' )
    # Part 2 - Libary build
    GeneratedLine = "# Define Libraries Position"
    f.write( GeneratedLine + '\n' )
    for list in MakeFileWorkingDirList:
        if ( list.find( "SYS_MAIN" ) == -1 ):
            GeneratedLine = list + "_POS = ../../" + list +  "/src/$(LIB_" + list + ")"
            f.write( GeneratedLine + '\n' )
    # Part 3 - Libary build
    GeneratedLine = "# Define Libraries for linking"
    f.write( GeneratedLine + '\n' )
    GeneratedLine = "LIBS =\\"
    f.write( GeneratedLine + '\n' )
    for list in MakeFileWorkingDirList:
        if ( list.find( "SYS_MAIN" ) == -1 ):
            GeneratedLine = " $(" + list +"_POS)\\"
            f.write( GeneratedLine + '\n' )
    
    # last line
    f.write('\n')
    f.close()


    return


# ###########
# Main part
# ###########
if __name__ == "__main__":
    # RunScript
    print "GenerateBuildHelpFiles\n"
    retVal = SearchDefinitionAndCopyFile()
    if ( retVal == -1 ):
        try:
            raise NameError(" !!!!!! NO PROCESSOR ENTRY FOUND IN UCOPTS !!!!!! ")
        except NameError:
            print "An exception flew by"
            raise
            
    else:
        GetNecessaryInformation()
        LookForMakefiles()
        GenerateBuildHelpFiles()
        sys.exit(0)

