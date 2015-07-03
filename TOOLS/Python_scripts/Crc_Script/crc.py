""" 


    $Date$
    $Rev$
    $Author$
    $URL$

    Copyright (c) 2006 S1nn GmbH & Co. KG.
    All Rights Reserved.

"""
import intelhex
import sys

def mkTable(poly, n):
    mask = (1L<<n) - 1
    poly = long(poly) & mask
    table = [bytecrc(long(i)<<(n-8),poly,n) for i in xrange(256)]
    return table

def crc32(data, crc, table):
    crc = crc & 0xFFFFFFFFL
    for x in data:
       crc = table[x ^ (int(crc>>24) & 0xFF)] ^ ((crc << 8) & 0xFFFFFFFFL)
    return crc

def bytecrc(crc, poly, n):
    crc = long(crc)
    poly = long(poly)
    mask = 1L<<(n-1)
    for i in xrange(8):
        if crc & mask:
            crc = (crc << 1) ^ poly
        else:
            crc = crc << 1
    mask = (1L<<n) - 1
    crc = crc & mask
    if mask <= sys.maxint:
        return int(crc)
    return crc



