#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to find the length of a null-terminated string
int my_strlen(unsigned char *str) {
    int len = 0;

    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Function to copy a null-terminated string from source to destination
void my_strcpy(unsigned char *dest, unsigned char *src) {
    int i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Function to concatenate two null-terminated strings
void my_strcat(unsigned char *dest, unsigned char *src) {
    int dest_len = my_strlen(dest);
    int i = 0;

    while (src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
}

// Function to compare two strings, similar to strcmp()
int my_strcmp(unsigned char *str1, unsigned char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

// Function to encode a UTF-8 string
int my_utf8_encode(unsigned char *input, unsigned char *output) {
    int input_len = my_strlen(input);
    int output_index = 0;

    for (int i = 0; i < input_len; i++) {
        if (input[i] < 0x80) {
            output[output_index++] = input[i];
        } else {
            int codepoint;
            sscanf((char *)&input[i], "\\u%X", &codepoint);

            if (codepoint <= 0x7FF) {
                output[output_index++] = 0xC0 | (codepoint >> 6);
                output[output_index++] = 0x80 | (codepoint & 0x3F);
            } else if (codepoint <= 0xFFFF) {
                output[output_index++] = 0xE0 | (codepoint >> 12);
                output[output_index++] = 0x80 | ((codepoint >> 6) & 0x3F);
                output[output_index++] = 0x80 | (codepoint & 0x3F);
            } else if (codepoint <= 0x10FFFF) {
                output[output_index++] = 0xF0 | (codepoint >> 18);
                output[output_index++] = 0x80 | ((codepoint >> 12) & 0x3F);
                output[output_index++] = 0x80 | ((codepoint >> 6) & 0x3F);
                output[output_index++] = 0x80 | (codepoint & 0x3F);
            }
            i += 5;
        }
    }
    output[output_index] = '\0';
    return output_index;
}

// Function to decode a UTF-8 string
int my_utf8_decode(unsigned char *input, unsigned char *output) {
    int input_len = my_strlen(input);
    int output_index = 0;

    for (int i = 0; i < input_len; i++) {
        if (input[i] < 0x80) {
            output[output_index++] = input[i];
        } else {
            int codepoint = 0;
            int bytes = 0;

            if ((input[i] & 0xE0) == 0xC0) {
                codepoint = input[i] & 0x1F;
                bytes = 2;
            } else if ((input[i] & 0xF0) == 0xE0) {
                codepoint = input[i] & 0x0F;
                bytes = 3;
            } else if ((input[i] & 0xF8) == 0xF0) {
                codepoint = input[i] & 0x07;
                bytes = 4;
            }

            for (int j = 1; j < bytes; j++) {
                codepoint = (codepoint << 6) | (input[i + j] & 0x3F);
            }

            output[output_index++] = codepoint;
            i += bytes - 1;
        }
    }
    output[output_index] = '\0';
    return output_index;
}

// Function to check if a string is a valid UTF-8 encoded string
int my_utf8_check(unsigned char *string) {
    int i = 0;

    while (string[i] != '\0') {
        if (string[i] < 0x80) {
            i++;
        } else {
            int codepoint = 0;
            int bytes = 0;

            if ((string[i] & 0xE0) == 0xC0) {
                codepoint = string[i] & 0x1F;
                bytes = 2;
            } else if ((string[i] & 0xF0) == 0xE0) {
                codepoint = string[i] & 0x0F;
                bytes = 3;
            } else if ((string[i] & 0xF8) == 0xF0) {
                codepoint = string[i] & 0x07;
                bytes = 4;
            } else {
                return 0;
            }

            for (int j = 1; j < bytes; j++) {
                if ((string[i + j] & 0xC0) != 0x80) {
                    return 0;
                }
                codepoint = (codepoint << 6) | (string[i + j] & 0x3F);
            }

            if (bytes == 2 && codepoint <= 0x7F) {
                return 0;
            } else if (bytes == 3 && codepoint <= 0x7FF) {
                return 0;
            } else if (bytes == 4 && (codepoint <= 0xFFFF || codepoint > 0x10FFFF)) {
                return 0;
            }
            i += bytes;
        }
    }
    return 1;
}

// Function to find the length of a UTF-8 string
int my_utf8_strlen(unsigned char *string) {
    int len = 0;
    int i = 0;

    while (string[i] != '\0') {
        if ((string[i] & 0xC0) != 0x80) {
            len++;
        }
        i++;
    }
    return len;
}

// Function to get the UTF8 encoded character at a specified index
unsigned char *my_utf8_charat(unsigned char *string, int index) {
    int len = my_utf8_strlen(string);

    if (index < 0 || index >= len) {
        return NULL;
    }

    int i = 0;
    int char_index = 0;

    while (string[i] != '\0') {
        if ((string[i] & 0xC0) != 0x80) {
            if (char_index == index) {
                unsigned char *result = malloc(5);

                if (result == NULL) {
                    return NULL;
                }
                int j = 0;

                while ((string[i] & 0xC0) == 0x80) {
                    i--;
                }

                while ((string[i] & 0xC0) != 0x80) {
                    result[j++] = string[i++];
                }

                result[j] = '\0';
                return result;
            }
            char_index++;
        }
        i++;
    }
    return NULL;
}

// Function to compare two UTF-8 strings
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2) {
    while (*string1 != '\0' && *string2 != '\0') {
        if (*string1 != *string2) {
            return *string1 - *string2;
        }
        string1++;
        string2++;
    }
    return *string1 - *string2;
}

// Additional Function 1: Count occurrences of a specific UTF-8 character in a string
int my_utf8_count_occurrences(unsigned char *string, unsigned char *character) {
    int count = 0;
    int i = 0;

    while (string[i] != '\0') {
        if (strncmp((char *)&string[i], (char *)character, strlen((char *)character)) == 0) {
            count++;
        }
        i += strlen((char *)&string[i]);
    }
    return count;
}

// Additional Function 2: Convert a UTF-8 string to uppercase
void my_utf8_to_uppercase(unsigned char *string) {
    int i = 0;

    while (string[i] != '\0') {
        if ((string[i] & 0xC0) != 0x80) {
            if ((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z')) {
                if (string[i] >= 'a' && string[i] <= 'z') {
                    string[i] = string[i] - 'a' + 'A';
                }
            }
        }
        i++;
    }
}

// Test function to demonstrate usage
void test_functions() {
    unsigned char input[] = "\\u05D0\\u05E8\\u05D9\\u05D4";
    unsigned char output[100];

    printf("Original Input: %s\n", input);

    // Test Encoding
    my_utf8_encode(input, output);
    printf("Encoded Output: %s\n", output);

    // Test Decoding
    my_utf8_decode(output, input);
    printf("Decoded Output: %s\n", input);

    // Test Length
    printf("Length of Decoded String: %d\n", my_utf8_strlen(input));

    // Test Charat
    unsigned char *char_at = my_utf8_charat(input, 1);
    if (char_at != NULL) {
        printf("Character at Index 1: %s\n", char_at);
        free(char_at); // Free allocated memory
    } else {
        printf("Invalid Index\n");
    }

    // Test Comparison
    unsigned char str1[] = "\\u05D0\\u05E8\\u05D9\\u05D4";
    unsigned char str2[] = "\\u05D0\\u05E8\\u05D9\\u05D5";
    int result = my_utf8_strcmp(str1, str2);
    printf("Comparison Result: %d\n", result);

    // Additional Function 1: Count Occurrences
    unsigned char str3[] = "\\u05D0\\u05E8\\u05D9\\u05D4\\u05D0\\u05E8\\u05D9\\u05D4";
    unsigned char character[] = "\\u05D0\\u05E8\\u05D9\\u05D4";
    int occurrences = my_utf8_count_occurrences(str3, character);
    printf("Occurrences of %s: %d\n", character, occurrences);

    // Additional Function 2: Convert to Uppercase
    unsigned char str4[] = "Hello \\u05D0\\u05E8\\u05D9\\u05D4";
    printf("Original String: %s\n", str4);
    my_utf8_to_uppercase(str4);
    printf("Uppercase String: %s\n", str4);

    // Test UTF-8 Check
    unsigned char str5[] = "\\u05D0\\u05E8\\u05D9\\u05D4";
    int check_result = my_utf8_check(str5);
    printf("UTF-8 Check Result: %s\n", check_result ? "Valid" : "Invalid");
}

int main() {
    test_functions();
    return 0;
}
