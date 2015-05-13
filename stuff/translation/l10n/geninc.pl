#!/usr/bin/perl

use Data::Dumper;

sub replace_in_file {
	my($plik,$linia,$r_from,$r_to)=@_;
	my $fh,$ofh;
	open($fh, "<", $plik)
	    or die "cannot open < $plik: $!";

	open($ofh, ">", $plik."-tmpreplace")
		or die "cannot open > $plik-tmpreplace: $!";

	local $tmp_line,$_;
	local $replaced=0;
	while (<$fh>) {
		if ($.==$linia) {
#			print "Linia: $_\n\n";
			if ($_=~s/\Q$r_from/$r_to/) { $replaced++; };
#			print $_."\n\n";
#			$replaced++;
		};
#		print "Linia: $_\n\n" if ($.==$linia);
		print $ofh $_;
	}
	
	close($fh);
	close($ofh);
	if ($replaced==1) {
		print "R $plik:$linia\n";
		rename $plik."-tmpreplace", $plik;
#		print "OK\n";
	} else {
		print "R $plik:$linia\t";
		print "	$r_from -> $r_to\n";

		print "FAIL\n\n";
		unlink $plik."-tmpreplace";
	}
}


#$string="":
while (<>) {
	$string.=$_;
}


# usuwamy:
#, fuzzy, c-format 
#, c-format
$string=~s/^#, c-format\n//mg;
$string=~s/^#, fuzzy\n//mg;
$string=~s/^#, fuzzy, c-format\n//mg;

$string=~s/(?<!\n)\n#/ /gms;

# zamieniamy cos takiego:
#	msgid ""
#	"Wprowadz tresc nowe tablicy rejestracyjnej.\n"
#	"{ff3030}Uzyles niedozwolonych znakow. Sprobuj ponownie."
#	msgstr ""
#	"Enter type your new plate.\n"
#	"{ff3030}Invalid characters used, please retry."
# na:
#	msgid "Wprowadz tresc nowe tablicy rejestracyjnej.\n{ff3030}Uzyles niedozwolonych znakow. Sprobuj ponownie."
#	msgstr "Enter type your new plate.\n{ff3030}Invalid characters used, please retry."

$string=~s/\"\n\"//gms;

# usuwamy komentarze

$string=~s/^\#\~ .*$//mg;


#print $string;
#exit;

# #: gamemodes/fs.pwn:117
while ($string=~m/^#: (.*?)\nmsgid (".*")\nmsgstr (".+")\n/gm) {
		$plik=$1;
		$z=$2;
		$na=$3;
		$plik=~s/ , c-format$//;
		$plik=~s/ , fuzzy$//;
		$plik=~s/ : / /g;
		@pliki=split /\s/, $plik;	

		foreach $p (@pliki) {
			next if ($p=~/^[,:]+$/);
			 
			$p=~s/:(\d+)$//;
			$linia=$1;
#			print "PLIK: $p LINIA $linia\n\t $z -> $na\n";
#			$RES=system "replace", ("__(".$z.")", $na, "--", $p);
			replace_in_file($p,$linia,'__('.$z.')',$na);
#			exit;
	
		}
#		print Dumper(@pliki);
		
		
#		$RES=system "replace", ("__(".$z.")", $na, "--", $plik);

#		print $RES."\n";
#		print "\n::".$1."\n---".$2."(".$3;
}

