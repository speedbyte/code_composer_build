# -*- coding: iso-8859-1 -*-
# Copyright (C) 2010 S1nn GmbH & Co. KG
# Author: marc.weber@s1nn (Hajo Butz)

import os
import sys
import string
import glob

rootdir = os.getenv( "rootdir" )
print "rootdir: <%s>" % rootdir

if rootdir == None or rootdir == "":
    print "ERROR: Cannot find build root directory"
    sys.exit(1)
    
# root search directory for makefile
RootPoint = "%s/MOD" % rootdir

#Target Directory for generate files
TargetDirectory = "%s/Temp" % rootdir

#Blacklist directory
BlacklistDirectory = "%s/BUILD/tools" % rootdir

# PClint file definition
PClintIncFile = "SoftwareBuild_IncludeProjectPCLintFile.lnt"
PClintSourceCheckFiles = "SoftwareBuild_SourceFilesToBeCheck.lnt"
PClintBlacklistDir = "PClintBlacklistDirectories.txt"
PClintBlacklistFile = "PClintBlacklistFiles.txt"

# global working directory list
WorkingDirList=[]
MakeFileWorkingDirList=[]

def GetNecessaryInformation():
    # look for directories
    AllList=os.listdir(RootPoint)
    for list in AllList:
        # only directories (no hide)
        if (( os.path.isdir( RootPoint + "/" + list )) & ( list.find(".") == -1 )):
            WorkingDirList.append( list )
    return

def LookForMakefiles():
    print "Search for make files\n"
    for list in WorkingDirList:
        if ( os.path.isfile( RootPoint + "/" + list + "/makefileModul" )):
            MakeFileWorkingDirList.append( list )
    return

def GenerateBuildHelpFiles():
    print "Generate Build Help Files for active modules\n"

    #generate PClint file (include information for headerfiles)
    f = open( TargetDirectory + "/" + PClintIncFile, 'w' )
    GeneratedLine = "// PClint include information for headerfiles"
    f.write( GeneratedLine + '\n') 
    f.write( '\n' )   
    # now collect information
    # first to root
    # os.chdir("..")
    # second get absolute path    
    save_dir = os.getcwd()  
    for list in MakeFileWorkingDirList:
        GeneratedLine = "-i"+ save_dir + "\\MOD\\" + list + "\\inc "
        GeneratedLine2 = "-i"+ save_dir + "\\MOD\\" + list + "\\src "
        f.write(GeneratedLine + '\n')
        f.write(GeneratedLine2 + '\n')
    # System Codecomposer includes
    GeneratedLine = "-i" + save_dir + "\\TOOLS\CCStudio_v3.3\\tms470\\cgtools\\include"
    f.write(GeneratedLine + '\n')
    # last line    
    f.write('\n')
    f.close()    
    
    return
def GenerateBuildSourceCheckFiles():
    print "Generate help file for source files to be check"
    #open blacklist files
    readBlackListDir = open(BlacklistDirectory + "/" + PClintBlacklistDir, 'r')
    readBlackListFile = open(BlacklistDirectory + "/" + PClintBlacklistFile, 'r')
    arrayBlacklistDir = []
    arrayBlacklistFile = []
    #copy file informations
    for line in readBlackListDir:
        arrayBlacklistDir.append(line) 
    for line in readBlackListFile:
        arrayBlacklistFile.append(line)
    #open file to generate files to be check
    g = open( TargetDirectory + "/" + PClintSourceCheckFiles, 'w' )
    GeneratedLine = "// PClint source files to be check"
    g.write( GeneratedLine + '\n') 
    g.write( '\n' )
    #print "working dir %s" % MakeFileWorkingDirList
    
    #start to search *.c files
    for list in MakeFileWorkingDirList:
        checkDir = True
        for line in arrayBlacklistDir:
            #print type(os.path.basename(list)) , os.path.basename(list), (line == os.path.basename(list)), len(os.path.basename(list))
            #print type(line) , line, (line == os.path.basename(list)), len(line)
            #check if directory is not in Blacklist directory
            if (line.strip() == os.path.basename(list)):
                checkDir = False
        if (checkDir):
            #print "valid directory " + list
            directoryPath = os.path.join("MOD", list, "src")
            files = glob.glob(directoryPath + '/*.c')
            for i in files:
                checkFile = True
                for line in arrayBlacklistFile:
                    #check if files not in blacklist file
                    if(line.strip() == os.path.basename(i)):
                        checkFile = False
                if(checkFile):
                    #write filesname in helper file
                    g.write(i + "\n")

    # last line    
    g.write('\n')
    
    return

# ###########
# Main part
# ###########
if __name__ == "__main__":
    # RunScript
    print "GenerateBuildHelpFiles\n"
    GetNecessaryInformation()
    LookForMakefiles()
    GenerateBuildHelpFiles()
    GenerateBuildSourceCheckFiles()
    sys.exit(0)

