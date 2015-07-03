""" 


    $Date: 2010-02-25 11:22:22 +0100 (Do, 25 Feb 2010) $
    $Rev: 11267 $
    $Author: agrawal $
    $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/TOOLS/Python_scripts/trunk/EEPROM_Script/EepromLayoutSettingsFile.py $

    Copyright (c) 2007 S1nn GmbH & Co. KG.
    All Rights Reserved.

"""

_svn_id = "$Id$"
_svn_rev = "$Rev: 11267 $"
__version__ = _svn_rev[6:-2]

import sys
import os


def generateTraceConfigFile(entireFile):
    print "Search for source files and generate FileID\n"
    fd_generated_file = open("../../../MOD/CONFIG_PLATFORM/inc/TRACE_CONFIG_GENERATED.h", "wb")
    fd_generated_file.write("/******************************************************************************/\r\n")
    fd_generated_file.write("/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/\r\n")
    fd_generated_file.write("/***************************  All Rights Reserved  ****************************/\r\n")
    fd_generated_file.write("/******************************************************************************/\r\n")    

    fd_generated_file.write("#ifndef _TRACE_CONFIG_GENERATED_H\r\n")    
    fd_generated_file.write("#define _TRACE_CONFIG_GENERATED_H\r\n")

    for line in entireFile:
        #print line
        if "#define" in line and " _TRACE_CONFIG_H" not in line:
            line_splitted = line.split(' ')
            print line_splitted[1]
            fd_generated_file.write("#if (1 == " + line_splitted[1] + ")\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)\r\n")                        
            fd_generated_file.write("    #define " + line_splitted[1] + "_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)\r\n")                                                        

            fd_generated_file.write("#else\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_TEXT(moduleID, format)                                         {}\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_VALUE1(moduleID, format, value)                                {}\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_VALUE2(moduleID, format, value1, value2)                       {}\r\n")                        
            fd_generated_file.write("    #define " + line_splitted[1] + "_VALUE4(moduleID, format, value1, value2, value3, value4)       {}\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_STRING(moduleID, format, string)                               {}\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_STATE1(moduleID, format, state)                                {}\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_STATE2(moduleID, format, state1, state2)                       {}\r\n")
            fd_generated_file.write("    #define " + line_splitted[1] + "_STATE4(moduleID, format, state1, state2, state3, state4)       {}\r\n")                                                        

            fd_generated_file.write("#endif\r\n")

    fd_generated_file.write("#endif\r\n")

    return




if __name__ == '__main__':

    if len(sys.argv) == 2:
        print sys.argv[1] 
    else:
        print os.getcwd()
        fd = open("../../../MOD/CONFIG_PLATFORM/inc/TRACE_CONFIG.h", "rb")
        entireFile = fd.readlines()
    generateTraceConfigFile(entireFile)
    
