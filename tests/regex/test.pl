#!/usr/bin/env perl 
#===============================================================================
#
#         FILE: test.pl
#
#        USAGE: ./test.pl  
#
#  DESCRIPTION: 
#
#      OPTIONS: ---
# REQUIREMENTS: ---
#         BUGS: ---
#        NOTES: ---
#       AUTHOR: YOUR NAME (), 
# ORGANIZATION: 
#      VERSION: 1.0
#      CREATED: 2014年12月25日 10时16分04秒
#     REVISION: ---
#===============================================================================

use strict;
use warnings;
use utf8;
use File::Path qw(make_path remove_tree);

my $str = "hello\nTarget: x86_64-linux-gnu\nConfigured with: ../src/configure\n";
#print $str;
$str =~ s/(.*)(Target: [\w-]*)(.*)/$3/s;
#$str =~ s/(.*[*\r\n]*)(Target: )(.*[*\r\n]*)/$2/g;
print "$str\n";

print STDOUT <<EOF;
#include <stdio.h>
#include <stdlib.h>
int main()
{

}
EOF

make_path('tests');
remove_tree('tests');

my $lib = "liblua5.2.so";
if ($lib =~ /liblua[\d.]+.so$/) {
    print "$lib\n";
    $lib =~ s/(lib)(lua[\d.]+)(.so)/$2/g;
    print "$lib\n";
}

my @list = (1, 2, 3, 4, 5);

foreach my $i (@list) {
    print "$i\n";
}

my $data = "hello,world";
my ($res) = $data =~ s/hello//g;
print "$res\n";
