#include "clop.h"

/* Begin Configuration */

int use_default_write = 0;
int use_default_format = 1;

char* logger_name = "Primary";
char* logger2_name = "Secondary";
char* log_location = NULL;

/* End Configuration */

void custom_writer (char* dir, char* logname, char* data, int verb);
char* custom_formatter (char* logname, char* message, clop_log_level level, int verb, char* timestamp_fmt);

int main() {
    clop_logger* logger = clop_new(logger_name, log_location, CLOP_TIME_FMT);
    clop_logger* logger2 = clop_new(logger2_name, log_location, CLOP_TIME_FMT);

    if (! use_default_write) {
        logger->write = custom_writer;
        logger2->write = custom_writer;
    }

    if (! use_default_format) {
        logger->format = custom_formatter;
        logger2->format = custom_formatter;
    }

    printf("Testing Logger For: %s\n\n", logger->name);

    clop_log(logger, Clop_LL_Error, 1, "Hello, %s, this is a(n) error message    (%i), why not add more? : %i", "Nathan", Clop_LL_Error, 3);
    clop_log(logger, Clop_LL_Warning, 2, "Hello, %s, this is a(n) warning message  (%i)", "Nathan", Clop_LL_Warning);
    clop_log(logger, Clop_LL_Information, 3, "Hello, %s, this is a(n) info message     (%i)", "Nathan", Clop_LL_Information);
    clop_log(logger, Clop_LL_Internal, 4, "Hello, %s, this is a(n) internal message (%i)", "Nathan", Clop_LL_Internal);
    clop_log(logger, Clop_LL_Debug, 5, "Hello, %s, this is a(n) debug message    (%i)", "Nathan", Clop_LL_Debug);

    printf("%s", "\n");
    printf("Testing Logger For: %s\n\n", logger2->name);

    clop_log(logger2, Clop_LL_Error, 1, "Hello, %s, this is a(n) error message    (%i), why not add more? : %i", "Nathan", Clop_LL_Error, 3);
    clop_log(logger2, Clop_LL_Warning, 2, "Hello, %s, this is a(n) warning message  (%i)", "Nathan", Clop_LL_Warning);
    clop_log(logger2, Clop_LL_Information, 3, "Hello, %s, this is a(n) info message     (%i)", "Nathan", Clop_LL_Information);
    clop_log(logger2, Clop_LL_Internal, 4, "Hello, %s, this is a(n) internal message (%i)", "Nathan", Clop_LL_Internal);
    clop_log(logger2, Clop_LL_Debug, 5, "Hello, %s, this is a(n) debug message    (%i)", "Nathan", Clop_LL_Debug);

    free(logger);
    free(logger2);
}

void custom_writer (char* dir, char* logname, char* data, int verb) {
    printf("%s\n", data);
}

char* custom_formatter (char* logname, char* message, clop_log_level level, int verb, char* timestamp_fmt) {
    char* out = malloc(256);
    memcpy(out, message, 256);
    return out;
}
