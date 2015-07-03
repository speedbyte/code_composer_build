#******************************************************************************/
#*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
#***************************  All Rights Reserved  ****************************/
#******************************************************************************/
#
#******************************************************************************/
#* File Name:     PoolTool.pl
# -----------------------------------------------------------------------------
#  Module Name: Pool handling
# -----------------------------------------------------------------------------
#  Description: convert config file to source files (.h, .c, .mk)
# -----------------------------------------------------------------------------
#  $Date$
#  $Rev$
#  $Author$
#  $URL$
# -----------------------------------------------------------------------------
#
use strict;

my %poolColl;      # hash holds pool specific configuration elements 
my %checks;        # hash holds non pool specific check elements 
my $pool;          # currently processed pool name
my @template;      # array holding the lines of the current template text file
my $insideLoop;    # indicates whether or not current line of template within loop
my @loop;          # loop block as part of the current template text file
my @files          # list of files (for which template file are required
        = qw (POOL_ARRAY.c POOL_GENERATED.h POOL_FLAGS.mk);
# 
sub replace {
my $lp   = shift;
my $line = shift;
   $line =~ s/<NAME>/$lp/g;
   foreach my $subst (%{$poolColl{$lp}}) {
   	  my $src = "<$subst>";
   	  my $dst = "$poolColl{$lp}{$subst}";
      $line =~ s/$src/$dst/g;
   }
   $line;
}

##################################
### MAIN #########################
##################################

open IN,"poolconfig.txt" or die;

# scan config file, filling the hash %poolColl
while (<IN>) {
   if    (/^\s*POOL\s+(\w+)/){
      $pool = $1;
   }
   elsif (/^\s*STRUCTURE\s+(\w+)/){
      $poolColl{$pool}{STRUCTURE} = $1;
   }
   elsif (/^\s*INCLUDE\s+\"(.*)\"\s*$/){
      $poolColl{$pool}{INCLUDE} = $1;
   }
   elsif (/^\s*COUNT\s+(\w+)\s*$/){
      $poolColl{$pool}{COUNT} = $1;
   }
   elsif (/^\s*CHECK\s+(\w+)\s+(TRUE|FALSE)\s*$/){
      $checks{$1} = $2 eq "TRUE" ? 1 : 0;
   }
}
close IN;

# === Process all files in the list
foreach my $file (@files) {
   # derive name of template file and open it 
   my ($fname,$ext) = ($file =~ /^(.*)(\..*)$/);
   open IN,  $fname."_template"."$ext" or die "Can not open template file for $fname";

   # create source file 
   open OUT, ">$file" or die "Can not write to $file";

   # read template file store and close it 
   @template = <IN>;
   close IN;
   
   $insideLoop = 0;
   my $lncnt =0;
   foreach my $line (@template) {
      $lncnt++;
      if (!$insideLoop) {
         if ($line =~ /!!loop/) {
           $insideLoop = 1;
           # clear loop block
           @loop = ();
         } elsif ($line =~ /!!endloop/) {
           die "*** ERR in config file, line $lncnt: misplaced endloop found";
         } else {
           print OUT "$line";
         }	
      } else {
         if ($line =~ /!!loop/) {
           die "*** ERR in config file, line $lncnt: loop nesing not allowed";
         } elsif ($line =~ /!!endloop cflags/) {
            $insideLoop = 0;
            # --- repeat loop for all cflags
            foreach my $lp (sort keys %checks) {
               # --- repeat fo all lines of a loop block  
               foreach (@loop) {
                  $line = $_;
                  my $relevance = $checks{$lp} ? " " : "#";
                  $line =~ s/<OPT>/$relevance/;  
                  $line =~ s/<FLAG>/$lp/;  
                  print OUT "$line";	      
               }
            }
         } elsif ($line =~ /!!endloop/) {
            $insideLoop = 0;
            # --- repeat loop for all pool elements
            foreach my $lp (sort keys %poolColl) {
               # --- repeat fo all lines of a loop block  
               foreach (@loop) {
                  $line = replace ($lp, $_);  
                  print OUT "$line";	      
               }
            }
         } else {
            push @loop, $line;
         }	
      } 
   }
   close OUT;
}

