HdrCommentTxt = \
"""
    @modul: GenerateTraceID.py
    @brief: Generate TraceIDs for IOC TraceClient
    @author: O. Braun
    @version: $Id$

    Copyright (c) 2010 S1nn GmbH & Co. KG.
    All Rights Reserved.
"""

#------------------------------------
UsageTxt1=\
"""
   TraceID Usage:
   call: python GenerateTraceID.py [-Cfile -idfile -trfile]
"""

## import os # e.f. for os.copy
import sys     # for e.g. sys.exit
import os      # for os.getcwd()
import glob

#----------------------------------------------------------------------
#

# global working directory list
WorkingDirList = []
MakeFileWorkingDirList = []


def dec2hex(n):
    return "%X" % n

def mainApplParseCmdLine():
    # usage message if no arguments passed:
    development = False
    if len(sys.argv) == 1:
        print(UsageTxt1)
    else:
#        print"  Parsing arguments ..."
        for aLstidx in range(len(sys.argv)):
            print"      ", sys.argv[aLstidx]


#    SrcFile = open(sys.argv[1], 'rb')
    removeLine = False
    skipThisEnum = False
    useListEnumValue = 0
    ctrl_Parameter = sys.argv[1]
    if(ctrl_Parameter == "file"):
        # Get File *.c
        C_input_file = file(sys.argv[2], 'rb')
        
        # Get File *.id
        ID_input_file = file(sys.argv[3], 'rb')

        # Open output file *.tr
        Tr_output_file = open(sys.argv[4], "wb+")
