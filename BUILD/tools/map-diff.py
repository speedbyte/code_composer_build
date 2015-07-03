import sys
import os
import re
import types

__script__  = "MAP-DIFF"
__version__ = "v0.1"

NONE = 0
TEXT = 1
CONST = 2
CINIT = 3
BSS   = 4

debug = False

linetype = [ "NONE", "TEXT", "CONST", "CINIT", "BSS" ]

def readMAPFile( filename ):
    code = {}
    const = {}
    cinit = {}
    bss = {}
    
    linevar  = [ None, code, const, cinit, bss ]
    linestart = [ None, ".code", ".const", ".cinit", ".bss" ]

    fnd = NONE
    adr    = None
    laenge = None
    datei  = None
    lib    = ""
    holes = 0

    f = open( filename, "r" )
    lineno = 0

    for line in f:
        lineno += 1
        line = line.strip( "\n\r" )
        #print "Read line <%s>" % line
        if line.startswith( ".text" ):
            fnd = TEXT
            if debug:
                print "Found TEXT in line %d: <%s>" % (lineno, line) 
        elif line.startswith( ".const" ):
            fnd = CONST
            if debug:
                print "Found CONST in line %d: <%s>" % (lineno, line ) 
        elif line.startswith( ".cinit" ):
            fnd = CINIT
            if debug:
                print "Found CINIT in line %d: <%s>" % (lineno, line ) 
        elif line.startswith( ".bss" ):
            fnd = BSS
            if debug:
                print "Found BSS in line %d: <%s>" % (lineno, line ) 
        elif line == "":
            if fnd != NONE:
#                print "Found EMPTY in line %d: <%s>" % (lineno, line )

                if holes != 0:
                    (linevar[fnd])[":--HOLE--"] = ("%d" % holes)
                    holes = 0
                    
                fnd = NONE

        else:
            if fnd != NONE:
#                print "Read %s line <%s>" % (linetype[fnd], line )

                toks = re.split( '[ \t]+(\w+)[ \t]+(\w+)[ \t]+(\S+)[ \t]*(:)[ \t]*(\S+)', line )
                if debug:
                    print "------(1)--> %d toks: <%s>" % (len(toks), toks)
                if len( toks ) >= 4:
                    if toks[3] != None:
                        lib = toks[3]

                    adr    = toks[1]
                    laenge = toks[2]
                    datei  = toks[5]

                else:
                    toks = re.split( '[ \t]+(\w+)[ \t]+(\w+)[ \t]+(:)[ \t]*(\S+)', line )
                    if debug:
                        print "------(2)--> %d toks: <%s>" % (len(toks), toks)
                    if len( toks ) >= 4:
                        # lib unveraendert!

                        adr    = toks[1]
                        laenge = toks[2]
                        datei  = toks[4]

                    else:
                        toks = re.split( '[ \t]+(\w+)[ \t]+(\w+)[ \t]+(\S+)', line )

                        if debug:
                            print "------(3)--> %d toks: <%s>" % (len(toks), toks)

                        if toks[3] == "--HOLE--":
                            laenge = toks[2]
                            if debug:
                                print "--------XX--> HOLE: old: %d + laenge: %d (%s) => new: %d" % (holes, (long( laenge, 16)), laenge, (holes + long( laenge, 16)))
                            holes += long( laenge, 16 )
                            continue
                        else:
                            lib = ""

                        adr    = toks[1]
                        laenge = toks[2]
                        datei  = toks[3]

                if debug:
                    print "--Adresse -> ", adr
                    print "--Laenge---> ", laenge
                    print "--Lib------> ", lib
                    print "--File-----> ", datei

                m = lib + ':' + datei
                if fnd != NONE:
                    (linevar[fnd])[m] = laenge

    return code, const, cinit, bss

def diffMAPFiles( dict1, dict2, alle ):
    dres = {}
    
    ci1 = dict1.keys()
    ci1.sort()
    ci2 = dict2.keys()
    ci2.sort()

    cnt1 = len( ci1 )
    cnt2 = len( ci2 )
    cnt = max( cnt1, cnt2 )
    if debug:
        print "Diff'ing %d vs. %d -> %d max" % (cnt1, cnt2, cnt )

    i = 0
    j = 0
    summe = 0
    
    for x in range(cnt):
        m1 = ci1[i]
        m2 = ci2[j]

        if debug:
            print "Working on (1): <%s> and (2): <%s>" % (m1, m2 )

        if m1 == m2:
            if debug:
                print "    m1 == m2"

            l1 = dict1[m1]
            l2 = dict2[m2]
            dl = long(l2,16) - long( l1, 16 )
            if alle or m1 == ":--HOLE--" or dl != 0 :
                dres[m1] = ( l1, l2, dl )

            summe += dl
            
            i += 1
            j += 1

        # m1 entfallen
        elif m1 < m2:
            if debug:
                print "    m1 < m2"

            l1 = dict1[m1]
            l2 = None
            dl = - long( l1, 16 ) 
            dres[m1] = ( l1, l2, dl )

            summe += dl
            i += 1
            # j unveraendert!

        # m2 neu
        elif m1 > m2:
            if debug:
                print "    m1 > m2"
            # i unveraendert!

            l1 = None
            l2 = dict2[m2]
            dl = long( l2, 16 )
            dres[m2] = ( l1, l2, dl )

            summe += dl
            j += 1
        else:
            print "    ERROR: m1 ?? m2. This should never appear!"
    return summe, dres

