// handlers.h

#ifndef HANDLERS_H
#define HANDLERS_H

#include "file_reader.h"

// Handler for reading files
void handle_read_files(const char *dir_path, int numa_node);

// Handler for parsing XML files
void handle_parse_xml();

#endif // HANDLERS_H
