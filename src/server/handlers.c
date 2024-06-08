// handlers.c

#include "handlers.h"
#include "file_reader.h"
#include "xml_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

static FileData *files = NULL;
static int file_count = 0;

void handle_read_files(const char *dir_path, int numa_node) {
    files = read_directory(dir_path, numa_node, &file_count);
    if (files) {
        printf("Files read successfully. Total files: %d\n", file_count);
    } else {
        fprintf(stderr, "Failed to read files from directory: %s\n", dir_path);
    }
}

void handle_parse_xml() {
    if (!files) {
        fprintf(stderr, "No files available for parsing.\n");
        return;
    }

    #pragma omp parallel for
    for (int i = 0; i < file_count; i++) {
        // Assuming parse_xml_file is a function in xml_parser.c
        parse_xml_file(files[i].data);
    }
    printf("XML parsing completed for %d files.\n", file_count);
}
