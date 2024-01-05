#include <stdio.h>
#include <stdlib.h>


// Function to find the length of a null-terminated string
// Declare a function named my_strlen that takes a pointer to an unsigned char (string) as its parameter
int my_strlen(unsigned char *str) {
    // Initialize a variable len to store the length of the string and set it to 0
    int len = 0;

    // Use a while loop to iterate through the string until a null character ('\0') is encountered
    while (str[len] != '\0') {
        // Increment the length variable for each character in the string
        len++;
    }

    // Return the final length of the string
    return len;
}


// Function to copy a null-terminated string from source to destination
// Declare a function named my_strcpy that takes two pointers to unsigned char (source and destination strings) as its parameters
void my_strcpy(unsigned char *dest, unsigned char *src) {
    // Initialize a variable i to use as an index for iterating through the source string
    int i = 0;

    // Use a while loop to iterate through the source string until a null character ('\0') is encountered
    while (src[i] != '\0') {
        // Copy each character from the source string to the corresponding position in the destination string
        dest[i] = src[i];

        // Increment the index variable to move to the next character in both source and destination strings
        i++;
    }

    // Add a null character at the end of the destination string to terminate it
    dest[i] = '\0';
}


// Function to concatenate two null-terminated strings
// Declare a function named my_strcat that takes two pointers to unsigned char (destination and source strings) as its parameters
void my_strcat(unsigned char *dest, unsigned char *src) {
    // Find the length of the destination string using the previously defined my_strlen function
    int dest_len = my_strlen(dest);

    // Initialize a variable i to use as an index for iterating through the source string
    int i = 0;

    // Use a while loop to iterate through the source string until a null character ('\0') is encountered
    while (src[i] != '\0') {
        // Concatenate each character from the source string to the end of the destination string
        dest[dest_len + i] = src[i];

        // Increment the index variable to move to the next character in both source and destination strings
        i++;
    }

    // Add a null character at the end of the concatenated string to terminate it
    dest[dest_len + i] = '\0';
}


// Function to compare two strings, similar to strcmp()
// Declare a function named my_strcmp that takes two pointers to unsigned char (strings to be compared) as its parameters
int my_strcmp(unsigned char *str1, unsigned char *str2) {
    // Use a while loop to iterate through both strings until either of them reaches the null character ('\0')
    while (*str1 != '\0' && *str2 != '\0') {
        // Compare the current characters of both strings
        if (*str1 != *str2) {
            // If characters are not equal, return the difference between their ASCII values
            return *str1 - *str2;
        }

        // Move to the next character in both strings
        str1++;
        str2++;
    }

    // If the loop completes, compare the final characters of both strings and return the difference
    return *str1 - *str2;
}

// Function to encode a UTF-8 string
// Declare a function named my_utf8_encode that takes two pointers to unsigned char (input and output strings) as its parameters
int my_utf8_encode(unsigned char *input, unsigned char *output) {
    // Find the length of the input string using the previously defined my_strlen function
    int input_len = my_strlen(input);

    // Initialize an index variable for the output string
    int output_index = 0;

    // Use a for loop to iterate through each character in the input string
    for (int i = 0; i < input_len; i++) {
        // Check if the character is within the ASCII range (less than 0x80)
        if (input[i] < 0x80) {
            // If it is, directly copy the character to the output string
            output[output_index++] = input[i];
        } else {
            // If the character is outside the ASCII range, indicating a UTF-8 encoded character
            int codepoint;

            // Use sscanf to convert the hexadecimal Unicode code point from the input string
            //sscanf((char *)&input[i], "\\u%X", &codepoint);
            sscanf((char *)&input[i], "%X", &codepoint);

            // Based on the Unicode code point, encode the character in UTF-8
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

            // Skip the next 5 characters in the input string as they were part of the Unicode escape sequence
            i += 5;
        }
    }

    // Add a null character at the end of the encoded string to terminate it
    output[output_index] = '\0';

    // Return the length of the encoded string
    return output_index;
}


