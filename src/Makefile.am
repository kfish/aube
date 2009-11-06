## Process this file with automake to produce Makefile.in

SUBDIRS = modules tdb

INCLUDES = -DDATADIR=\"$(pkgdatadir)\"	\
	-I$(top_srcdir)/src/tdb		\
	@GTK_CFLAGS@ @SNDFILE_CFLAGS@

TDB_LIBS = $(top_builddir)/src/tdb/libtdb.a

bin_PROGRAMS = aube

noinst_HEADERS = \
	about_dialog.h		\
	aube_error.h		\
	aube.h			\
	gtkaube.h		\
	inputoption.h		\
	levelmeter.h		\
	master.h		\
	modulemenu.h		\
	modulewindow.h		\
	none_config.h		\
	none.h			\
	opsmenu.h		\
	outputlabel.h		\
	sample.h		\
	sample_if.h		\
	slider.h

aube_SOURCES = \
	about_dialog.c		\
	aube.c			\
        aube_error.c		\
	inputoption.c		\
	levelmeter.c		\
	master.c		\
	modulemenu.c		\
	modulewindow.c		\
	none.c			\
	opsmenu.c		\
	outputlabel.c		\
	sample.c		\
	sample_if.c		\
	slider.c		\
	tables.c		\
	none_config.c

aube_LDADD = modules/libmodules.a $(TDB_LIBS) @SNDFILE_LIBS@ @GTK_LIBS@
