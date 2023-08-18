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

#include <fuzzer/FuzzedDataProvider.h>
#include <cifuzz/cifuzz.h>

extern "C" {
#include "bg_envtools.h"
#include "bg_printenv.h"
}

FUZZ_TEST_SETUP()
{
}

FUZZ_TEST(const uint8_t *data, size_t size)
{
	/* Ensure a minimum data length */
	if (size < 100) return;

	FuzzedDataProvider fuzzed_data(data, size);

	/* 1. Generate manipulated BGENV.DAT */
	BG_ENVDATA env;
	memset(&env, 0, sizeof(data));
	fuzzed_data.ConsumeData(env.kernelfile, sizeof(env.kernelfile));
	fuzzed_data.ConsumeData(env.kernelparams, sizeof(env.kernelparams));
	env.in_progress = fuzzed_data.ConsumeIntegral<uint8_t>();
	env.ustate = fuzzed_data.ConsumeIntegral<uint8_t>();
	env.watchdog_timeout_sec = fuzzed_data.ConsumeIntegral<uint16_t>();
	env.revision = fuzzed_data.ConsumeIntegral<uint32_t>();
	fuzzed_data.ConsumeData(env.userdata, sizeof(env.userdata));

	/* 2. Write BGENV to temp file */
	char envfilepath[] = "/tmp/BGENV.XXXXXX";
	int fd = mkstemp(envfilepath);
	if (fd == -1) {
		throw "Error creating temporary BGENV";
	}
	FILE *of = fdopen(fd, "wb");
	if (fwrite(&env, sizeof(env), 1, of) != 1) {
		throw "Error writing BGENV";
	}
	fclose(of);

	/* 3. Call the fuzz targets */
	memset(&env, 0, sizeof(data));
	get_env(envfilepath, &env);
	const struct fields ALL_FIELDS = {1, 1, 1, 1, 1, 1, 1};
	dump_env(&env, &ALL_FIELDS, false);

	unlink(envfilepath);
}
