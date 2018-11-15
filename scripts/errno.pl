#!/usr/bin/env perl

open I, '<errno.txt' or die;
open H, '>errno.h' or die;
open M, '>lib/strerr.c' or die;
open MH, '>lib/strerr.h' or die;

print H qq[#pragma once\n\n];

print M <<EOF
static
const char *err_msgs[] = {
	"success",
EOF
;

print MH <<EOF
#pragma once

const char *strerror(int err);
EOF
;

sub trim
{
	my $x = shift;
	$x =~ s/^\s+//;
	$x =~ s/\s+$//;
	return $x;
}

my $i = 0;
while (<I>)
{
	$i++;

	my @x = split /:/;
	continue unless length @x;
	my ($err, $msg) = @x;
	$err = trim $err;
	$msg = trim $msg;

	print H qq[#define E_$err $i // $msg\n];
	print M qq[\t"$msg",\n];
}

print M <<EOF
};

const char *strerror(int err)
{
	if (err < 0)
		err = -err;
	if (err >= sizeof(err_msgs))
		return "error";
	return err_msgs[err];
}
EOF
;