// Function to decode a UTF-8 string
// Declare a function named my_utf8_decode that takes two pointers to unsigned char (input and output strings) as its parameters
int my_utf8_decode(unsigned char *input, unsigned char *output) {
    // Find the length of the input string using the previously defined my_strlen function
    int input_len = my_strlen(input);

    // Initialize an index variable for the output string
    int output_index = 0;

    // Use a for loop to iterate through each character in the input string
    for (int i = 0; i < input_len; i++) {
        // Check if the character is within the ASCII range (less than 0x80)
        if (input[i] < 0x80) {
            // If it is, directly copy the character to the output string
            output[output_index++] = input[i];
        } else {
            // If the character is outside the ASCII range, indicating a UTF-8 encoded character
            int codepoint = 0;
            int bytes = 0;

            // Determine the number of bytes used to encode the character based on the leading byte
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

            // Extract the remaining bits of the code point from the subsequent bytes
            for (int j = 1; j < bytes; j++) {
                codepoint = (codepoint << 6) | (input[i + j] & 0x3F);
            }

            // Store the decoded code point in the output string
            if (codepoint <= 0x7F) {
                output[output_index++] = codepoint;
            } else if (codepoint <= 0x7FF) {
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

            // Skip the next (bytes - 1) characters in the input string as they were part of the UTF-8 encoding
            i += bytes - 1;
        }
    }

    // Add a null character at the end of the decoded string to terminate it
    output[output_index] = '\0';

    // Return the length of the decoded string
    return output_index;
}


// Function to check if a string is a valid UTF-8 encoded string
// Declare a function named my_utf8_check that takes a pointer to an unsigned char (string) as its parameter
int my_utf8_check(unsigned char *string) {
    // Initialize an index variable for iterating through the string
    int i = 0;

    // Use a while loop to iterate through the string until a null character ('\0') is encountered
    while (string[i] != '\0') {
        // Check if the current character is within the ASCII range (less than 0x80)
        if (string[i] < 0x80) {
            // If it is, move to the next character
            i++;
        } else {
            // If the character is outside the ASCII range, indicating a UTF-8 encoded character
            int codepoint = 0;
            int bytes = 0;

            // Determine the number of bytes used to encode the character based on the leading byte
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
                // If the leading byte is not valid for UTF-8 encoding, return 0 (not a valid UTF-8 string)
                return 0;
            }

            // Check the validity of the remaining bytes in the encoding
            for (int j = 1; j < bytes; j++) {
                if ((string[i + j] & 0xC0) != 0x80) {
                    // If a subsequent byte does not match the expected format, return 0
                    return 0;
                }
                codepoint = (codepoint << 6) | (string[i + j] & 0x3F);
            }

            // Check the validity of the code point based on the number of bytes used for encoding
            if (bytes == 2 && codepoint <= 0x7F) {
                return 0;
            } else if (bytes == 3 && codepoint <= 0x7FF) {
                return 0;
            } else if (bytes == 4 && (codepoint <= 0xFFFF || codepoint > 0x10FFFF)) {
                return 0;
            }

            // Move the index to the next character after the UTF-8 encoding
            i += bytes;
        }
    }

    // If the loop completes, the string is a valid UTF-8 string, so return 1
    return 1;
}


// Function to find the length of a UTF-8 string
// Declare a function named my_utf8_strlen that takes a pointer to an unsigned char (string) as its parameter
int my_utf8_strlen(unsigned char *string) {
    // Initialize a variable len to store the length of the string and set it to 0
    int len = 0;

    // Initialize an index variable for iterating through the string
    int i = 0;

    // Use a while loop to iterate through the string until a null character ('\0') is encountered
    while (string[i] != '\0') {
        // Check if the current character is the start of a UTF-8 character (does not match the pattern 0x80 or 0xC0)
        if ((string[i] & 0xC0) != 0x80) {
            // If it is, increment the length variable to count the start of a new character
            len++;
        }

        // Move to the next character in the string
        i++;
    }

    // Return the final length of the UTF-8 string
    return len;
}


// Function to get the UTF8 encoded character at a specified index
// Declare a function named my_utf8_charat that takes a pointer to an unsigned char (string) and an integer index as its parameters
unsigned char *my_utf8_charat(unsigned char *string, int index) {
    // Find the length of the UTF-8 string using the previously defined my_utf8_strlen function
    int len = my_utf8_strlen(string);

    // Check if the specified index is out of bounds
    if (index < 0 || index >= len) {
        // If it is, return NULL as the index is invalid
        return NULL;
    }

    // Initialize index variables for iterating through the string and counting UTF-8 characters
    int i = 0;
    int char_index = 0;

    // Use a while loop to iterate through the string until a null character ('\0') is encountered
    while (string[i] != '\0') {
        // Check if the current character is the start of a UTF-8 character (does not match the pattern 0x80 or 0xC0)
        if ((string[i] & 0xC0) != 0x80) {
            // Check if the current UTF-8 character is the one at the specified index
            if (char_index == index) {
                // Calculate the number of bytes in the UTF-8 character
                int bytes = 1;
                while ((string[i + bytes] & 0xC0) == 0x80) {
                    bytes++;
                }

                // Allocate memory for the result (maximum 4 bytes for a UTF-8 character and 1 for null terminator)
                unsigned char *result = malloc(bytes + 1);

                // Check if memory allocation is successful
                if (result == NULL) {
                    // If not, return NULL
                    return NULL;
                }

                // Initialize an index variable for building the result string
                int j = 0;

                // Copy the UTF-8 character to the result string
                while ((string[i] & 0xC0) != 0x80 && string[i] != '\0') {
                    result[j++] = string[i++];
                }

                // Add a null character at the end of the result string to terminate it
                result[j] = '\0';

                // Return the result string containing the UTF-8 character at the specified index
                return result;
            }

            // Increment the UTF-8 character index
            char_index++;
        }

        // Move to the next character in the string
        i++;
    }

    // Return NULL if the specified index is not found (should not reach this point)
    return NULL;
}


