# -*- coding: iso-8859-1 -*-
# Copyright (C) 2010  S1nn GmbH
# Author: Oliver.Braun@s1nn (Oliver Braun, base taken from Hayo Butz ;-)...)

import os
import os.path
import sys
import string
import shutil
import glob


#Target Directory for generate files
TargetDirectory = "./TEMP"

Root = "."

#makeoptsinc file definition
MakeOptsIncFile = "makeoptsinc"
#makeoptsinc file definition
MakeFileForMOD_RekursiveSubList = "MOD_RekursiveSubList"
#uCprojectopts file definition
MakeFileForuCprojectopts = "uCprojectopts"
#search the prozessor ID
SearchIdOfProzessor = "ucopts"

# global working directory list
WorkingDirList=[]
MakeFileWorkingDirList=[]


def GetNecessaryInformation():
    # look for directories
    try:
        AllList=os.listdir(".\\MOD")
    except:
        RootPoint = ".\\"
        AllList=os.listdir(RootPoint)
    for list in AllList:
        # only directories (no hide)
        if ((os.path.isdir(".\\MOD" +"\\"+list)) & (list.find(".") == -1)):
            WorkingDirList.append(list)
#    print WorkingDirList

    return

def LookForSourceDirs():
#    print "Search for Source Dirs\n"
    for list in WorkingDirList:
        pathname=".\\MOD"+"\\"+list+"\\src"
#        print pathname
        if (os.path.isdir(pathname)):
            MakeFileWorkingDirList.append(pathname)
#    print MakeFileWorkingDirList
    return

def LookForSourceFilesGenerateFileID():
    print "Search for source files and generate FileID\n"
    AllFoundFiles=[]
    FileID=0x000000
    print "Number of directories to parse" , len(MakeFileWorkingDirList)
    if (len(MakeFileWorkingDirList) != 0):
        pass
    else:
        MakeFileWorkingDirList.append(".\\src") # This is when someone clicks the Make batch from the Module level.
    for list in MakeFileWorkingDirList:
        FileLocationPatternC = list+"\\*.c"
        FoundFiles=glob.glob(FileLocationPatternC)
        for list in FoundFiles:
            FileID=FileID+0x10000
            (path, filename) = os.path.split(list)
            (name, extension) = os.path.splitext(filename)
            id_filename = path+ "\\" + name + ".id"
#            print "CWD: %s, File: %s" % (os.path.abspath(os.curdir), id_filename)
            output_id_file = open(id_filename, "wb+")
            output_id_file.write("#define __C_FILE_ID 0x%X\n" % FileID)
            output_id_file.close()
        AllFoundFiles.append(FoundFiles)
#    print AllFoundFiles
    return



# ###########
# Main part
# ###########
if __name__ == "__main__":
    # RunScript
    GetNecessaryInformation()
    LookForSourceDirs()
    LookForSourceFilesGenerateFileID()
    sys.exit(0)

