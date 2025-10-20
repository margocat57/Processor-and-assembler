//! @file
#ifndef FILE_WORK_H
#define FILE_WORK_H
#include <stdlib.h>
#include "parsing_str.h"
#include "assembler_struct.h"

bool incorr_work_with_stat(const char *name_of_file, struct stat *all_info_about_file);

size_t count_strings_by_symbols(char *array_to_search, char ch);

//! @brief Reads a file and puts it's content into array
//!
//! The function reads all file's content into a buffer,
//!     created by using ^calloc function,
//!     replaces newline characters with null terminators,
//!     and calls function to count amount of strings
//!
//! ^Creates a buffer with 1 extra byte compared to the file size for safety
//!
//! @return file_in_array structure containing:
//!     - all_strings_in_file: pointer to buffer with file content (strings separated by '\0')
//!     - amount_str: number of lines in the file
//!
//! @note Allocated memory must be freed by the caller
//! @warning Function uses assert for pointer name_of_file
//! @warning Function checks file status using fstat to get file size in bytes.
//!          If fstat fails, error message is printed to stderr and program exit wit
file_in_array read_file_to_string_array(const char *name_of_file);

//! @brief Creates an array of pointer_and_string_size structures from file content
//!
//! This function processes a null-terminated array of strings and creates a parallel array
//!     each element of which containing pointers to each string and their sizes.
//! It uses strlen to find the len of each string and
//!     moves pointer to idx strlen+1 to find index of new string(except last string for not to be out of index)
//!
//! @return Pointer to allocated array of ptr_array_and_size_of_strings structures
//!
//! @warning UNDEFINED BEHAVIOR if the source text contains empty strings in the middle.
//!
//! @note Memory allocated by this function must be freed by the caller
char** create_ptr_array(file_in_array* file_in_arr);

//! @brief Writes assembler bytecode buffer to a binary file
//!
//! This function saves bytecode to a file in the following format:
//! 1. Author signature string 
//! 2. Bytecode array size 
//! 3. Actual bytecode array data 
//!
//! @note File is opened in binary write mode ("wb")
//! @note File is properly closed even if errors occur during writing
//! @note Performs NULL pointer checks on input parameters
//! @note Еrrors are handled with descriptive messages to stderr
//!
//! @warning On write failure, partial data may already be written to file
void put_buffer_to_file(const char *name_of_file, assembler* assembl);

#endif //FILE_WORK_H