// Function to compare two UTF-8 strings
// Declare a function named my_utf8_strcmp that takes two pointers to unsigned char (UTF-8 strings to be compared) as its parameters
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2) {
    // Use a while loop to iterate through both UTF-8 strings until either of them reaches the null character ('\0')
    while (*string1 != '\0' && *string2 != '\0') {
        // Compare the current characters of both UTF-8 strings
        if (*string1 != *string2) {
            // If characters are not equal, return the difference between their ASCII values
            return *string1 - *string2;
        }

        // Move to the next character in both UTF-8 strings
        string1++;
        string2++;
    }

    // If the loop completes, compare the final characters of both UTF-8 strings and return the difference
    return *string1 - *string2;
}


// Additional Function 1: Count occurrences of a specific UTF-8 character in a string
// Declare a function named my_utf8_count_occurrences that takes two pointers to unsigned char (string and character) as its parameters
int my_utf8_count_occurrences(unsigned char *string, unsigned char *character) {
    // Initialize a variable count to store the number of occurrences and set it to 0
    int count = 0;

    // Use a while loop to iterate through the string until a null character ('\0') is encountered
    while (*string != '\0') {
        // Initialize variables for iterating through the character to match and checking for a match
        int i = 0;
        int match = 1;

        // Use a nested while loop to compare each character in the character string with the corresponding character in the main string
        while (character[i] != '\0') {
            // Check if the characters match
            if (string[i] != character[i]) {
                // If not, set match to 0 and break out of the loop
                match = 0;
                break;
            }
            i++;
        }

        // Check if a complete match was found
        if (match) {
            // If so, increment the count of occurrences
            count++;
        }

        // Move to the next character in the main string
        string++;
    }

    // Return the final count of occurrences
    return count;
}


