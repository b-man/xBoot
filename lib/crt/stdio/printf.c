/* Basic printf for xBoot
 *
 * Copyright (c) 2017, Brian McKenzie <mckenzba@gmail.com>
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

/*
 * Based on public-domain printf by Chris Giese	<geezer@execpc.com> http://my.execpc.com/~geezer (Rev. 2/16/14)
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <device/uart.h>

/* putc function to use */
static void (*putchar)(int c);
boolean_t printf_initialized = FALSE;

/* display pointers in upper-case hex (A-F) instead of lower-case (a-f) */
#define	PRINTF_UCP	1

/* flags used in processing format string */
#define	PR_LEFTJUST	0x01	/* left justify */
#define	PR_UC		0x02	/* use upper case (A-F vs. a-f) for hex */
#define	PR_DO_SIGN	0x04	/* signed numeric conversion (%d vs. %u) */
#define	PR_LONG		0x08	/* long numeric conversion (%lu) */
#define	PR_SHORT	0x10	/* short numeric conversion (%hu) */
#define	PR_NEGATIVE	0x20	/* PR_DO_SIGN set and num was < 0 */
#define	PR_PADLEFT0	0x40	/* pad left with '0' instead of ' ' */
#define	PR_FARPTR	0x80	/* pointers are far */

/* Largest number handled is 2^32-1, lowest radix handled is 8.
 * 2^32-1 in base 8 has 11 digits (add 5 for trailing NUL and for slop).
 * Strings are copied directly to the output, and don't use 'buf'.
 */
#define PR_BUFLEN       16

typedef int (*fnptr_t)(unsigned int c, void **helper);

/**
 * num2asc - Converts binary numeric value 'num' to ASCII string.
 *
 * @arg buf: pointer to a zeroed byte at the END of a suitably-large buffer
 * @arg radix: base for the number (i.e 10 for integer, 16 for hex)
 * @arg num: binary numeric value
 * @arg upper_case: display in upper or lower case if possible
 * @return: pointer to the START of the string stored in 'buf'
 */
static char *num2asc(char *buf, unsigned int radix, unsigned long num, int upper_case)
{
	unsigned long i;

	/* do..while() instead of while() ensures we get a single '0' if num==0 */
	do {
		i = (unsigned long)num % radix;
		buf--;
		if (i < 10)
			*buf = i + '0';
		else {
			if (upper_case)
				*buf = i - 10 + 'A';
			else
				*buf = i - 10 + 'a';
		}
		num = (unsigned long)num / radix;
	} while (num != 0);

	return buf;
}

/**
 * do_printf - Minimal subfunction for ?printf
 *
 * @arg fmt: formatted string to parse
 * @arg args: arguments passed to ?printf
 * @arg fn: function to call while processing string
 * @arg ptr: pointer to argument to pass to 'fn'
 * @return: total number of characters output
 */