#        print "Generating trace file:", Tr_output_file.name
        
        Enum_output_file = open("Enums_in_directory.enum", "wb")
        Enum_output_file_1 = open("Enums_in_directory_TrFile.enum", "wb")

        # This would give location of the current directory
        # import os
        current_dir = os.getcwd()
        # print "Current directory:",current_dir
        
        

        last_current_dir = current_dir

        temp = current_dir[:-3] + 'inc\\'
        #print temp
        listofHFiles = os.listdir(temp)
        #print listofHFiles
        
        startpoint = Enum_output_file.tell()
        startpoint_1 = Enum_output_file_1.tell()
        #print "START POINT ", startpoint
        parse_for_enums = True
        
        for element in range(len(listofHFiles)):
            if (element == len(listofHFiles)):
                parse_for_enums = False
            if ('.h' in listofHFiles[element]):
                H_input_file = file(temp + listofHFiles[element], 'rb')
                #print H_input_file
                list_enum = []
                list_enum_value = []
                linecounter_enum = 0
                enum_found = "no_enum"
                for line in H_input_file:
                    linecounter_enum += 1
                    endComment = 0
                    if (line.find('typedef enum') != -1):
                        #print "found a enum"
                        enum_found = "found_enum"
                        #Enum_output_file.write("found a enum at linuenumber at in file " + str(linecounter_enum)+ H_input_file.name + '\n')
                        continue
                    if (enum_found == "found_enum"):
                        line = str(line)
                        comment_double_slash = line.find('//')
                        if (comment_double_slash != -1):
                            line = line[:comment_double_slash]
                        comment_start = line.find('/*')
                        comment_end = line.find('*/')
                        if (comment_start != -1 and comment_end == -1):
                            removeLine = True
                            continue
                        elif (removeLine == True and comment_end == -1):
                            continue
                        elif (removeLine == True and comment_end != -1):
                            removeLine = False
                            continue
                        compilerSwitchstart = line.find('#if')
                        compilerSwitchend = line.find('#endif')
                        if (compilerSwitchstart != -1 or compilerSwitchend != -1):
                            skipThisEnum = True
                            continue
                        comma = line.find(',')
                        if (comma != -1):
                            line = line[:comma + 1]
                            line = line.strip(' ')
                            comment_end = line.find('*/')
                            equalto = line.find('=')                        
                            if ((comment_end < comma) and (comment_end != -1)):
                                endComment = comment_end + 3
                            line_value = line[equalto:comma]
                            line = line[endComment:equalto]
                            if (equalto != -1):
                                line_value = line_value.strip(' ')
                                line_value = line_value.strip('\r\n')
                                line_value = line_value.strip('\t')
                                line_value = line_value.strip(',')
                                line_value = line_value.strip('=')
                                line_value = line_value.strip('-')
                                #line_int_value = int(line_value,16 )
                                #line_value = str(line_int_value)
                            line = line.strip(' ')
                            line = line.strip('\r\n')
                            line = line.strip('\t')
                            if (line != ''):
                                list_enum.append(line)
                                if (equalto != -1):
                                    if ( '0x' in line_value ):
                                        try:
                                            useListEnumValue = int(line_value,16)
                                        except:
                                            skipThisEnum = True
                                    else:
                                        try:
                                            useListEnumValue = int(line_value)
                                        except:
                                            skipThisEnum = True
                                    list_enum_value.append(str(useListEnumValue))
                                    useListEnumValue = useListEnumValue + 1                                    
                                else:
                                    list_enum_value.append(str(useListEnumValue))
                                    useListEnumValue = useListEnumValue + 1 
                                if (development == True):
                                    print line
                                    print line_value
                        elif ((line.find('}') == -1) and (line.find('{') == -1) and (line != '')): 
                            line = line.strip(' ')
                            comment_end = line.find('*/')
                            equalto = line.find('=')                        
                            if ((comment_end != -1)):
                                endComment = comment_end + 3
                            line_value = line[equalto:]
                            line = line[endComment:]
                            if (equalto != -1):
                                line = line[:equalto]
                                line_value = line_value.strip(' ')
                                line_value = line_value.strip('\r\n')
                                line_value = line_value.strip('\t')
                                line_value = line_value.strip(',')
                                line_value = line_value.strip('=')
                                line_value = line_value.strip('-')
                                #line_int_value = int(line_value,16 )
                                #line_value = str(line_int_value)
                            line = line.strip(' ')
                            line = line.strip('\r\n')
                            line = line.strip('\t')
                            if (line != ''):
                                list_enum.append(line)
                                if (equalto != -1):
                                    if ( '0x' in line_value ):
                                        try:
                                            useListEnumValue = int(line_value,16)
                                        except:
                                            skipThisEnum = True
                                    else:
                                        try:
                                            useListEnumValue = int(line_value)
                                        except:
                                            skipThisEnum = True
                                    list_enum_value.append(str(useListEnumValue))
                                    useListEnumValue = useListEnumValue + 1                                    
                                else:
                                    list_enum_value.append(str(useListEnumValue))
                                    useListEnumValue = useListEnumValue + 1 
                                if (development == True):
                                    print line
                                    print line_value
                    if (line.find('}') != -1):
                        enum_found = 'found_end_of_enum'
                        if (enum_found == "found_end_of_enum"):
                            #Enum_output_file.write(str(list_enum))
                            semicolon = line.find(';')
                            if (semicolon != -1):
                                enum_word = str(line[:semicolon])
                                enum_word = enum_word.strip('}')
                                enum_word = enum_word.strip(' ')
                                if (skipThisEnum == True):
                                    list_enum = ['UNABLE_TO_SHOW_ENUM']
                                    list_enum_value = ['12345'] 
                                for enum_value in range(len(list_enum)):
                                    startpoint = Enum_output_file.tell()
                                    startpoint_1 = Enum_output_file_1.tell()
                                    Enum_output_file.write(205*' ')
                                    Enum_output_file_1.write(205*' ')
                                    Enum_output_file.seek(startpoint)
                                    Enum_output_file_1.seek(startpoint_1)
                                    Enum_output_file.seek(0,1)
                                    Enum_output_file_1.seek(0,1)
                                    Enum_output_file.write("DEBUG_STATE_ENUM_NAME='") ## debug
                                    Enum_output_file_1.write("#ENUM  '")  ##enum
                                    enum_word = enum_word.lstrip(' ')
                                    Enum_output_file.write(enum_word)
                                    Enum_output_file_1.write(enum_word)
                                    Enum_output_file.seek(startpoint)
                                    Enum_output_file_1.seek(startpoint_1)
                                    Enum_output_file.seek(60,1)
                                    Enum_output_file_1.seek(60,1)
                                    Enum_output_file.write("'DEBUG_STATE_NAME='")   ## debug
                                    Enum_output_file_1.write("'")   ## enum
                                    if ( len(list_enum[enum_value]) > 82 ):
                                        list_enum[enum_value] = list_enum[enum_value].lstrip(' ')
                                        Enum_output_file.write(list_enum[enum_value])
                                        Enum_output_file_1.write(list_enum[enum_value])
                                    else:
                                        list_enum[enum_value] = list_enum[enum_value].lstrip(' ')
                                        Enum_output_file.write(list_enum[enum_value])
                                        Enum_output_file_1.write(list_enum[enum_value])
                                        Enum_output_file.seek(startpoint)
                                        Enum_output_file_1.seek(startpoint_1)
                                        Enum_output_file.seek(150,1)
                                        Enum_output_file_1.seek(150,1)
                                    Enum_output_file.write("'DEBUG_STATE_NUMBER='")   ## debug
                                    Enum_output_file_1.write("'")  ##enum
                                    Enum_output_file.write(list_enum_value[enum_value])
                                    Enum_output_file_1.write(list_enum_value[enum_value])
                                    Enum_output_file.seek(startpoint)
                                    Enum_output_file_1.seek(startpoint_1)
                                    Enum_output_file.seek(180,1)
                                    Enum_output_file_1.seek(180,1)
                                    Enum_output_file.write("'\n"  ) ##debug
                                    Enum_output_file_1.write("'ABCD   'ABCD.c  '123   \n"  ) ##enum
                                enum_found = "no_enum"
                                #print enum_word
                                list_enum = []
                                list_enum_value = []
                                skipThisEnum = False
                                useListEnumValue = 0
                            else:
                                continue                            
                    if (enum_found == "found_end_of_enum"):
                        semicolon = line.find(';')                        
                        if (semicolon != -1):
                            enum_word = str(line[:semicolon + 1])
                            if (skipThisEnum == True):
                                list_enum = ['UNABLE_TO_SHOW_ENUM']
                                list_enum_value = ['12345'] 
                            for enum_value in range(len(list_enum)):
                                startpoint = Enum_output_file.tell()
                                Enum_output_file.write(205 * ' ')
                                Enum_output_file_1.write(205 * ' ')
                                Enum_output_file.seek(startpoint)
                                Enum_output_file_1.seek(startpoint)
                                Enum_output_file.seek(0, 1)
                                Enum_output_file_1.seek(0, 1)
                                Enum_output_file.write("DEBUG_STATE_ENUM_NAME='") ## debug
                                Enum_output_file_1.write("#ENUM  '")  ##enum
                                enum_word = enum_word.lstrip(' ')
                                Enum_output_file.write(enum_word)
                                Enum_output_file_1.write(enum_word)
                                Enum_output_file.seek(startpoint)
                                Enum_output_file_1.seek(startpoint)
                                Enum_output_file.seek(60, 1)
                                Enum_output_file_1.seek(60, 1)
                                Enum_output_file.write("'DEBUG_STATE_NAME='")   ## debug
                                Enum_output_file_1.write("'")   ## enum
                                if (len(list_enum[enum_value]) > 82):
                                    Enum_output_file.write(list_enum[enum_value])
                                    Enum_output_file_1.write(list_enum[enum_value])
                                else:
                                    Enum_output_file.write(list_enum[enum_value])
                                    Enum_output_file_1.write(list_enum[enum_value])
                                    Enum_output_file.seek(startpoint)
                                    Enum_output_file_1.seek(startpoint)
                                    Enum_output_file.seek(150, 1)
                                    Enum_output_file_1.seek(150, 1)
                                Enum_output_file.write("'DEBUG_STATE_NUMBER='")   ## debug
                                Enum_output_file_1.write("'")  ##enum
                                Enum_output_file.write(list_enum_value[enum_value])
                                Enum_output_file_1.write(list_enum_value[enum_value])
                                Enum_output_file.seek(startpoint)
                                Enum_output_file_1.seek(startpoint)
                                Enum_output_file.seek(180, 1)
                                Enum_output_file_1.seek(180, 1)
                                Enum_output_file.write("'\n") ##debug
                                Enum_output_file_1.write("' ABCD   'ABCD.c  '123   \n") ##enum                                
                            enum_found = "no_enum"
                            list_enum = []
                            skipThisEnum = False
                            useListEnumValue = 0
                        else:
                            continue                            
                            