// Additional Function 2: Convert a UTF-8 string to uppercase
// Declare a function named my_utf8_to_uppercase that takes a pointer to an unsigned char (string) as its parameter
void my_utf8_to_uppercase(unsigned char *string) {
    // Initialize an index variable for iterating through the string
    int i = 0;

    // Use a while loop to iterate through the string until a null character ('\0') is encountered
    while (string[i] != '\0') {
        // Check if the current character is the start of a UTF-8 character (does not match the pattern 0x80 or 0xC0)
        if ((string[i] & 0xC0) != 0x80) {
            // Check if the current character is a lowercase ASCII letter
            if ((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z')) {
                // If it is, convert it to uppercase
                if (string[i] >= 'a' && string[i] <= 'z') {
                    string[i] = string[i] - 'a' + 'A';
                }
            }
        }

        // Move to the next character in the string
        i++;
    }
}


/////////////////////////////////////
////////// TEST FUNCTIONS ///////////
/////////////////////////////////////


// Test my_strcmp function
void test_my_strcmp() {
    printf("Testing my_strcmp:\n");

    // Test 1: Equal strings
    unsigned char str1[] = "שלום";
    unsigned char str2[] = "שלום";
    printf("Test 1: %d\n", my_strcmp(str1, str2)); 
    // Expected output: 0

    // Test 2: Different strings
    unsigned char str3[] = "שלום";
    unsigned char str4[] = "הי";
    printf("Test 2: %d\n", my_strcmp(str3, str4)); 
    // Expected output: Non-zero

    // Test 3: Mix of ASCII and UTF-8 characters
    unsigned char str5[] = "שלום, hello";
    unsigned char str6[] = "שלום, שלום";
    printf("Test 3: %d\n", my_strcmp(str5, str6)); 
    // Expected output: Non-zero

    // Test 4: Empty strings
    unsigned char str7[] = "";
    unsigned char str8[] = "";
    printf("Test 4: %d\n", my_strcmp(str7, str8)); 
    // Expected output: 0

    printf("\n");
}


// Test my_utf8_encode function
void test_my_utf8_encode() {
    printf("Testing my_utf8_encode:\n");

    // Test 1: Basic ASCII characters
    unsigned char input1[] = "שלום";
    unsigned char output1[100]; 
    int len1 = my_utf8_encode(input1, output1);
    printf("Test 1: %s, Length: %d\n", output1, len1);
    // Expected output: "\xd7\xa9\xd7\x9c\xd7\x95\xd7\x9d", Length: 12

    // Test 2: UTF-8 characters
    unsigned char input2[] = "Hello, עולם";
    unsigned char output2[100]; 
    int len2 = my_utf8_encode(input2, output2);
    printf("Test 2: %s, Length: %d\n", output2, len2);
    // Expected output: "Hello, \xd7\xa2\xd7\x95\xd7\x9c\xd7\x9d", Length: 18

    // Test 3: Empty string
    unsigned char input3[] = "";
    unsigned char output3[100]; 
    int len3 = my_utf8_encode(input3, output3);
    printf("Test 3: %s, Length: %d\n", output3, len3);
    // Expected output: "", Length: 0

    printf("\n");
}


// Test my_utf8_decode function
void test_my_utf8_decode() {
    printf("Testing my_utf8_decode:\n");

    // Test 1: Basic ASCII characters
    unsigned char input1[] = "שלום";
    unsigned char output1[100]; 
    int len1 = my_utf8_decode(input1, output1);
    printf("Test 1: %s, Length: %d\n", output1, len1);
    // Expected output: "שלום", Length: 4

    // Test 2: UTF-8 characters
    unsigned char input2[] = "Hello, עולם";
    unsigned char output2[100]; 
    int len2 = my_utf8_decode(input2, output2);
    printf("Test 2: %s, Length: %d\n", output2, len2);
    // Expected output: "Hello, עולם", Length: 12

    // Test 3: Empty string
    unsigned char input3[] = "";
    unsigned char output3[100]; 
    int len3 = my_utf8_decode(input3, output3);
    printf("Test 3: %s, Length: %d\n", output3, len3);
    // Expected output: "", Length: 0

    printf("\n");
}


// Test my_utf8_check function
void test_my_utf8_check() {
    printf("Testing my_utf8_check:\n");

    // Test 1: Valid UTF-8 string
    unsigned char str1[] = "שלום, hello";
    printf("Test 1: %d\n", my_utf8_check(str1)); 
    // Expected output: 1

    // Test 2: Invalid UTF-8 string
    unsigned char str2[] = "שלום\x80";
    printf("Test 2: %d\n", my_utf8_check(str2)); 
    // Expected output: 0

    // Test 3: Valid UTF-8 string with ASCII characters
    unsigned char str3[] = "Hello, עולם";
    printf("Test 3: %d\n", my_utf8_check(str3)); 
    // Expected output: 1

    // Test 4: Empty string
    unsigned char str4[] = "";
    printf("Test 4: %d\n", my_utf8_check(str4)); 
    // Expected output: 1

    printf("\n");
}


// Test my_utf8_strlen function
void test_my_utf8_strlen() {
    printf("Testing my_utf8_strlen:\n");

    // Test 1: Basic ASCII characters
    unsigned char str1[] = "שלום";
    printf("Test 1: %d\n", my_utf8_strlen(str1)); 
    // Expected output: 4

    // Test 2: UTF-8 characters
    unsigned char str2[] = "Hello, עולם";
    printf("Test 2: %d\n", my_utf8_strlen(str2)); 
    // Expected output: 12

    // Test 3: Empty string
    unsigned char str3[] = "";
    printf("Test 3: %d\n", my_utf8_strlen(str3)); 
    // Expected output: 0

    printf("\n");
}


// Test my_utf8_charat function
void test_my_utf8_charat() {
    printf("Testing my_utf8_charat:\n");

    // Test 1: Basic ASCII characters
    unsigned char str1[] = "שלום";
    int index1 = 2;
    unsigned char *result1 = my_utf8_charat(str1, index1);
    printf("Test 1: %s\n", result1);
    // Expected output: "ל", assuming UTF-8 encoding, length: 2
    free(result1);

    // Test 2: UTF-8 characters
    unsigned char str2[] = "Hello, עולם";
    int index2 = 7;
    unsigned char *result2 = my_utf8_charat(str2, index2);
    printf("Test 2: %s\n", result2);
    // Expected output: "ע", assuming UTF-8 encoding, length: 2
    free(result2);

    // Test 3: Invalid index
    unsigned char str3[] = "שלום";
    int index3 = 5;
    unsigned char *result3 = my_utf8_charat(str3, index3);
    printf("Test 3: %p\n", (void *)result3);
    // Expected output: NULL
    free(result3);

    // Test 4: Empty string
    unsigned char str4[] = "";
    int index4 = 0;
    unsigned char *result4 = my_utf8_charat(str4, index4);
    printf("Test 4: %p\n", (void *)result4);
    // Expected output: NULL
    free(result4);

    printf("\n");
}


// Test my_utf8_strcmp function
void test_my_utf8_strcmp() {
    printf("Testing my_utf8_strcmp:\n");

    // Test 1: Equal strings
    unsigned char str1[] = "שלום";
    unsigned char str2[] = "שלום";
    printf("Test 1: %d\n", my_utf8_strcmp(str1, str2)); 
    // Expected output: 0

    // Test 2: Different strings
    unsigned char str3[] = "שלום";
    unsigned char str4[] = "הי";
    printf("Test 2: %d\n", my_utf8_strcmp(str3, str4)); 
    // Expected output: Non-zero

    // Test 3: Mix of ASCII and UTF-8 characters
    unsigned char str5[] = "שלום, hello";
    unsigned char str6[] = "שלום, שלום";
    printf("Test 3: %d\n", my_utf8_strcmp(str5, str6)); 
    // Expected output: Non-zero

    // Test 4: Empty strings
    unsigned char str7[] = "";
    unsigned char str8[] = "";
    printf("Test 4: %d\n", my_utf8_strcmp(str7, str8)); 
    // Expected output: 0

    printf("\n");
}


// Test my_utf8_count_occurrences function
void test_my_utf8_count_occurrences() {
    printf("Testing my_utf8_count_occurrences:\n");

    // Test 1: Count occurrences of a specific UTF-8 character
    unsigned char str1[] = "שלום, שלום";
    unsigned char character1[] = "ש";
    printf("Test 1: %d\n", my_utf8_count_occurrences(str1, character1)); 
    // Expected output: 2

    // Test 2: Count occurrences of a specific UTF-8 character (none)
    unsigned char str2[] = "Hello, עולם";
    unsigned char character2[] = "ש";
    printf("Test 2: %d\n", my_utf8_count_occurrences(str2, character2)); 
    // Expected output: 0

    // Test 3: Count occurrences of a specific UTF-8 sequence
    unsigned char str3[] = "שלום, שלום, שלום";
    unsigned char character3[] = "שלום";
    printf("Test 3: %d\n", my_utf8_count_occurrences(str3, character3)); 
    // Expected output: 3

    // Test 4: Count occurrences of a specific UTF-8 sequence (none)
    unsigned char str4[] = "Hello, עולם";
    unsigned char character4[] = "שלום";
    printf("Test 4: %d\n", my_utf8_count_occurrences(str4, character4)); 
    // Expected output: 0

    // Test 5: Empty string
    unsigned char str5[] = "";
    unsigned char character5[] = "שלום";
    printf("Test 5: %d\n", my_utf8_count_occurrences(str5, character5)); 
    // Expected output: 0

    printf("\n");
}


// Test my_utf8_to_uppercase function
void test_my_utf8_to_uppercase() {
    printf("Testing my_utf8_to_uppercase:\n");

    // Test 1: Convert a mixed-case UTF-8 string to uppercase
    unsigned char str1[] = "שלום, hello";
    my_utf8_to_uppercase(str1);
    printf("Test 1: %s\n", str1);
    // Expected output: "שלום, HELLO"

    // Test 2: Convert an already uppercase UTF-8 string to uppercase (no change)
    unsigned char str2[] = "שלום, HELLO";
    my_utf8_to_uppercase(str2);
    printf("Test 2: %s\n", str2);
    // Expected output: "שלום, HELLO"

    // Test 3: Convert an empty UTF-8 string to uppercase (no change)
    unsigned char str3[] = "";
    my_utf8_to_uppercase(str3);
    printf("Test 3: %s\n", str3);
    // Expected output: ""

    printf("\n");
}


// Test function to demonstrate usage
void test_functions() {
    test_my_strcmp();
    test_my_utf8_encode();
    test_my_utf8_decode();
    test_my_utf8_check();
    test_my_utf8_strlen();
    test_my_utf8_charat();
    test_my_utf8_strcmp();
    test_my_utf8_count_occurrences();
    test_my_utf8_to_uppercase();
}


int main() {
    test_functions();
    return 0;
}
