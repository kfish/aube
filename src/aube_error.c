/*
 * AUBE/Metadecks Live
 *
 * Copyright (C) 1998-2009 Conrad Parker
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include "aube.h"

void aube_error(int errlvl, char *msg)
{
	switch (errlvl) {
	case AUBE_MESSAGE:
		fprintf(stderr, "AUBE: %s\n", msg);
		break;
	case AUBE_ERROR_RECOVERABLE:
		fprintf(stderr, "AUBE: %s\n", msg);
		break;
	case AUBE_ERROR_URGENT:
		fprintf(stderr, "AUBE: %s\n", msg);
		break;
	case AUBE_ERROR_FATAL:
		fprintf(stderr, "AUBE FATAL ERROR: %s\n", msg);
		break;
	default:
		break;
	}
}