# DEBUG_STATE_ENUM_NAME='CSHDL_ENUM_ONOFF_STATES' DEBUG_STATE_NAME='ON_OFF_INIT                               '    DEBUG_STATE_NUMBER='0'
##ENUM  'CAN_RESPONSES_PHONE_ID 'INVALID_PHONE_RSP           '0 '\MOD\MOD_APP_CAN\src 'APP_CAN_IL_PHONE.c '185
        # Searching for Traces
        linecounter = 0
        TraceLine = ''
        TraceText = ''
        TraceDebuglevel = ''
        FileID_String = ''
        FileID_Number = 0
        TraceID_String = ''
        TraceID_Number = 0

        for line in ID_input_file:
            if (line.find("#define __C_FILE_ID") != -1):
                FileID_String = (line[20:29])
#        print FileID_String
        FileID_Number = int(FileID_String, 16);
        print ("       FileID 0x%X" % FileID_Number)

        for line in C_input_file:
            if (line.find('typedef enum') != -1):
                print "found a enum"
            linecounter += 1
            #check if
            StateName = 'unknown'
            TraceAPI = 'unknown'
            AnyDebugTrace = 0
            AnyState = 0
            if (FileID_Number > 0):
                if(line.find("DEBUG_TEXT") != -1):
                    TraceAPI = 'DEBUG_TEXT   ' + '\''
                    TraceID_Number = FileID_Number + linecounter
                    AnyDebugTrace = 1
                elif(line.find("DEBUG_VALUE1") != -1):
                    TraceAPI = 'DEBUG_VALUE1 ' + '\''
                    TraceID_Number = FileID_Number + linecounter
                    AnyDebugTrace = 2
                elif(line.find("DEBUG_VALUE2") != -1):
                    TraceAPI = 'DEBUG_VALUE2 ' + '\''
                    TraceID_Number = FileID_Number + linecounter
                    AnyDebugTrace = 3
                elif(line.find("DEBUG_VALUE4") != -1):
                    TraceAPI = 'DEBUG_VALUE4 ' + '\''
                    TraceID_Number = FileID_Number + linecounter
                    AnyDebugTrace = 4
                elif(line.find("DEBUG_STATE1") != -1):
                    TraceAPI = 'DEBUG_STATE1 ' + '\''
                    TraceID_Number = FileID_Number + linecounter
                    AnyDebugTrace = 5
                elif(line.find("DEBUG_STATE2") != -1):
                    TraceAPI = 'DEBUG_STATE2 ' + '\''
                    TraceID_Number = FileID_Number + linecounter
                    AnyDebugTrace = 6
                elif(line.find("DEBUG_STATE_ENUM_NAME") != -1):
                     if(line.find("DEBUG_STATE_NAME") != -1):
                        if(line.find("DEBUG_STATE_NUMBER") != -1):
                          #found all we need
                          indexleftenumname = line.find("'", 0, len(line))
                          indexleftenumname += 1
                          indexrightenumname = line.find("'", indexleftenumname, len(line))
                          StateEnumName = (line[indexleftenumname:indexrightenumname])

                          indexleftstatename = line.find("'", indexrightenumname + 1, len(line))
                          indexleftstatename += 1
                          indexrightstatename = line.find("'", indexleftstatename, len(line))
                          StateName = (line[indexleftstatename:indexrightstatename])

                          indexleftstatenumber = line.find("'", indexrightstatename + 1, len(line))
                          indexleftstatenumber += 1
                          indexrightstatenumber = line.find("'", indexleftstatenumber, len(line))
                          StateNumber = (line[indexleftstatenumber:indexrightstatenumber])

                          AnyDebugTrace = 0
                          AnyState = 1

                          #print 'EnumName=',StateEnumName,  'StateName=',StateName,  'StateNumber=',StateNumber


                if (AnyDebugTrace != 0):
                    #parse for debuglevel
                    if(line.find("_SWI_ERR") != -1):
                      TraceDebuglevel = 'DBG_LEV_ERR ' + '\''
                    elif(line.find("_SWI_WNG") != -1):
                      TraceDebuglevel = 'DBG_LEV_WNG ' + '\''
                    elif(line.find("_SWI_TRC") != -1):
                      TraceDebuglevel = 'DBG_LEV_TRC ' + '\''
                    elif(line.find("_SWI_DBG") != -1):
                      TraceDebuglevel = 'DBG_LEV_DBG ' + '\''
                    elif(line.find("_SWI_HMI") != -1):
                      TraceDebuglevel = 'DBG_LEV_HMI ' + '\''


                    #parse for module
                    modindexleft = line.find("MOD_", 0, len(line))
                    modindexright = line.find(",", modindexleft, len(line))
                    TraceModule = (line[modindexleft:modindexright]) + ' \''


                    #parse for text
                    quoteindexleft = line.find("\"", 0, len(line))
                    quoteindexright = line.rfind("\"", 0, len(line))
                    TraceText = (line[quoteindexleft:quoteindexright + 1]) + ' \''

                    #parse folder for MOD -> projektspecific for UHV ?
                    modindex = current_dir.find("\\MOD\\", 0, len(current_dir))
                    TraceFolder = current_dir[modindex:len(current_dir)]


                    #output one line of *.tr file
                    Tr_output_file.write("#TRACE_ID '");

