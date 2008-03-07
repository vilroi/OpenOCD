/***************************************************************************
 *   Copyright (C) 2005 by Dominic Rath                                    *
 *   Dominic.Rath@gmx.de                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef ERROR_H
#define ERROR_H

#include "replacements.h"
#include "command.h"

#include <stdarg.h>

/* logging priorities 
 * LOG_USER - user messages. Could be anything from information 
 *            to progress messags. These messages do not represent
 *            incorrect or unexpected behaviour, just normal execution. 
 * LOG_ERROR - fatal errors, that are likely to cause program abort
 * LOG_WARNING - non-fatal errors, that may be resolved later
 * LOG_INFO - state information, etc.
 * LOG_DEBUG - debug statements, execution trace
 */
enum log_levels
{
	LOG_OUTPUT = -2,
	LOG_USER = -1,
	LOG_ERROR = 0,
	LOG_WARNING = 1,
	LOG_INFO = 2,
	LOG_DEBUG = 3
};

extern void log_printf(enum log_levels level, const char *file, int line, 
	const char *function, const char *format, ...) 
__attribute__ ((format (printf, 5, 6)));
extern void log_printf_lf(enum log_levels level, const char *file, int line,
	const char *function, const char *format, ...) 
__attribute__ ((format (printf, 5, 6)));
extern int log_register_commands(struct command_context_s *cmd_ctx);
extern int log_init(struct command_context_s *cmd_ctx);
extern int set_log_output(struct command_context_s *cmd_ctx, FILE *output);

typedef void (*log_callback_fn)(void *priv, const char *file, int line,
		const char *function, const char *string);

typedef struct log_callback_s
{
	log_callback_fn fn;
	void *priv;
	struct log_callback_s *next;
} log_callback_t;

extern int log_add_callback(log_callback_fn fn, void *priv);
extern int log_remove_callback(log_callback_fn fn, void *priv);

char *alloc_vprintf(const char *fmt, va_list ap);
char *alloc_printf(const char *fmt, ...);

extern int debug_level;

/* Avoid fn call and building parameter list if we're not outputting the information.
 * Matters on feeble CPUs for DEBUG/INFO statements that are involved frequently */


#define DEBUG(expr ...) \
		log_printf_lf (LOG_DEBUG, __FILE__, __LINE__, __FUNCTION__, expr)

#define INFO(expr ...) \
		log_printf_lf (LOG_INFO, __FILE__, __LINE__, __FUNCTION__, expr)

#define INFO_N(expr ...) \
		log_printf (LOG_INFO, __FILE__, __LINE__, __FUNCTION__, expr)

#define WARNING(expr ...) \
		log_printf_lf (LOG_WARNING, __FILE__, __LINE__, __FUNCTION__, expr)

#define ERROR(expr ...) \
		log_printf_lf (LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, expr)

#define USER(expr ...) \
		log_printf_lf (LOG_USER, __FILE__, __LINE__, __FUNCTION__, expr)

#define USER_N(expr ...) \
		log_printf (LOG_USER, __FILE__, __LINE__, __FUNCTION__, expr)

#define OUTPUT(expr ...) \
		log_printf (LOG_OUTPUT, __FILE__, __LINE__, __FUNCTION__, expr)


/* general failures
 * error codes < 100
 */
#define ERROR_OK					(0)
#define ERROR_INVALID_ARGUMENTS		(-1)
#define ERROR_NO_CONFIG_FILE		(-2)
#define ERROR_BUF_TOO_SMALL			(-3)

#endif /* LOG_H */
