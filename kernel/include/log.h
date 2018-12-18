#ifndef LOG_H
#define LOG_H

enum log_type {
	LOG_NONE,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
};

void log(enum log_type type, const char* data);

#endif