#gibt dezimal aus                    Tr_output_file.write(str(TraceID_Number));

                    TraceID_tmp = dec2hex(TraceID_Number)
                    TraceID_String = "0x" + TraceID_tmp
#                    print TraceID_String


                    Tr_output_file.write(TraceID_String);
                    Tr_output_file.write(" '");
                    Tr_output_file.write(TraceAPI);
                    Tr_output_file.write(TraceDebuglevel);
                    Tr_output_file.write(TraceModule);
                    Tr_output_file.write(TraceText);
                    Tr_output_file.write(TraceFolder);
                    Tr_output_file.write(" '");
                    Tr_output_file.write(sys.argv[2]);
                    Tr_output_file.write(" '");
                    Tr_output_file.write(str(linecounter));

                    Tr_output_file.write(" \n");


                if (AnyState != 0):
                    #parse for debuglevel

                    #parse for module
 #                   modindexleft=line.find("MOD_",0,len(line))
  #                  modindexright=line.find(",",modindexleft,len(line))
   #                 TraceModule=(line[modindexleft:modindexright])+' \''


                    #parse for text
                    quoteindexleft = line.find("}", 0, len(line))
                    quoteindexright = line.rfind(";", 0, len(line))
                    TraceText = (line[quoteindexleft:quoteindexright + 1]) + ' \''

                    #parse folder for MOD -> projektspecific for UHV ?
                    modindex = current_dir.find("\\MOD\\", 0, len(current_dir))
                    TraceFolder = current_dir[modindex:len(current_dir)]


                    #output one line of *.tr file
                    Tr_output_file.write("#ENUM ");

