#!/usr/bin/perl

# create a po file from a help file
#
# $ARGV[0]: help file
# $ARGV[1]: po file

my %podefs;
my %written;

# read a help file

sub read_help
{
  my ($file) = @_;
  my ($lang) = ($file =~ /help\/help_mp-(.*).h/);

  open(help, "< $file") or die "Cannot open $file";

  while (my $line = <help>)
  {
    chomp $line;

    if ($line =~ s/\\$//)
    {
      $line .= <help>;
      redo unless eof(help);
    }

    next if ($line !~ /^\s*#define/);

    my ($msgid, $msgstr) = ($line =~ /^\s*#define\s+(\w+)\s+(".*")/);

    if (!$msgstr)
    {
      # defined by an other macro?
      ($msgid, $msgstr) = ($line =~ /^\s*#define\s+(\w+)\s+(\w+)/);

      next if (!$msgstr);

      $msgstr = $podefs{$msgstr}{$lang};
    }

    $podefs{$msgid}{$lang} = $msgstr;
  }

  close(help);
}

read_help("help/help_mp-en.h");
read_help($ARGV[0]);

# create po file

my ($lang) = ($ARGV[1] =~ /help\/po\/(.*).po/);
open(po, "> $ARGV[1]") or die "Cannot open $ARGV[1]";

print po "# DO NOT EDIT - this file is generated from $ARGV[0]\n";

foreach my $id (keys %podefs)
{
  # for the moment only X11/GTK GUI strings
  next if (substr($id, 0, 10) ne "MSGTR_GUI_");
  next if (substr($id, 0, 15) eq "MSGTR_GUI_WIN32");

  my $msgid = $podefs{$id}{"en"};
  my $msgstr = $podefs{$id}{$lang} ? $podefs{$id}{$lang} : $msgid;

  # avoid duplicates
  next if ($written{$msgid}{$msgstr});

  # no translation necessary
  next if ($msgid eq $msgstr);

  # different translations
  die "$msgid has already been translated differently\n" if (keys %{$written{$msgid}});

  print po "\nmsgid $msgid\n";
  print po "msgstr $msgstr\n";

  $written{$msgid}{$msgstr} = 1;
}

close(po);
