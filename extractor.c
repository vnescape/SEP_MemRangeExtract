#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 1024

int main(int argc, char *argv[]) {
    // Check if a filename was provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Attempt to open the file in read mode
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Variables for reading and searching
    char buffer[CHUNK_SIZE];
    size_t bytesRead;
    const char *searchString = "mard"; // "dram" in reverse
    size_t searchLength = strlen(searchString);
    long position = 0;
    int found = 0;

    // Read through the file in chunks
    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, file)) > 0) {
        for (size_t i = 0; i <= bytesRead - searchLength; i++) {
            if (memcmp(&buffer[i], searchString, searchLength) == 0) {
                printf("Found '%s' at byte position: 0x%lx\n", searchString, position + i);
                found = 1;
            }
        }
        position += bytesRead;
    }

    // Close the file
    fclose(file);

    // If not found, print a message
    if (!found) {
        printf("'%s' not found in the file.\n", searchString);
    }

    return EXIT_SUCCESS;
}
