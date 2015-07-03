# -*- coding: iso-8859-1 -*-
# Copyright (C) 2008  S1nn GmbH
# Author: hajo.butz@s1nn (Hajo Butz)
from __future__ import with_statement
import os
import sys
import string
import re

# Configure info
PCLintCompiler = 'SoftwareBuild_CompilerSpecPCLintFile.lnt'
LogPCLintFileName='PCLint.log'
PCLintRunDir = '.'  # relativ        
PCLintMisra = 'S1nn-misra.lnt'
PCLintProject = 'SoftwareBuild_ProjectPCLintFile.lnt'    
PCLintIncludeProject = 'SoftwareBuild_IncludeProjectPCLintFile.lnt'
PCLintTempDir= '\\Temp\\'  # relativ     
PCLintConfigDir= '\\BUILD\\tools\\'  # relativ           
     
def getModules(path='MOD_RekursiveSubList'):
    ret = []
    with open(path,'r') as f:
        
        line = f.readline()
        while line:
            exp = re.search('[\w]{3}_[\w]+',line) 
            if exp:
                ret.append(exp.group(0))
            line = f.readline()
    return ret



def RunPCLint(): # PCLintCompiler, LogPCLintFileName, PCLintRunDir,
               #PCLintMisra, PCLintProject, PCLintIncludeProject,
               #PCLintTempDir, PCLintConfigDir ):
    print "Now start PCLINT \n"
    # prepare environment
    save_dir = os.getcwd() 
    PCLindLogFileWithDirectory = '.\\TEMP\\' + LogPCLintFileName   
    os.chdir( PCLintRunDir )  
    print "PCLint is working in  directory ", os.getcwd(), "\n" 
    CmdString = "lint-nt.exe "+ save_dir + PCLintConfigDir + PCLintMisra +" " \
                + save_dir + PCLintConfigDir + PCLintCompiler +" " \
                + save_dir + PCLintTempDir + PCLintIncludeProject +" " \
                + save_dir + PCLintConfigDir + PCLintProject +" " \
                + ' > '+ PCLindLogFileWithDirectory + ' 2>&1'  
    print CmdString
    
    # run PCLINT
    os.system(CmdString)
    
    # count MISRA problems
    MISRARulesProblems = 0;
    WarningRulesProblems = 0 
    ErrorRulesProblems = 0
    NoteRulesProblems = 0
    
    f = open( PCLindLogFileWithDirectory, 'rt' )  
    for s in f.readlines():
        # search for Misra Problems
        if re.findall(r"\sError\s\d+",s,re.I):
            ErrorRulesProblems += 1 
        if re.findall(r"\sWarning\s\d+",s,re.I):
            WarningRulesProblems += 1 
        if re.findall(r"\sNote\s\d+",s,re.I):
            NoteRulesProblems += 1  
    f.close()

    MISRARulesProblems   = "MISRA Problem total:  %d" % (WarningRulesProblems + ErrorRulesProblems + NoteRulesProblems)
    ErrorRulesProblems   = "MISRA Errors:         %d" % ErrorRulesProblems
    WarningRulesProblems = "MISRA Warnings:       %d" % WarningRulesProblems
    NoteRulesProblems    = "MISRA Notes:          %d" % NoteRulesProblems

    print "\n"
    print "%s" % MISRARulesProblems
    print "%s" % ErrorRulesProblems
    print "%s" % WarningRulesProblems
    print "%s" % NoteRulesProblems

##    MISRARulesProblems =  str(MISRARulesProblems)  + " MISRA overall rules found for this build.\n"  
##    print  MISRARulesProblems 
##    ErrorRulesProblems =  str(ErrorRulesProblems)  + " MISRA Errors found for this build.\n"  
##    print  ErrorRulesProblems
##    WarningRulesProblems =  str(WarningRulesProblems)  + " MISRA Warnings found for this build.\n"  
##    print  WarningRulesProblems
##    NoteRulesProblems =  str(NoteRulesProblems)  + " MISRA Notes found for this build.\n"  
##    print  NoteRulesProblems

    return ( MISRARulesProblems, ErrorRulesProblems, WarningRulesProblems, NoteRulesProblems )
       
def SearchModulePCLintErrors():
    import time
    #modules = getModules()
    index = 0
    copyNow = False
    PCLindLogFileWithDirectory = '.\\TEMP\\' + LogPCLintFileName
    modules = []
    mod = ''
    tmp = ''
    with open(PCLindLogFileWithDirectory, 'r' ) as m:
        wr = ''
        for i in m.readlines():
            startline = re.match(r'---[\s]*Module:.*',i,re.I)
            if startline:
#                print startline.group(0)
                mod = re.search(r'(?<=MOD\\).*(?=\\src)',startline.group(0),re.I)                
            if mod:
                if mod.group(0) not in modules:
                    mode = 'w'
                    wr = open('.\\TEMP\\' + '%s.txt'%mod.group(0), 'w')
                    modules.append(mod.group(0))
                else:
                    mode ='a+'
                with open('.\\TEMP\\' + '%s.txt'%mod.group(0), mode) as wr:
                    wr.write(i)
                    wr.flush()
            else:
                module = ''

def ReadMisraInformationsFromFiles(path):
    WarningRulesProblems = 0 
    ErrorRulesProblems = 0
    NoteRulesProblems = 0
    with open(path, 'r') as a:
        for line in a.readlines():
            # search for Misra Problems
            if re.findall(r"\sError\s\d+",line,re.I):
                ErrorRulesProblems += 1 
            if re.findall(r"\sWarning\s\d+",line,re.I):
                WarningRulesProblems += 1 
            if re.findall(r"\sNote\s\d+",line,re.I):
                NoteRulesProblems += 1
        name = re.search(r'((?<=TEMP\\).*(?=\.txt))',path,re.I)
        Test = 123
        print '%-25s ==> %+5s %s %+5s %s %+5s %s'%(name.group(0), ErrorRulesProblems, 'Errors', WarningRulesProblems, 'Warnings', NoteRulesProblems, 'Notes')

        
def SearchPrintModulePCLintErrors():
    SearchModulePCLintErrors()
    AllList = os.listdir(os.curdir+PCLintTempDir)
    print '\n----------------------------------------------\n'
    print '\nMISRA reporting for each module:\n' 
    for i in AllList:
        name = re.search(r'[\w]{3}_[\w]+.+(?<=\.txt)',i,re.I)
        if name:
            ReadMisraInformationsFromFiles('.\\TEMP\\'+ name.group(0))

# ###########
# Main part
# ###########
if __name__ == "__main__":
    # RunScript
    
    # Start PCLINT    
    
    # InfoMisra = RunPCLint() # PCLintCompiler, LogPCLintFileName, 
                           #PCLintRunDir,
                           #PCLintMisra, PCLintProject,
                           #PCLintIncludeProject, PCLintTempDir,
                           #PCLintConfigDir )

    RunPCLint()
    SearchPrintModulePCLintErrors()

    sys.exit(0)

