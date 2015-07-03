""" 


    $Date$
    $Rev$
    $Author$
    $URL$

    Copyright (c) 2007 S1nn GmbH & Co. KG.
    All Rights Reserved.

"""
import intelhex
import sys

_svn_id = "$Id$"
_svn_rev = "$Rev$"
__version__ = _svn_rev[6:-2]

print "\n\nUtility to build a Complete Flash (Application + Bootloader)"
print "V0.1"
print "Copyright (c) 2007 S1nn GmbH & Co. KG."
print

input_filename1 = sys.argv[1]
input_filename2 = sys.argv[2]
output_filename = sys.argv[3]
# read application
image1 = intelhex.IntelHex(input_filename1)
image1.readfile()
#read bootloader
image2 = intelhex.IntelHex(input_filename2)
image2.readfile()

print "Using input file", input_filename1
print "Using input file", input_filename2

# bootloaderfile hex file has to update the application file
image1.updateheximage(image2)
image1.writefile(output_filename)

print "\nNew build CompleteFlash write to ", output_filename

