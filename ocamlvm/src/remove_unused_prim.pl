#!/usr/bin/perl

use strict; use warnings;
use 5.14.0;

my %fun_ok; my $cpt = 0;

open my $IN_MAIN, "<", "prims.c.old" or die $!;
OUTER: while (<$IN_MAIN>) {
    my $line = $_;
    chomp $_;
    next unless $_ =~ m/^extern/;
    $_ =~ s/extern value (.*)\(\);/$1/;
    my $fun_name = $_;

    foreach my $file (glob("*")) {
	next unless (-e $file);
	next if ($file =~ /prims/);
	
	open my $IN, "<", $file or die $!;
	
	my @lines = grep / $fun_name/ , <$IN>;
	
	goto add_fun if (@lines);
	close $IN;
    }
    next OUTER;
    
  add_fun:
    $fun_ok{$fun_name} = $cpt++;
}
close $IN_MAIN;

open my $OUT, ">", "prims.c" or die $!;

print $OUT "#include \"caml/mlvalues.h\"\n" .
    "#include \"caml/prims.h\"\n";
foreach my $key (sort keys %fun_ok) {
    print $OUT "extern value $key();\n";
}

print $OUT "c_primitive caml_builtin_cprim[] = {\n";
foreach my $key (sort keys %fun_ok) {
    print $OUT "\t$key,\n";
} print $OUT "\t0 };\n";

print $OUT "char * caml_names_of_builtin_cprim[] = {\n";
foreach my $key (sort keys %fun_ok) {
    print $OUT "\t\"$key\",\n";
} print $OUT "\t0 };\n";

