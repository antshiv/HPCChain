// main.c

#include <stdio.h>
#include <sys/stat.h>
#include "file_reader.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <file_or_directory_path> <numa_node>\n", argv[0]);
        return 1;
    }

    struct stat path_stat;
    stat(argv[1], &path_stat);
    int numa_node = atoi(argv[2]);

    if (S_ISREG(path_stat.st_mode)) {
        // Read single file
        FileData file_data = read_file(argv[1], numa_node);
        if (file_data.data) {
            printf("File content:\n%s\n", file_data.data);
            free_file_data(&file_data, 1);
        }
    } else if (S_ISDIR(path_stat.st_mode)) {
        // Read all files in directory
        int file_count;
        FileData *files = read_directory(argv[1], numa_node, &file_count);
        for (int i = 0; i < file_count; i++) {
            printf("File %d content:\n%s\n", i, files[i].data);
        }
        free_file_data(files, file_count);
    } else {
        fprintf(stderr, "Invalid file or directory path.\n");
        return 1;
    }

    return 0;
}
