## Process this file with automake to produce Makefile.in

SUBDIRS = modules

bin_PROGRAMS = aube

aube_SOURCES = \
	about_dialog.c		\
	about_dialog.h		\
	aube.c			\
	aube.h			\
        aube_error.c		\
        aube_error.h		\
	inputoption.c		\
	inputoption.h		\
	interface.c		\
	interface.h		\
	levelmeter.c		\
	levelmeter.h		\
	master.c		\
	master.h		\
	modulemenu.c		\
	modulemenu.h		\
	none.c			\
	none.h			\
	opsmenu.c		\
	opsmenu.h		\
	outputlabel.c		\
	outputlabel.h		\
	sample.c		\
	sample.h		\
	sample_if.c		\
	sample_if.h		\
	slider.c		\
	slider.h		\
	tables.c		\
	none_config.c		\
	none_config.h

INCLUDES = -DDATADIR=\"$(pkgdatadir)\" \
 @GTK_CFLAGS@ @SNDFILE_CFLAGS@

aube_LDADD = \
 modules/libmodules.a @SNDFILE_LIBS@ @GTK_LIBS@

