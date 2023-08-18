/*
 * EFI Boot Guard
 *
 * Copyright (c) Siemens AG, 2023
 *
 * Authors:
 *  Michael Adler <michael.adler@siemens.com>
 *  psy <patrice.salathe@code-intelligence.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#include <zconf.h>

int crc32(Bytef __attribute__((unused)) * dest,
	  uLongf __attribute__((unused)) * destLen,
	  const Bytef __attribute__((unused)) * source,
	  uLong __attribute__((unused)) sourceLen)
{
	return 0;
}
