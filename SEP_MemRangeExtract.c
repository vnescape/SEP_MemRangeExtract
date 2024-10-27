#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#define CHUNK_SIZE 1024
#define DEBUG false // Set this to true or false

// Define the struct
struct phys_range {
    char ctx[4];
    char name[4];
    uint64_t start;
    uint32_t size;
    uint32_t unk;
};

// Ensure the struct size is correct
static_assert(sizeof(struct phys_range) == 0x18, "struct phys_range size must be 0x18");

// Debug print macro
#if DEBUG
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG]: " fmt, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif

// Function to reverse a string in place
void reverse_string(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        // Swap characters
        char temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }
}

bool is_struct_valid(const struct phys_range* mem_struct) {
    // Check if ctx and name are valid ASCII characters
    for (int i = 0; i < 4; i++) {
        if (!isprint((unsigned char)mem_struct->ctx[i]) || !isprint((unsigned char)mem_struct->name[i])) {
            return false;
        }
    }

    // Check if start and size are positive
    if (mem_struct->start <= 0 || mem_struct->size <= 0) {
        return false;
    }

    return true; // All conditions met
}

void process_structs(FILE *file, uint64_t *position, int count, int backwards) {
    for (int i = 0; i < count; i++) {
        struct phys_range entry;
        fseek(file, *position, SEEK_SET);

        if (fread(&entry, sizeof(struct phys_range), 1, file) == 1) {
            reverse_string(entry.ctx);
            if(!is_struct_valid(&entry)) { continue; }
            printf("Read struct: ctx=%.4s, name=%.4s, start=0x%llx, size=0x%x, unk=0x%x\n",
                   entry.ctx, entry.name, entry.start, entry.size, entry.unk);
            *position += backwards ? -sizeof(struct phys_range) : sizeof(struct phys_range); // Adjust position
        } else {
            fprintf(stderr, "Error reading struct at position: 0x%llx\n", *position);
            break; // Exit if there's an error
        }
    }
}

int main(int argc, char *argv[]) {
    // Check if a filename was provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filepath to decrypted Apple SEP image>\n", argv[0]);
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

    uint64_t old_position = 0;
    uint64_t new_position = 0;

    // Read through the file in chunks
    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, file)) > 0) {
        for (size_t i = 0; i <= bytesRead - searchLength; i++) {
            if (memcmp(&buffer[i], searchString, searchLength) == 0) {
                DEBUG_PRINT("Found '%s' at byte position: 0x%lx\n", searchString, position + i);
                old_position = new_position;
                new_position = position + i;
                DEBUG_PRINT("Distance to new_position from old position: 0x%llx\n", new_position - old_position);
                
                // Seek to the position of the found string to read the struct
                fseek(file, position + i, SEEK_SET);
                struct phys_range entry;

                // Read the struct from the file
                if (fread(&entry, sizeof(struct phys_range), 1, file) == 1) {
                    reverse_string(entry.ctx);
                    
                    // Check if the current struct is valid
                    if (!is_struct_valid(&entry)) {
                        DEBUG_PRINT("Current struct at position 0x%zx is invalid. Checking adjacent structs.\n", position + i);
                    }

                    // Check for the previous struct if not at the start
                    struct phys_range prev_entry;
                    bool prev_valid = false;
                    if (new_position > sizeof(struct phys_range)) {
                        fseek(file, new_position - sizeof(struct phys_range), SEEK_SET);
                        if (fread(&prev_entry, sizeof(struct phys_range), 1, file) == 1) {
                            prev_valid = is_struct_valid(&prev_entry);
                            if (prev_valid) {
                                DEBUG_PRINT("Previous struct at position 0x%llx is valid.\n", new_position - sizeof(struct phys_range));
                            } else {
                                DEBUG_PRINT("Previous struct at position 0x%llx is invalid.\n", new_position - sizeof(struct phys_range));
                            }
                        }
                    }

                    // Check for the next struct, if it is at the end of the file we crash, should never happen
                    struct phys_range next_entry;
                    bool next_valid = false;
                    fseek(file, new_position + sizeof(struct phys_range), SEEK_SET);
                    if (fread(&next_entry, sizeof(struct phys_range), 1, file) == 1) {
                        next_valid = is_struct_valid(&next_entry);
                        if (next_valid) {
                            DEBUG_PRINT("Next struct at position 0x%llx is valid.\n", new_position + sizeof(struct phys_range));
                        } else {
                            DEBUG_PRINT("Next struct at position 0x%llx is invalid.\n", new_position + sizeof(struct phys_range));
                        }
                    } else {
                        DEBUG_PRINT("No next struct found at position 0x%llx.\n", new_position + sizeof(struct phys_range));
                    }

                    // Proceed if either the current, previous, or next struct is valid
                    if (is_struct_valid(&entry) || prev_valid || next_valid) {
                        printf("Read struct: ctx=%.4s, name=%.4s, start=0x%llx, size=0x%x, unk=0x%x\n",
                            entry.ctx, entry.name, entry.start, entry.size, entry.unk);
                        
                        // Break after finding a valid struct
                        found = 1;
                        break; // Exit the for loop
                    } else {
                        DEBUG_PRINT("All structs at position 0x%zx are invalid. Skipping.\n", position + i);
                    }
                } else {
                    fprintf(stderr, "Error reading struct at position: 0x%lx\n", position + i);
                }
            }
        }
        if (found) {
            break; // Exit the while loop if a valid struct has been found
        }
        position += bytesRead;
    }

    uint64_t new_position_reverse = new_position;
    // Process backward structs
    if (found) {
        process_structs(file, &new_position_reverse, 30, 1); // 1 for backwards
    }

    // Process forward structs
    if (found) {
        process_structs(file, &new_position, 30, 0); // 0 for forwards
    }

    // Close the file
    fclose(file);

    // If not found, print a message
    if (!found) {
        printf("'%s' not found in the file.\n", searchString);
    }

    return EXIT_SUCCESS;
}
