/* Environment variable handling for xBoot
 *
 * Copyright (c) 2014, Brian McKenzie <mckenzba@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the organization nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <interface/nvram.h>

static nvram_variable_list_t *gNvramVariables;
static bool environment_initialized = FALSE;

int env_init(void)
{
	if (environment_initialized == TRUE)
		return -1;

	gNvramVariables = nvram_initialize_list();
	environment_initialized = TRUE;

	return 0;
}

char *getenv(const char *var)
{
	char *setting;
	nvram_variable_t *variable = nvram_read_variable_info(gNvramVariables, var);

	setting = variable->setting;

	if (setting != NULL)
		return setting;

	return NULL;
}

int setenv(const char *var, const char *val, int overwrite)
{
	nvram_variable_t *variable = nvram_read_variable_info(gNvramVariables, var);

	if ((overwrite == 0) && (strlen(variable->name) >= 1))
		return 0;

	nvram_variable_set(gNvramVariables, var, val);

	return 0;
}

int unsetenv(const char *var)
{
	return nvram_variable_unset(gNvramVariables, var);
}

void printenv(void)
{
	nvram_dump_list(gNvramVariables);

	return;
}
