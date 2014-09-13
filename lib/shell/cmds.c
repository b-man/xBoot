/*
 * Copyright 2013, winocm. <winocm@icloud.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 *   If you are going to use this software in any form that does not involve
 *   releasing the source to this project or improving it, let me know beforehand.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cmd.h>
#include <stdio.h>

int command_help(int argc, char* argv[]);
int command_halt(int argc, char* argv[]);
int command_reset(int argc, char *argv[]);
int command_getenv(int argc, char* argv[]);
int command_setenv(int argc, char* argv[]);
int command_printenv(int argc, char* argv[]);

/* Command dispatch. */
command_dispatch_t gDispatch[] = {
    {"help", command_help, "display this list"},
    {"halt", command_halt, "halt the system"},
    {"reset", command_reset, "reset the system"},
/*
    {"mws", command_mws, "memory write - string"},
    {"mwb", command_mwb, "memory write - 8bit"},
    {"mwh", command_mwh, "memory write - 16bit"},
    {"mw", command_mw, "memory write - 32bit"},
    {"mdb", command_mdb, "memory display - 8bit"},
    {"mdh", command_mdh, "memory display - 16bit"},
    {"md", command_md, "memory display - 32bit"},
*/
    {"setenv", command_setenv, "set an environment variable"},
    {"getenv", command_getenv, "get an environment variable over console"},
    {"printenv", command_printenv, "print one or all environment variables"},
    {NULL, NULL, NULL},
};
