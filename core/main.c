/* Main loop
 *
 * Copyright (c) 2013, Brian McKenzie <mckenzba@gmail.com>
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

#include <cmd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <boot/bsp.h>
#include <boot/xboot.h>
#include <interface/timer.h>
#include <interface/serial.h>

void xboot_banner(void)
{
    /* display banner */
    printf("\nxBoot for %s\n", bsp_platform_name);
    printf("Copyright (c) 2014 Brian McKenzie <mckenzba@gmail.com>\n");
    printf("Build: %s\n\n", (char *)__xBoot_version);

    return;
}

void xboot_main(void)
{
    int boot_delay;

    /* Display the banner */
    xboot_banner();

#if 0
    int size = 100;
    char *test_str = "Hello, World!";
    printf("==Start malloc test==\n");
    printf("Allocating %d chars of space for string \"%s\"...\n", size, test_str);
    char *str = (char *)malloc((sizeof(char)*size));
    sprintf(str, "%s", test_str);
    printf("string allocated: \"%s\", size of string is %d chars.\n", str, strlen(str));
    printf("releasing resources...\n");
    free(str);
    printf("===End malloc test===\n\n");
#endif

    /* Delay the boot if necessary */
    printf("Press any key to enter interactive shell.\n");
    for (boot_delay = atoi(getenv("bootdelay")); boot_delay > 0; boot_delay--) {
        printf("Booting in %2d seconds...\r", boot_delay);
        if (serial_poll() != 0)
            command_prompt();
        usleep(1000 * 1000);
    }

    printf("\nnow looping forever.\n");

    int i = '1';
    while (1) {
        if ( i <= '9' ) {
            printf("count: %c\r", i);
            usleep(1000 * 1000);
            ++i;
        } else
            i = '0';
    }

    return;
}
