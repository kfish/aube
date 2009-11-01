## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -DDATADIR=\"$(pkgdatadir)\"

AM_CFLAGS = -g -O2 -Wall

INCLUDES = -I$(top_srcdir)/src @GTK_CFLAGS@

noinst_LIBRARIES = libmodules.a

noinst_HEADERS = \
	atonal.h		\
	atonal_if.h		\
	delay.h			\
	delay_if.h		\
	drummachine.h		\
	drummachine_if.h	\
	filter_reslp.h		\
	filter_reslp_if.h	\
	general_mixer.h		\
	general_mixer_if.h	\
	klav.h			\
	klavier.h		\
	klavier_if.h		\
	line.h			\
	linein_if.h		\
	lineout_if.h		\
	minimaube.h		\
	minimaube_if.h		\
	minitracker.h		\
	minitracker_if.h	\
	panner4.h		\
	panner4_if.h		\
	panner.h		\
	panner_if.h		\
	reslp.h			\
	sample_recorder.h	\
	sample_recorder_if.h	\
	syre.h			\
	syre_if.h		\
	waves.h			\
	whitenoise.h		\
	whitenoise_if.h		\
	xfader.h		\
	xfader_if.h


libmodules_a_SOURCES = \
	atonal.c		\
	atonal_if.c		\
	delay.c			\
	delay_if.c		\
	drummachine.c		\
	drummachine_if.c	\
	bilinear.c		\
	filter_reslp.c		\
	filter_reslp_if.c	\
	general_mixer.c		\
	general_mixer_if.c	\
	klav.c			\
	klavier.c		\
	klavier_if.c		\
	minimaube.c		\
	minimaube_if.c		\
	minitracker.c		\
	minitracker_if.c	\
	line.c			\
	linein_if.c		\
	lineout_if.c		\
	panner.c		\
	panner4.c		\
	panner4_if.c		\
	panner_if.c		\
	reslp.c			\
	sample_recorder.c	\
	sample_recorder_if.c	\
	syre.c			\
	syre_if.c		\
	waves.c			\
	whitenoise.c		\
	whitenoise_if.c		\
	xfader.c		\
	xfader_if.c