#gibt dezimal     aus                    Tr_output_file.write(str(TraceID_Number));

  #                  TraceID_tmp = dec2hex(TraceID_Number)
 #                   TraceID_String = "0x"+TraceID_tmp
#                        print TraceID_String


#                    Tr_output_file.write(TraceID_String);
                    Tr_output_file.write(" '");
                    Tr_output_file.write(StateEnumName);
                    Tr_output_file.write(" '");
                    Tr_output_file.write(StateName);
                    Tr_output_file.write(" '");
                    Tr_output_file.write(StateNumber);
                    Tr_output_file.write(" '");
#                    Tr_output_file.write(TraceDebuglevel);
#                    Tr_output_file.write(TraceModule);
#                    Tr_output_file.write(TraceText);
                    Tr_output_file.write(TraceFolder);
                    Tr_output_file.write(" '");
                    Tr_output_file.write(sys.argv[2]);
                    Tr_output_file.write(" '");
                    Tr_output_file.write(str(linecounter));

                    Tr_output_file.write(" \n");



    else:
        if(ctrl_Parameter == "folder"):
            #search read actuell Softwareversion
            # Open output file
            # Get MOD directory
            mod_dir = sys.argv[2]
#===============================================================================
            project_name_dir = os.getcwd().split('\\')
            project_name = project_name_dir[len(project_name_dir) - 4]
            addDirectory = False
            try:
                os.system("mkdir C:\AllTraceFiles")
                Tr_output_file_dev = open("C:/AllTraceFiles/" + project_name + "_old.tr", "wb+")
                print "Generate complete Trace file:", Tr_output_file_dev.name

                Enum_output_file_dev = open("C:/AllTraceFiles/" + project_name + ".enum", "wb+")
                print "Generate complete enum file:", Enum_output_file_dev.name
	
                Gesamt_output_file_dev = open("C:/AllTraceFiles/" + project_name + ".tr", "wb+")
                print "Generate complete enum file:", Gesamt_output_file_dev.name
                addDirectory = True
               
            except OsError:
                print "Cannot create directory"

            except:
                print "Cannot create directory"

            Tr_output_file = open(sys.argv[(len(sys.argv) - 2)], "wb+")
            Tr_output_file.close()
            TempFile = Tr_output_file.name.replace('.tr', '_old.tr')
            Tr_output_file_old = open(TempFile, "wb+")
            print "Generate complete Trace file:", Tr_output_file.name
 
            Enum_output_file = open(sys.argv[(len(sys.argv) - 1)], "wb+")
            print "Generate complete enum file:", Enum_output_file.name

            #Gesamt_output_file_name = Tr_output_file.name.replace('.tr', '_gesamt.tr')
            Gesamt_output_file = open(sys.argv[(len(sys.argv) - 2)], "wb+")
            print "Generate complete enum file:", Gesamt_output_file.name

            # look for directories
            current_dir = os.getcwd()
