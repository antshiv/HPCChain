// router.c
#include <stdio.h>
#include "router.h"
#include "handlers.h"

void route_request(const char* request, int client_socket) {
    if (strstr(request, "GET /read_files") != NULL) {
        handle_read_files("/path/to/directory", 0); // Adjust parameters as needed
    } else if (strstr(request, "GET /parse_xml") != NULL) {
        handle_parse_xml();
    } else {
        fprintf(stderr, "Unknown request: %s\n", request);
    }
}
