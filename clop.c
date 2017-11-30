#include "clop.h"

char* clop_default_formatter (
    char* logname, char* message,
    clop_log_level level, int verb, char* timestamp_fmt
);

void clop_default_writer (
    char* dir, char* logname,
    char* data, int verb
);

clop_logger* clop_new(char* name, char* log_location, char* timestamp_fmt) {
    clop_logger* logger = (clop_logger*)malloc(sizeof(clop_logger));

    logger->name = name;
    logger->timestamp_fmt = timestamp_fmt;
    logger->format = clop_default_formatter;
    logger->write = clop_default_writer;
    logger->log_location = log_location;

    return logger;
}

void clop_log (
    clop_logger* logger, clop_log_level level,
    int verb, char* fmt, ...
) {
    va_list args,argsc;
    va_start(args, fmt);

    int found_length = 0;
    int need = 0;
    char *out = NULL;

    clop_log_allocate_output_buffer: {
        if (out == NULL)
            out = (char*)malloc(need);
        else
            out = (char*)realloc(out, need);

        memset(out, 0, need);
        va_copy(argsc, args);
        need = vsnprintf (
            out,
            need,
            fmt,
            argsc
        ) + 1;

        if (! found_length) {
            found_length = 1;
            goto clop_log_allocate_output_buffer;
        }
    }

    va_end(args);

    char* msg = logger->format(
        logger->name, out, level, verb,
        logger->timestamp_fmt
    );

    free(out);

    logger->write(logger->log_location, logger->name, msg, verb);
    free(msg);
}

char* clop_default_formatter (
    char* logname, char* message, clop_log_level level,
    int verb, char* timestamp_fmt
) {
    int found_length = 0;
    int need = 0;
    char *out = NULL;

    char* formatted_date = malloc(128);

    time_t timestamp = time(NULL);
    strftime(formatted_date, 128, timestamp_fmt, localtime(&timestamp));

    default_output_allocate_output_buffer: {
        if (out == NULL)
            out = (char*)malloc(need);
        else
            out = (char*)realloc(out, need);

        memset(out, 0, need);

        need = snprintf (
            out,
            need,
            CLOP_OUT_FMT,
            formatted_date,
            logname,
            verb,
            clop_level_as_string(level),
            message
        ) + 1;

        if (! found_length) {
            found_length = 1;
            goto default_output_allocate_output_buffer;
        }
    }

    free(formatted_date);
    return out;
}

void clop_default_writer (char* dir, char* logname, char* data, int verb) {
    printf("%s\n", (char*)data);

    if (dir != NULL) {
        while (verb-1>-1) {
            char* filename = malloc(4096);
            sprintf(filename, CLOP_LOG_NAME_FMT, dir, logname, verb);

            FILE* fh = fopen(filename, "w");
            if (fh == NULL) {
                printf(
                    "%s",
                    "CLOP: WARN: Failed to write to log file, NULL handle.\n"
                );
                printf("CLOP: WARN: Attempted to save to file: %s\n", filename);
                return;
            }

            fprintf(fh, "%s\n", (char*)data);
            fclose(fh);

            free(filename);
            verb = verb - 1;
        }
    }
}

char* clop_level_as_string(int level) {
    switch(level) {
        case Clop_LL_Error:
            return "ERRO";
        case Clop_LL_Warning:
            return "WARN";
        case Clop_LL_Information:
            return "INFO";
        case Clop_LL_Internal:
            return "ITRN";
        case Clop_LL_Debug:
            return "DEBG";

        default:
            return "UNKN";
    }
}
