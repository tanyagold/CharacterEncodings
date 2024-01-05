# CharacterEncodings
# UTF-8 String Manipulation in C

This repository contains C code for manipulating UTF-8 encoded strings. The code provides functions for encoding, decoding, comparing, checking validity, and performing various operations on UTF-8 strings. Additionally, there are two custom functions for counting occurrences of a specific UTF-8 character and converting a UTF-8 string to uppercase.

## Table of Contents

- [Introduction](#introduction)
- [Functions](#functions)
- [Usage](#usage)
- [Testing](#testing)
- [Additional Functions](#additional-functions)

## Introduction

UTF-8 is a widely used character encoding that represents characters in a variable number of bytes. This C code provides functions to work with UTF-8 strings, including encoding and decoding, length calculation, character extraction, comparison, and more.

## Functions

### `my_strlen`

Find the length of a null-terminated string.

### `my_strcpy`

Copy a null-terminated string from source to destination.

### `my_strcat`

Concatenate two null-terminated strings.

### `my_strcmp`

Compare two strings similar to `strcmp()`.

### `my_utf8_encode`

Encode a UTF-8 string.

### `my_utf8_decode`

Decode a UTF-8 string.

### `my_utf8_check`

Check if a string is a valid UTF-8 encoded string.

### `my_utf8_strlen`

Find the length of a UTF-8 string.

### `my_utf8_charat`

Get the UTF-8 encoded character at a specified index.

### `my_utf8_strcmp`

Compare two UTF-8 strings.

### `my_utf8_count_occurrences`

Count occurrences of a specific UTF-8 character in a string.

### `my_utf8_to_uppercase`

Convert a UTF-8 string to uppercase.

## Usage

Clone the repository and include the C source file in your project. Utilize the provided functions as needed for UTF-8 string manipulation.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the UTF-8 string manipulation code file

int main() {
    test_functions(); // Test the provided functions
    return 0;
}
```

## Testing

The `test_functions` function in the main file demonstrates the usage of the provided functions. Run the program to see the results of encoding, decoding, length calculation, character extraction, comparison, and additional operations.

## Additional Functions

### `my_utf8_count_occurrences`

Count occurrences of a specific UTF-8 character in a string.

### `my_utf8_to_uppercase`

Convert a UTF-8 string to uppercase.