static int do_printf(const char *fmt, va_list args, fnptr_t fn, void *ptr)
{
	int i;
	long num;
	char *where;
	char state, radix, buf[PR_BUFLEN];
	unsigned int flags, actual_wd, given_wd, count;

	state = count = given_wd = flags = 0;

	if (fmt == NULL)
		serial_puts("NULL\n");

	for (; *fmt; fmt++) {
		switch(state) {
		    /* STATE 0: AWAITING '%' */
		    case 0:

			/* echo text until '%' seen */
			if (*fmt != '%') {
				if ((i = fn(*fmt, &ptr)) == EOF)
					return i;

				count++;
				break;
			}

			/* found %, get next char and advance state to check if next char is a flag */
			fmt++;
			state++;
			flags = 0;
			/* FALL THROUGH */

		    case 1: /* STATE 1: AWAITING FLAGS ('%' or '-' or '0') */

			if (*fmt == '%') { /* %% */
				if((i = fn(*fmt, &ptr)) == EOF)
					return i;

				count++;
				state = 0;
				break;
			}

			if (*fmt == '-') {
				flags |= PR_LEFTJUST;
				break;
			}

			if (*fmt == '0') {
				flags |= PR_PADLEFT0;
				/* '0' could be flag or field width -- fall through */
				fmt++;
			}

			/* '0' or not a flag char: advance state to check if it's field width */
			state++;
			given_wd = 0;
			/* FALL THROUGH */

		    case 2: /* STATE 2: AWAITING (NUMERIC) FIELD WIDTH */

			if (*fmt >= '0' && *fmt <= '9')
			{
				given_wd = 10 * given_wd + (*fmt - '0');
				break;
			}

			/* not field width: advance state to check if it's a modifier */
			state++;
			/* FALL THROUGH */

	    	    case 3: /* STATE 3: AWAITING MODIFIER CHARACTERS ('F', 'N', 'l', or 'h') */

			if (*fmt == 'F') {
				flags |= PR_FARPTR;
				break;
			}

			if (*fmt == 'N') { /* near pointer (the default) */
				flags &= ~PR_FARPTR;
				break;
			}

			if (*fmt == 'l') {
				flags |= PR_LONG;
				break;
			}

			if (*fmt == 'h') {
				flags |= PR_SHORT;
				break;
			}

			/* not a modifier: advance state to check if it's a conversion char */
			state++;
			/* FALL THROUGH */

		    case 4: /* STATE 4: AWAITING CONVERSION CHARACTER ('X' 'x' 'p' 'n' 'd' 'i' 'u' 'o' 'c' 's') */

			where = &buf[PR_BUFLEN - 1];
			*where = '\0';

			/* 'h' and 'l' modifiers cancel each other out */
			if ((flags & (PR_LONG | PR_SHORT)) == (PR_LONG | PR_SHORT))
				flags &= ~(PR_LONG | PR_SHORT);

			/* pointer and numeric conversions */
			if (*fmt == 'p' || *fmt == 'n' || *fmt == 'X' || *fmt == 'x' || *fmt == 'd' || *fmt == 'i' || *fmt == 'u' || *fmt == 'o') {
				if (*fmt == 'X') {
					flags |= PR_UC;
					flags &= ~PR_DO_SIGN;
					radix = 16; /* display pointers in hex */
				} else if (*fmt == 'x' || *fmt == 'p' || *fmt == 'n') {
					flags &= ~PR_UC;
					flags &= ~PR_DO_SIGN;
					radix = 16;
				} else if (*fmt == 'd' || *fmt == 'i') {
					flags |= PR_DO_SIGN;
					radix = 10;
				} else if (*fmt == 'u') {
					flags &= ~PR_DO_SIGN;
					radix = 10;
				} else { /*if (*fmt == 'o')*/
					flags &= ~PR_DO_SIGN;
					radix = 8;
				}

				/* load the value to be printed. 'l'=long (usu. 32 bits)... */
				if (flags & PR_LONG) {
					if (flags & PR_DO_SIGN)
						num = (signed long )va_arg(args, signed long);
					else
						num = (unsigned long)va_arg(args, unsigned long);
				} else if(flags & PR_SHORT) { /* ...'h'=short (usu. 16 bits) */
					if (flags & PR_DO_SIGN)
						num = (signed short)va_arg(args, signed int);
					else
						num = (unsigned short)va_arg(args, unsigned int);
				} else { /* no 'h' nor 'l': sizeof(int) bits */
					if (flags & PR_DO_SIGN)
						num = (signed int)va_arg(args, signed int);
					else
						num = (unsigned int)va_arg(args, unsigned int);
				}

				/* convert to unsigned but remember that it was negative */
				if ((flags & PR_DO_SIGN) && num < 0) {
					flags |= PR_NEGATIVE;
					num = -num;
				}

				/* convert binary unsigned number to octal/decimal/hex ASCII */
				where = num2asc(where, radix, num, flags & PR_UC);

			} else if (*fmt == 'c') {
				/* disallow these modifiers for %c */
				flags &= ~(PR_DO_SIGN | PR_NEGATIVE | PR_PADLEFT0);
				/* yes; we're converting a character to a string here: */
				where--;
				*where = (unsigned char)va_arg(args, unsigned int);
			} else if (*fmt == 's') {
				/* disallow these modifiers for %s */
				flags &= ~(PR_DO_SIGN | PR_NEGATIVE | PR_PADLEFT0);
				where = va_arg(args, char *);
			} else { /* bogus conversion character -- copy it to output and go back to state 0 */
				if ((i = fn(*fmt, &ptr)) == EOF)
					return i;

				count++;
				state = flags = given_wd = 0;
				break;
			}

			/* emit formatted string */
			actual_wd = strlen(where);
			if (flags & PR_NEGATIVE)
				actual_wd++;

			/* if we pad left with ZEROES, do the sign now (for numeric values; not for %c or %s) */
			if ((flags & (PR_NEGATIVE | PR_PADLEFT0)) == (PR_NEGATIVE | PR_PADLEFT0)) {
				if ((i = fn('-', &ptr)) == EOF)
					return i;

				count++;
			}

			/* pad on left with spaces or zeroes (for right justify) */
			if ((flags & PR_LEFTJUST) == 0) {
				for (; given_wd > actual_wd; given_wd--) {
					if ((i = fn(flags & PR_PADLEFT0 ? '0' : ' ', &ptr)) == EOF)
						return i;
					count++;
				}

				/* if we pad left with SPACES, do the sign now */
				if (flags & PR_NEGATIVE) {
					if((i = fn('-', &ptr)) == EOF)
						return i;

					count++;
				}
			}

			/* emit converted number/char/string */
			for (; *where != '\0'; where++) {
				if ((i = fn(*where, &ptr)) == EOF)
					return i;

				count++;
			}

			/* pad on right with spaces (for left justify) */
			if (given_wd < actual_wd)
				given_wd = 0;
			else
				given_wd -= actual_wd;

			for (; given_wd; given_wd--) {
				if ((i = fn(' ', &ptr)) == EOF)
					return i;
				count++;
			}

			/* FALL THROUGH */

		    default:

			state = flags = given_wd = 0;
			break;
		}
	}

	return count;
}