def printSortedResult( dicts, dictt, summe, header ):
    ci1 = dictt.keys()
    ci1.sort()
        
    print "%s: (sorted)" % dicts
    print "   %60.60s:  %s" % ( "modul:object file", header )
    for m in ci1:
        val = dictt[m]

        if debug:
            print "val is <%s>, %s, %d" % (type(val), isinstance( val, tuple ), len(val))
            
        if isinstance( val, tuple ):
            s = ""
            first = True
            #print "   <%s>: <%s>: <%s> len: %d" % (type(val), m, val, len(val) )
            for a in val:
                if first:
                    first = False
                else:
                    s += "   "
                    
                #print "       - <%s> <%s>" % (type(a), a )
                if isinstance( a, long ):
                    s += "%8d" % a
                elif isinstance( a, int ):
                    s += "%8d" % a
                elif isinstance( a, str ):
                    s += "%8.8s" % a
                else:
                    s += "        "
                    
            print "   %60.60s:  %s" % ( m, s )
            # print "   <%60.60s>:  <%s>" % ( m, s )   # Testausgabe
        else:
            print "   %60.60s:  %s" % ( m, val )
            # print "   <%60.60s>:  <%s>" % ( m, val ) # dictt[m] )  # Testausgabe
    print "   %60.60s:  %8.8s   %8.8s   %8d" % ("Summe", "               ", "                ", summe )
    print ""
    
def printSortedTest( dicts, dictt, header ):
    ci1 = dictt.keys()
    ci1.sort()
        
    print "%s: (sorted)" % dicts
    print "   %60.60s:  %s" % ( "modul:object file", header )
    for m in ci1:
        val = dictt[m]
        if debug:
            print "val is <%s>, %s, %d" % (type(val), isinstance( val, tuple ), len(val))
        if isinstance( val, tuple ):
            s = ""
            first = True
            if debug:
                print "   <%s>: <%s>: <%s> len: %d" % (type(val), m, val, len(val) )
            for a in val:
                if first:
                    first = False
                else:
                    s += "   "
                    
                #print "       - <%s> <%s>" % (type(a), a )
                if isinstance( a, long ):
                    s += "%8d" % a
                elif isinstance( a, int ):
                    s += "%8d" % a
                elif isinstance( a, str ):
                    s += "%8.8s" % a
                else:
                    s += "        "
                    
            print "   <%60.60s>:  <%s>" % ( m, s )   # Testausgabe
        else:
            print "   <%60.60s>:  <%s>" % ( m, val ) # dictt[m] )  # Testausgabe
    print ""
    
def usage():
    print "Usage:"
    print ""
    print "%s [-all] [-debug] <map-file-1> <map-file-2>" % (os.path.basename(sys.argv[0]))


if __name__ == "__main__":

    print ""
    print ""
    print "%s %s" % ( __script__, __version__ )
    print ""
    print ""
    
    if len( sys.argv ) < 3:
        print "Missing arguments"
        usage()
##    elif len( sys.argv ) > 5:
##        print "Too many arguments"
##        usage()
    else:
        file1 = None
        file2 = None
        alle = False
        
        
        for arg in sys.argv:
#            print "found %s" % arg
            if arg.startswith( "-" ):
                if arg == "-all":
#                    print "found option -all"
                    alle = True
                elif arg == "-debug":
                    debug = True
                else:
                    print "Unknown option: <%s>" % arg
                    usage()
                    sys.exit(1)
            else:
                if arg == sys.argv[0]:
                    pass
                elif file1 == None:
#                    print "found file1: <%s>" % arg
                    file1 = arg
                elif file2 == None:
#                    print "found file2: <%s>" % arg
                    file2 = arg
                else:
                    print "Unknown or too much arguments: <%s>" % arg
                    usage()
                    sys.exit(1)

        if file1 == None or file2 == None:
            if file1 == None:
                print "Missing first file argument"
            elif file2 == None:
                print "Missing second file argument"
            usage()
            exit(1)
            
        print "Comparing MAP file %s" % file1
        print "     with MAP file %s" % file2
        print ""
        code1, const1, cinit1, bss1 = readMAPFile( file1 )
        code2, const2, cinit2, bss2 = readMAPFile( file2 )

        if debug:
            printSortedTest( "CODE 1", code1, "Length" )
            printSortedTest( "CONST 1", const1, "Length" )
            printSortedTest( "CINIT 1", cinit1, "Length" )
            printSortedTest( "BSS 1", bss1, "Length" )

            printSortedTest( "CODE 2", code2, "Length" )
            printSortedTest( "CONST 2", const2, "Length" )
            printSortedTest( "CINIT 2", cinit2, "Length" )
            printSortedTest( "BSS 2", bss2, "Length" )
        
        scode, dcode = diffMAPFiles( code1, code2, alle )
        printSortedResult( "CODE", dcode, scode, "Length     Length         diff" )
        
        sconst, dconst = diffMAPFiles( const1, const2, alle )
        printSortedResult( "CONST", dconst, sconst, "Length     Length         diff" )

        scinit, dcinit = diffMAPFiles( cinit1, cinit2, alle )
        printSortedResult( "CINIT", dcinit, scinit, "Length     Length         diff" )

        sbss, dbss = diffMAPFiles( bss1, bss2, alle )
        printSortedResult( "BSS", dbss, sbss, "Length     Length         diff" )
