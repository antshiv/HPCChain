#define _GNU_SOURCE
#include "file_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <numa.h>
#include <numaif.h>
#include <omp.h>
#include <immintrin.h>

// Define cache line size for alignment
#define CACHE_LINE_SIZE 64

// Function to read a single file
FileData read_file(const char *file_path, int numa_node) {
    FileData file_data = {NULL, 0};
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Failed to open file");
        return file_data;
    }

    fseek(file, 0, SEEK_END);
    file_data.size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory on the specified NUMA node and align to cache line size
    posix_memalign((void **)&file_data.data, CACHE_LINE_SIZE, file_data.size + 1);
    if (!file_data.data) {
        perror("Failed to allocate memory");
        fclose(file);
        return file_data;
    }

    // Bind the allocated memory to the specified NUMA node
    if (mbind(file_data.data, file_data.size + 1, MPOL_BIND, &numa_node, sizeof(numa_node) * 8, 0) != 0) {
        perror("Failed to bind memory to NUMA node");
        free(file_data.data);
        fclose(file);
        return file_data;
    }

    fread(file_data.data, 1, file_data.size, file);
    file_data.data[file_data.size] = '\0';

    fclose(file);
    return file_data;
}

// Function to read all files in a directory
FileData* read_directory(const char *dir_path, int numa_node, int *file_count) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Failed to open directory");
        *file_count = 0;
        return NULL;
    }

    struct dirent *entry;
    struct stat statbuf;
    FileData *files = NULL;
    *file_count = 0;

    // Collect file names first
    char **file_paths = NULL;

    while ((entry = readdir(dir)) != NULL) {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);

        if (stat(file_path, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
            file_paths = (char **)realloc(file_paths, (*file_count + 1) * sizeof(char *));
            file_paths[*file_count] = strdup(file_path);
            (*file_count)++;
        }
    }

    closedir(dir);

    // Allocate space for FileData structures
    files = (FileData *)malloc(*file_count * sizeof(FileData));

    // Parallel reading of files
    #pragma omp parallel for
    for (int i = 0; i < *file_count; i++) {
        files[i] = read_file(file_paths[i], numa_node);
        free(file_paths[i]);  // Free the duplicated file path string

        // Prefetch the data to L1 cache
        for (size_t j = 0; j < files[i].size; j += CACHE_LINE_SIZE) {
            _mm_prefetch(&files[i].data[j], _MM_HINT_T0);
        }
    }

    free(file_paths);  // Free the array of file paths

    return files;
}

// Function to free file data
void free_file_data(FileData *file_data, int file_count) {
    for (int i = 0; i < file_count; i++) {
        numa_free(file_data[i].data, file_data[i].size + 1);
    }
    free(file_data);
}