#            print  current_dir

            AllList = os.listdir(mod_dir)
            for list in AllList:
                # only directories (no hide)
                if ((os.path.isdir(mod_dir + "\\" + list)) & (list.find(".") == -1)):
                    WorkingDirList.append(list)
#            print WorkingDirList


#            print "Search for Source Dirs\n"
            for list in WorkingDirList:
                pathname = mod_dir + "\\" + list + "\\src"
#                print pathname
                if (os.path.isdir(pathname)):
                    MakeFileWorkingDirList.append(pathname)
#           print MakeFileWorkingDirList


#            print "Search for each Tr-File\n"
            AllFoundFiles = []
            i = 0
            for list in MakeFileWorkingDirList:
                FileLocationPatternTr = list + "\\*.tr"
                FoundFiles = glob.glob(FileLocationPatternTr)
                for list in FoundFiles:
                    i = i + 1
#                    print i, list

                    (path, filename) = os.path.split(list)
                    (name, extension) = os.path.splitext(filename)
                    input_id_filename = path + "\\" + name + ".tr"
#                    print "CWD: %s, File: %s" % (os.path.abspath(os.curdir), input_id_filename)

                    # Open input file
                    fo = open(input_id_filename, "rb")
                    inhalt = fo.read() # Lese die gesamte Datei

                    Tr_output_file_old.write(inhalt)
                    if (addDirectory == True):
                        Tr_output_file_dev.write(inhalt)

                    # Close opend file
                    fo.close()



                AllFoundFiles.append(FoundFiles)
