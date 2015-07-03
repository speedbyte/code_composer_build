""" 


    $Date$
    $Rev$
    $Author$
    $URL$

    Copyright (c) 2009 S1nn GmbH & Co. KG.
    All Rights Reserved.

"""
import intelhex
import sys
import crc

_svn_id = "$Id$"
_svn_rev = "$Rev$"
__version__ = _svn_rev[6:-2]


input_filename = sys.argv[1]
output_filename =sys.argv[2]

checksum_start_adress = int(sys.argv[3], 16)
#Example : 0x00008024
checksum_end_adress = int(sys.argv[4], 16)
#Example : 0x000BFFFB
crc_adress = int(sys.argv[5], 16) 
#Example : 0x000BFFFC

print "Utility to add a CRC to an Intel hex file"
print "V0.1"
print "Please note that this algoritm should be in sync "
print "with the CRC.c module of the UHV"
print "Copyright (c) 2009 S1nn GmbH & Co. KG."
print

print "Using input file", input_filename
image = intelhex.IntelHex(input_filename)
image.padding = 0x00
image.readfile()
image.addpads(checksum_start_adress,checksum_end_adress)
print "Lowest Adress found:  0x%X" % (image.minaddr(),)
print "Highest Adress found: 0x%X" % (image.maxaddr(),)

crcValue=0
myArray = []
for index in xrange(checksum_start_adress,checksum_end_adress+1):
    myArray.append(image[index])

table=crc.mkTable(0x104C11DB7,32)

crcValue = crc.crc32(myArray, crcValue, table)

print "CRC total:     0x%X" % (crcValue,)

image[crc_adress+3] = crcValue&0xFF
image[crc_adress+2] = (crcValue&0xFF00)>>8
image[crc_adress+1] = (crcValue&0xFF0000)>>16
image[crc_adress] = (crcValue&0xFF000000L)>>24

image.writefile(output_filename)


print "Output written to", output_filename