/**
 * printf_init - ?printf initialization function
 *
 * @arg putc - pointer to putc function
 */
void printf_init(void (*putc)(int c))
{
	if (printf_initialized == TRUE)
		return;

	putchar = putc;
	printf_initialized = TRUE;

	return;
}

/**
 * vsprintf_help - Helper function for vsprintf
 */
static int vsprintf_help(unsigned int c, void **ptr)
{
	char *dst;

	dst = *ptr;
	*dst++ = (char)c;
	*ptr = dst;

	return 0;
}

/**
 * discard - dummy function
 */
static int discard(unsigned int c_UNUSED, void **ptr_UNUSED)
{
	return 0;
}

/**
 * vsprintf - Print formatted output of a stdarg argument list to a buffer
 *
 * @arg buf: pointer to string output
 * @arg fmt: formatted string
 * @arg args: arguments passed to vsprintf
 * @return: number of characters printed
 */
int vsprintf(char *buf, const char *fmt, va_list args)
{
	int rv;

	if (buf == NULL)
		rv = do_printf(fmt, args, discard, NULL);
	else {
		rv = do_printf(fmt, args, vsprintf_help, (void *)buf);
		buf[rv] = '\0';
	}

	return rv;
}

/**
 * sprintf - Print formatted output to a buffer
 *
 * @arg buf: buffer for output
 * @arg fmt: formatted string
 * @arg ...: any additional arguments
 * @return: number of characters printed
 */
int sprintf(char *buf, const char *fmt, ...)
{
	int rv;
	va_list args;

	va_start(args, fmt);
	rv = vsprintf(buf, fmt, args);
	va_end(args);

	return rv;
}

/* asprintf struct */
typedef struct {
	char *buf;
	unsigned int alloc;
	unsigned int ptr;
} asprintf_t;

/**
 * vasprintf_help - Helper function for vasprintf
 */
static int vasprintf_help(unsigned int c, void **ptr)
{
	char *t;
	asprintf_t *as;
	unsigned int len;

	as = *ptr;

	if (as->ptr + 1 >= as->alloc) {
		/* start with 16 bytes, then double the string length every time it overflows */
		len = (as->alloc == 0) ? 16 : (as->alloc * 2);
		if ((t = realloc(as->buf, len)) == NULL)
			return EOF;

		as->buf = t;
		as->alloc = len;
	}

	as->buf[as->ptr] = c;
	as->ptr++;

	return 0 ;
}

/**
 * vasprintf - Print formatted output of a stdarg argument list to an allocated string
 *
 * @arg dst_ptr: pointer to allocated string
 * @arg fmt: formatted string
 * @arg args: arguments passed to vasprintf
 * @return: number of characters printed
 */
int vasprintf(char **dst_ptr, const char *fmt, va_list args)
{
	int i;
	asprintf_t *as = NULL;

	as->buf = NULL;
	as->alloc = as->ptr = 0;
	i = do_printf(fmt, args, vasprintf_help, as);

	if (i == EOF) {
		if (as->buf != NULL)
			free(as->buf);

		*dst_ptr = NULL; /* FreeBSD does this */
	} else {
		as->buf[as->ptr] = '\0';
		*dst_ptr = as->buf;
	}

	return i;
}

/**
 * asprintf - Print formatted output to allocated string
 *
 * @arg dst_ptr: pointer to allocated string
 * @arg fmt: formatted string
 * @arg ...: any additional arguments
 * @return: number of characters printed
 */
int asprintf(char **dst_ptr, const char *fmt, ...)
{
	int rv;
	va_list args;

	va_start(args, fmt);
	rv = vasprintf(dst_ptr, fmt, args);
	va_end(args);

	return rv;
}

/**
 * vprintf_help - Helper function for vprintf
 */
int vprintf_help(unsigned int c, void **ptr_UNUSED)
{
	putchar(c);

	return 0;
}

/**
 * vprintf - Print formatted output of an stdarg argument list
 *
 * @arg fmt: formatted string
 * @arg: args: argument list
 * @return: number of characters printed
 */
int vprintf(const char *fmt, va_list args)
{
	return do_printf(fmt, args, vprintf_help, NULL);
}

/**
 * printf - Print formatted output
 *
 * @arg fmt: formatted string
 * @arg ...: any additional arguments
 * @return: number of characters printed
 */
int printf(const char *fmt, ...)
{
	int rv;
	va_list args;

	va_start(args, fmt);
	rv = vprintf(fmt, args);
	va_end(args);

	return rv;
}