#            print AllFoundFiles


#            print "Search for each ENUM-File\n"
            AllFoundFiles = []
            i = 0
            for list in MakeFileWorkingDirList:
                FileLocationPatternTr = list + "\\*.enum"
                FoundFiles = glob.glob(FileLocationPatternTr)
                for list in FoundFiles:
                    i = i + 1
#                    print i, list

                    (path, filename) = os.path.split(list)
                    (name, extension) = os.path.splitext(filename)
                    input_id_filename = path + "\\" + 'Enums_in_directory_TrFile' + ".enum"
#                    print "CWD: %s, File: %s" % (os.path.abspath(os.curdir), input_id_filename)

                    # Open input file
                    fo = open(input_id_filename, "rb")
                    inhalt = fo.read() # Lese die gesamte Datei

                    Enum_output_file.write(inhalt)
                    if (addDirectory == True):
                        Enum_output_file_dev.write(inhalt)
                    
                    

                    # Close opend file
                    fo.close()



                AllFoundFiles.append(FoundFiles)
#            print AllFoundFiles

            Enum_output_file.seek(0)
            if (addDirectory == True):
                Enum_output_file_dev.seek(0)
            Tr_output_file_old.seek(0)            
            if (addDirectory == True):
                Tr_output_file_dev.seek(0)
            #Enum_output_file = open(sys.argv[(len(sys.argv) - 1)], "rb")
            inhalt = Enum_output_file.read() # Lese die gesamte Datei
            Enum_output_file.seek(0)
            inhalt_2 = Enum_output_file.readlines() # Lese die gesamte Datei
            inhalt_corrected = ""
            for line in inhalt_2:
                if (line.count("'") == 6 ):
                    inhalt_corrected = inhalt_corrected + line
            if (development == True):
                inhalt_corrected = inhalt
            inhalt_tr_file_old = Tr_output_file_old.read()
            Gesamt_output_file.write(inhalt_corrected)
            Gesamt_output_file.write(inhalt_tr_file_old)
            
            if (addDirectory == True):
                inhalt = Enum_output_file_dev.read() # Lese die gesamte Datei
                inhalt_tr_file_old = Tr_output_file_dev.read()
                Gesamt_output_file_dev.write(inhalt_corrected)
                Gesamt_output_file_dev.write(inhalt_tr_file_old)
            
            Gesamt_output_file.close()
            if (addDirectory == True):
                Gesamt_output_file_dev.close()
            
            if (addDirectory == True):
                Tr_output_file_dev.close()
                Enum_output_file_dev.close()
                Tr_output_file_old.close()
                
                os.remove(Tr_output_file_dev.name)
                os.remove(Enum_output_file_dev.name)
                os.remove(Tr_output_file_old.name)

            
            


    # Close opened output file
    Tr_output_file.close()
    Enum_output_file.close()
    



    ExecResult = 0
    print "Exit with result %d ..." % ExecResult
    sys.exit(ExecResult) # zero is considered "successful termination"
    
if __name__ == '__main__':

#    print(HdrCommentTxt)
    print "START TRACE ACCUMULATION"
    mainApplParseCmdLine()

