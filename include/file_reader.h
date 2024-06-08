// file_reader.h
// file_reader.h

#ifndef FILE_READER_H
#define FILE_READER_H

#include <stddef.h>

// Struct to hold file data
typedef struct {
    char *data;
    size_t size;
} FileData;

// Function to read a single file
FileData read_file(const char *file_path, int numa_node);

// Function to read all files in a directory
FileData* read_directory(const char *dir_path, int numa_node, int *file_count);

// Function to free file data
void free_file_data(FileData *file_data, int file_count);

#endif // FILE_READER_H
