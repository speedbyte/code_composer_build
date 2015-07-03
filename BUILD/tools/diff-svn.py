import os
import os.path
import sys
import string
import re
import urlparse
import optparse

import pysvn

def usage():
    print "%s <rel 1> <rel 2>"
    
def getModulList( externals1 ):
    mods = {}
    for ext1key, ext1val in externals1.iteritems():
        #print "ext1key: <%s>" % ext1key
        #print "ext1val: <%s>" % ext1val
    
        lines = ext1val.split( '\n' )
        #print "lines: %d" % len( lines )

        cnt = 0
        for line in lines:
            #print "line %d: <%s>" % (cnt, line )
            linec = line.split()
            #print "linec %d: %d: <%s>" % (cnt, len( linec ), linec )

            if len( linec ) == 3:
                rev = linec[0]
                label = linec[1]
                mod = linec[2]
            #rev, label, mod = re.split( "-r([0-9]+)[ \t]+([a-zA-Z_0-9.]+)[ \t]+(\s*)", line )
                #print "rev: %s, mod: <%s>, label: <%s>" % (rev, mod, label )

                mods[mod] = [ rev, label ]
                
            cnt += 1

    return mods


#-------------------------------------------------------------------------------
# main


if __name__ == "__main__":
    description = """

    Small tool to generate diff reports for two tags in Subversion
    
"""
    script_version = "0.1"

    prog = os.path.basename( sys.argv[0] )

    parser = optparse.OptionParser( usage = "%prog [options] tag1 tag2", version = "%prog " + script_version, description = description )
    parser.add_option( "-d", "--debug",   action = "store_true", dest = "debug",   default = False, help = "Debugging output" )
    parser.add_option( "-s", "--summary", action = "store_true", dest = "summary", default = False, help = "Only summary" )
    parser.add_option( "-r", "--revision", dest = "rev", nargs=2, help = "SVN revisions" )

    ( options, args ) = parser.parse_args()

    debug   = options.debug
    summary = options.summary
    if options.rev != None :
        rev1 = options.rev[0]
        try:
            rev2 = options.rev[1]
        except IndexError:
            rev2 = ""
    else:
        rev1 = ""
        rev2 = ""
        
    rel1 = args[0]

    try:
        rel2 = args[1]
    except IndexError:
        rel2 = rel1
        rel1 = rev2
        rev2 = ""

    parser.print_version()
    
    #print "%s v0.1" % os.path.basename( sys.argv[0] )
    print
    if debug:
        print "debug:     %s" % debug
        print "summary:   %s" % summary
        print "rev1:      %s" % rev1
        print "rev2:      %s" % rev2
        print "rel1:      %s" % rel1
        print "rel2:      %s" % rel2
        
    svnclient = pysvn.Client()

    root_url = svnclient.root_url_from_path( rel1 )
    
    (url_scheme, url_netloc, url_path, _, _, frag) = urlparse.urlparse( root_url )

    if debug:
        print
        print "Repository Root: %s: scheme: %s, netloc: %s, path: %s" % (root_url, url_scheme, url_netloc, url_path )

    if rev1 != "":
        r1 = pysvn.Revision( pysvn.opt_revision_kind.number, rev1 )
    else:
        r1 = pysvn.Revision( pysvn.opt_revision_kind.unspecified )

    if rev2 != "":
        r2 = pysvn.Revision( pysvn.opt_revision_kind.number, rev2 )
    else:
        r2 = pysvn.Revision( pysvn.opt_revision_kind.unspecified )

    if summary:
        sumdiffpattern = re.compile( "(.*)'([a-zA-Z0-9_-]+)'(.*)" )
        
        difflst = svnclient.diff_summarize( rel1,                revision1 = r1,
                                            url_or_path2 = rel2, revision2 = r2 )

        if debug:
            print "difflst: <%s>" % difflst 
        diffstr = ""
        for i in range( len( difflst ) ):
            ds = str( difflst[i] )
            dsp = ds.split( '\'' )
            if dsp == None:
                dsp = ""
            else:
                if debug:
                    print "dsp: <%s>" % dsp
                dsp = str( dsp[1] )

            if debug:
                print "difflst[%d/%d]: <%s> -> ds <%s> -> dsp <%s>" % (i, len(difflst), difflst[i], ds, dsp )
            diffstr = diffstr + "\n" + dsp

        if debug:
            print "diffstr: <%s>" % diffstr
        
    else:
        diffstr = svnclient.diff( "C:\TEMP",
                                  rel1, r1,
                                  rel2, r2 )

    lines = diffstr.split( '\n' )
    switchoff = False
    for line in lines:
        if debug:
            print "Read <%s>" % line
            
        if line.find( "svn:externals" ) >= 0:
            switchoff = True
        elif line.find( "Property changes on:" ) >= 0:
            switchoff = True
        elif line == "" or line == "\r" or line == "\r\n" or line == "\n":
            pass
        else:
            if not switchoff:
                print "%s" % line
            else:
                if line == "" or line == "\n":
                    switchoff = False
                    
    print
    
    externals1 = svnclient.propget( "svn:externals", rel1,
                                    #revision = pysvn.Revision(),
                                    revision = pysvn.Revision( pysvn.opt_revision_kind.unspecified ),
                                    recurse = False,
                                    peg_revision = pysvn.Revision( pysvn.opt_revision_kind.unspecified ),
                                    #depth = 1
                                    )

    mods1 = getModulList( externals1 )

    if debug:
        print
        print "von %s:" % rel1
        
    for modul, prop in mods1.iteritems():
        rev = prop[0]
        label = prop[1]

        if debug:
            print "Modul: %s: rev %s, label: %s" % (modul, rev, label )
    
    externals2 = svnclient.propget( "svn:externals", rel2,
                                    #revision = pysvn.Revision(),
                                    revision = pysvn.Revision( pysvn.opt_revision_kind.unspecified ),
                                    recurse = False,
                                    peg_revision = pysvn.Revision( pysvn.opt_revision_kind.unspecified ),
                                    #depth = 1
                                    )

    mods2 = getModulList( externals2 )

    if debug:
        print
        print "von %s:" % rel2
        
    for modul, prop in mods2.iteritems():
        rev = prop[0]
        label = prop[1]

        if debug:
            print "Modul: %s: rev %s, label: %s" % (modul, rev, label )

    if debug:
        print
        print

    diffmods = {}
    for mod in mods1.iterkeys():
        if debug:
            print "Module %s:" % mod

        prop1 = mods1[mod]
        prop2 = mods2[mod]

        if prop1 == None:
            print "        Module %s is not available in %s" % (mod, rel1)
        if prop2 == None:
            print "        Module %s is not available in %s" % (mod, rel2)

        if prop1 != None and prop2 != None :
            rev1 = prop1[0]
            lab1 = prop1[1]
            rev2 = prop2[0]
            lab2 = prop2[1]

            if rev1 == rev2 and lab1 == lab2:
                if debug:
                    print "       identical"
                pass
            elif rev1 == rev2 and lab1 != lab2:
                print "       ERROR: revision are identical, but labels not!"
            elif rev1 != rev2 and lab1 == lab2:
                print "       ERROR: labels are identical, but revisions not!"
            else:
                if debug:
                    print "       different: rev %s -> %s" % ( rev1, rev2 )
                diffmods[mod] = [ rev1, lab1, rev2, lab2 ]

    print
    print

    revrev = re.compile( "-r[ ]*([0-9]+)" )
    
    for mod, prop in diffmods.iteritems():
        rev1 = prop[0]
        lab1 = prop[1]
        rev2 = prop[2]
        lab2 = prop[3]

        if debug:
            print "Module %s: %s(%s) -> %s(%s)" % (mod, lab1, rev1, lab2, rev2 )

        p1 = urlparse.urlunparse( (url_scheme, url_netloc, lab1, "", "", "" ))
        r1o = revrev.search( rev1 )
        r1 = r1o.group(1)
        
        p2 = urlparse.urlunparse( (url_scheme, url_netloc, lab2, "", "", "" ))
        r2o = revrev.search( rev2 )
        r2 = r2o.group(1)

        print "Module: %s:" % mod
        print "<===  %s: %s" % (r1, p1 )
        print "===>  %s: %s" % (r2, p2 )

        if summary:
            difflst = svnclient.diff_summarize( 
                                                p1,                revision1 = pysvn.Revision( pysvn.opt_revision_kind.number, r1 ),
                                                url_or_path2 = p2, revision2 = pysvn.Revision( pysvn.opt_revision_kind.number, r2 ) )
            #print "difflst: <%s>" % difflst 
            diffstr = ""
            for i in range( len( difflst ) ):
                ds = str( difflst[i] )
                dsp = ds.split( '\'')

                if dsp == None:
                    dsp = ""
                else:
                    if debug:
                        print "dsp: <%s>" % dsp
                    dsp = str( dsp[1] )

                if debug:
                    print "difflst[%d/%d]: <%s> -> ds <%s> -> dsp <%s>" % (i, len(difflst), difflst[i], ds, dsp )
                diffstr = diffstr + "\n" + dsp

            if debug:
                print "diffstr: <%s>" % diffstr
        
        else:
            diffstr = svnclient.diff( "C:\TEMP",
                                      p1, pysvn.Revision( pysvn.opt_revision_kind.number, r1 ),
                                      p2, pysvn.Revision( pysvn.opt_revision_kind.number, r2 ) )

        print "%s" % diffstr
        print
