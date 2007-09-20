/*
 * Copyright (c) 1996, 1997, 1998, 1999
 *             Shigio Yamaguchi. All rights reserved.
 * Copyright (c) 1999, 2000
 *             Tama Communications Corporation. All rights reserved.
 *
 * This file is part of GNU GLOBAL.
 *
 * GNU GLOBAL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GNU GLOBAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <assert.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#include "gparam.h"
#include "locatestring.h"
#include "makepath.h"
#include "path.h"
#include "test.h"
#include "usable.h"

#ifdef _WIN32
static const char *suffix[] = {".exe", ".com", ".bat",};
#endif

/*
 * usable: check if command is executable or not.
 *
 *	i)	command
 *	r)		1: executable
 *			0: non executable
 */
char *
usable(command)
char	*command;
{
	char	buf[MAXENVLEN+1], *p, *dir;
	static char path[MAXPATHLEN+1];
#ifdef _WIN32
	int	i, lim = sizeof(suffix)/sizeof(char *);
#endif

	if (isabspath(command) || locatestring(command, "./", MATCH_AT_FIRST)
		|| locatestring(command, "../", MATCH_AT_FIRST)) {
		if (test("fx", command)) {
			strcpy(path, command);
			return path;
		}
		return NULL;
	}
	strcpy(buf, getenv("PATH"));
	p = buf;
	while (p) {
		dir = p;
		if ((p = locatestring(p, PATHSEP, MATCH_FIRST)) != NULL)
			*p++ = 0;
		if (test("fx", makepath(dir, command, NULL))) {
			strcpy(path, makepath(dir, command, NULL));
			return path;
		}
#if defined(_WIN32)
		for (i = 0; i < lim; i++)
			if (test("f", makepath(dir, command, suffix[i]))) {
				strcpy(path, makepath(dir, command, suffix[i]));
				return path;
			}
#endif
	}
	return NULL;
}
