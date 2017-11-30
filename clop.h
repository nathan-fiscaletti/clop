/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                      CLOP ( A logging library for C )                      */
/*                       Written by Nathan Fiscaletti                         */
/*                                                                            */
/*                           2017.11.30.1512064181                            */
/*             Released on November 30th, 2017 at  9:39AM PST                 */
/*                                                                            */
/*                                  License                                   */ 
/*   http://raw.githubusercontent.com/nathan-fiscaletti/clop/master/LICENSE   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Includes
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

// Format Definitions
#define CLOP_OUT_FMT      "[%s] -- %s -- [Verb: %i] [%s] %s"
#define CLOP_TIME_FMT     "%F %R:%S"
#define CLOP_LOG_NAME_FMT "%s/%s-v%i.log"

// Log Level Definitions
typedef enum clop_log_level {
    Clop_LL_Error       = 0,
    Clop_LL_Warning     = 1,
    Clop_LL_Information = 2,
    Clop_LL_Internal    = 3,
    Clop_LL_Debug       = 4
} clop_log_level;

// The clop_formatter typedef is a function signature used to customize the way 
// that log output is formatted. You can implement your own and assign it to the
// ->format property of your clop_logger instance.
// If you leave the ->format property set to  NULL, the default clop_formatter 
// will be used instead, which will utilize the CLOP_OUT_FMT format property.
typedef char* clop_formatter
( 
    char* logname, char* message, 
    clop_log_level level, int verb, 
    char* timestamp_fmt 
);

// The clop_writer typedef is a function signature used to customize the way
// that log output is written. You can implement your own and assign it to the
// ->write property of your clop_logger instance.
// If you leave the ->write property set to NULL, the default clop_writer will
// be used instead, which will utilize printf() as well as write to log files.
// Each log file written to by the default clop_writer will be separated based
// on the current messages verbosity.
// If log_location is NULL when calling clop_new, no logs will be written.
typedef void  clop_writer        
( 
    char* dir, char* logname, 
    char* data, int verb 
);

// This struct is used to manage loggers.
struct clop_logger {
    clop_formatter* format;
    clop_writer* write;
    char* log_location;
    char* name;
    char* timestamp_fmt;
} typedef clop_logger;

// This function will allocate a new instance of clop_logger using the provided
// name, log location, and timestamp_format. These parameters will be available
// later when you implement a custom writer and formatter.
clop_logger* clop_new
(
    char* name, char* log_location, char* timestamp_fmt
);

// This function will log a message using the logger. When using the default 
// formatter and writer the verbosity will control which log file the message 
// is sent to as well as how the message is formatted and output in the end.
void clop_log
(
    clop_logger* logger, clop_log_level level, 
    int verb, char* fmt, ...               
);

// This function will convert a clop_log_level to it's string representation.
char* clop_level_as_string
( 
    int level
);
