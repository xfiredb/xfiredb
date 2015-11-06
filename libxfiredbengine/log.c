/*
 *  LOG library
 *  Copyright (C) 2015   Michel Megens <dev@michelmegens.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @addtogroup log
 * @{
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <xfiredb/xfiredb.h>
#include <xfiredb/types.h>
#include <xfiredb/log.h>

static FILE *xfire_stdout = NULL;
static FILE *xfire_stderr = NULL;

/**
 * @brief Initialise XFire logging.
 * @param out stdout file name.
 * @param err stderr file name.
 */
void xfire_log_init(const char *out, const char *err)
{
	if(out)
		xfire_stderr = fopen(err, "w+");
	else
		xfire_stderr = stderr;

	if(err)
		xfire_stdout = fopen(out, "w+");
	else
		xfire_stdout = stdout;
}

/**
 * @brief Close the logging streams.
 */
void xfire_log_exit(void)
{
	raw_xfire_log("[exit]: XFIRE logger stopped.\n");
	fclose(xfire_stderr);
	fclose(xfire_stdout);

}

static void vfxfire_log(const char *src, const char *fmt, va_list args)
{
	fprintf(xfire_stdout, "[%s]: ", src);
	vfprintf(xfire_stdout, fmt, args);
	fflush(xfire_stdout);
}

/**
 * @brief Log a message during the initialisation.
 * @param src Logging source.
 * @param fmt Log format.
 * @param ... Variable argument list.
 */
void xfire_log_console(const char *src, const char *fmt, ...)
{
	va_list args, args2;

	va_start(args, fmt);
	va_copy(args2, args);
	vfxfire_log(src, fmt, args);
	va_end(args);

	if(xfire_stdout != stdout) {
		fprintf(stdout, "[%s]: ", src);
		vfprintf(stdout, fmt, args2);
	}

	va_end(args2);
}

/**
 * @brief Raw error logger.
 * @param msg Message to log.
 */
void raw_xfire_log_err(const char *msg)
{
	fprintf(xfire_stderr, msg);
	fflush(xfire_stderr);
}

/**
 * @brief Raw logger.
 * @param msg Message to log.
 */
void raw_xfire_log(const char *msg)
{
	fprintf(xfire_stdout, msg);
	fflush(xfire_stdout);
}

void raw_xfire_log_console(const char *msg)
{
	if(xfire_stdout != stdout)
		fprintf(stdout, msg);
	raw_xfire_log(msg);
}

/**
 * @brief Log a message.
 * @param src Source of the log (i.e. the subsystem.
 * @param msg Format string.
 * @param ... Variable arguments to match \p msg.
 */
void xfire_log(const char *src, const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	vfxfire_log(src, msg, args);
	va_end(args);
}

/**
 * @brief Log a message.
 * @param src Source of the log (i.e. the subsystem.
 * @param msg Format string.
 * @param ... Variable arguments to match \p msg.
 */
void xfire_log_err(const char *src, const char *msg, ...)
{
	va_list args;

	va_start(args, msg);
	fprintf(xfire_stderr, "[%s]: ", src);
	vfprintf(xfire_stderr, msg, args);
	va_end(args);
}

/** @} */

