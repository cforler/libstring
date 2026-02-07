#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define LIBSTRING_VERSION "0.1.3"

#define COLOR_DEFAULT "\033[0m"
#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

enum stringcolor { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

typedef struct {
  size_t len;
  char buf[];
} string_t;

/**
 * Creates a new colored string_t initialized with the provided character array.
 *
 * @param str A null-terminated character array to initialize the string with.
 * @param c The color to apply to the string (specified by the enum
 * stringcolor).
 * @return A pointer to the newly allocated and colored string containing a copy
 *         of the input character array, or NULL if memory allocation failed.
 *         The returned string must be deallocated using the standard C
 *         library function `free()` when no longer needed.
 */
string_t *string_colored(const char *str, enum stringcolor c);

/**
 * Allocates a new string_t initialized with the provided character array.
 *
 * @param str A null-terminated character array to initialize the string with.
 * @return A pointer to the newly allocated string_t object containing a copy
 *         of the input character array, or NULL if memory allocation failed.
 *         The returned string must be deallocated using the standard C library
 *         function `free()` when no longer needed.
 **/
string_t *string_new(const char *str);

/**
 * Creates a new string_t object as a copy of the provided string.
 *
 * @param str The source string_t object to be cloned.
 * @return A pointer to the newly allocated string_t containing a copy of the
 *         source string, or NULL if memory allocation failed.
 *         The returned string must be deallocated using the standard C
 *         library function `free()` when no longer needed.
 *
 **/
string_t *string_clone(const string_t *str);

/**
 * Reads up to 64 KB of data from the specified file descriptor and
 * creates a new string containing the read data.
 *
 * @param fd The file descriptor to read from.
 * @return A pointer to the newly allocated string_t containing the read data
 *         or an empty string if an error occurred or the end of the file has
 *         been reached. The returned string must be deallocated using the
 *         standard C library function `free()` when no longer needed.
 **/
string_t *string_readfd(int fd);

/**
 * Reads a line from a file stream, with a maximum of 64 KB of data,
 * and stores it as a newly allocated string.
 *
 * @param stream The file stream to read from.
 * @return A pointer to the newly allocated string containing the read line or
           an empty string if an error occurred or the end of the file has
           been reached. The returned string must be deallocated using the
           standard C library function `free()`  when no longer needed.
 **/
string_t *string_readline(FILE *stream);

/**
 * Concatenates two strings and returns a new string containing the combined
 * contents.
 *
 * @param s1 The first string to concatenate.
 * @param s2 The second string to concatenate.
 * @return A pointer to a newly allocated string containing the concatenated
 *         contents of the input strings, or NULL if memory allocation failed.
 *         The returned string must be deallocated using the standard C
 *         library function `free()` when no longer needed.
 **/
string_t *string_concat(const string_t *s1, const string_t *s2);

/**
 * Creates a new string by removing leading and trailing whitespace characters
 * from the provided string.
 *
 * @param str The input string to trim.
 * @return A pointer to a newly allocated string containing the trimmed
 *         contents of the input string, or NULL if memory allocation failed.
 *         The returned string must be deallocated using `free()`  when no
 *         longer needed.
 **/
string_t *string_trim(const string_t *str);

/**
 * Compares two strings lexicographically.
 *
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @return An integer greater than, equal to, or less than 0 if s1 is greater
 *         than, equal to, or less than s2, respectively.
 **/
int string_compare(const string_t *s1, const string_t *s2);

/**
 * Checks if two string_t objects have equal contents.
 *
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @return True if the contents of s1 are equal to the contents of s2, false
 *         otherwise.
 **/
bool string_equal(const string_t *s1, const string_t *s2);

/**
 * Creates a new string that represents a substring of the input string.
 *
 * @param str The input string.
 * @param start The starting index of the substring.
 * @param end The ending index of the substring
 * @return A pointer to a newly allocated string containing the specified
 *         substring, or NULL if memory allocation failed or NULL if the
 *         indices are out of bounds. The returned string must be deallocated
 *         using the standard C library function `free()` when no longer needed.
 **/
string_t *string_substring(const string_t *str, size_t start, size_t end);

/**
 * Converts a string to a null-terminated C string.
 *
 * @param str The input string.
 * @return A pointer to a null-terminated C string containing the contents of
 *         the input string, or NULL if memory allocation failed. The returned
 *         C string must be deallocated using the standard C library function
 *         `free()` when no longer needed.
 **/
char *string_tocstr(const string_t *str);

/**
 * Finds the first occurrence of a substring within a string.
 *
 * @param str The input string to search in.
 * @param substring The substring to search for.
 * @return The index of the first occurrence of the substring in the input
 *         string, or -1 if not found.
 **/
int string_substring_index(const string_t *str, const string_t *substring);

/**
 * Checks if a substring appears in the input string at a specified offset.
 *
 * @param str The input string to search in.
 * @param sub The substring to search for.
 * @param off The offset within the input string to start the search.
 * @return True if the substring appears in the input string at the specified
 *         offset, false otherwise.
 */
bool string_is_substring(const string_t *str, const string_t *sub, size_t off);

/**
 * Creates a new string by repeating the input string multiple times.
 *
 * This function takes an input string `str` and a `times` parameter. It
 * creates a new string by repeating the input `str` `times` times. The
 * returned C string must be deallocated using the standard C library function
 * `free()`.
 *
 * @param str The input string to be repeated.
 * @param times The number of times to repeat the input string, or NULL if
 *        memory allocation failed.
 * @return A dynamically allocated string containing the repeated input string.
 **/
string_t *string_repeat(const string_t *str, size_t times);

/**
 * Creates a new string where all occurrences of the 'old' character
 * in the input string are replaced with the 'new' character.
 *
 * @param str The input string.
 * @param old The character to be replaced.
 * @param new The character to replace occurrences of 'old'.
 * @return A new string with replaced characters. Must be freed after use.
 * @note The returned string is allocated on the heap and must be freed by the
 * caller.
 * @note If 'old' is not found in the input string, the function returns a copy
 * of the input string.
 **/
string_t *string_replace_char(const string_t *str, char old, char new);

/* Creates a new string where all occurrences of the 'old' substring
 * in the input string are replaced with the 'new' substring.
 *
 * @param str The input string.
 * @param old The substring to be replaced.
 * @param new The substring to replace occurrences of 'old'.
 * @return A new string with replaced substrings. Must be freed after use.
 * @note The returned string is allocated on the heap and must be freed by the
 * caller.
 * @note If 'old' is not found in the input string, the function returns a copy
 * of the input string.
 **/
string_t *string_replace(const string_t *str, const string_t *old,
                         const string_t *new);

/**
 * Returns the character at the specified index in the string.
 *
 * @param s The string to access.
 * @param index The index of the character to retrieve.
 * @return The character at the specified index, or -1 if the index is out of
 *         bounds.
 **/
static inline char string_get(const string_t *s, size_t index) {
  return (index < s->len) ? s->buf[index] : -1;
}

/**
 * Reads a line from the standard input and stores it as a newly allocated
 * string.
 *
 * @return A pointer to a newly allocated string_t object containing the read
 *         line, or NULL if memory allocation failed. The returned C string
 * must be deallocated using the standard C library function `free()` when no
 * longer needed.
 **/
static inline string_t *string_read() { return string_readfd(STDIN_FILENO); }

/**
 * Reads a line from the specified file stream and stores it as a newly
 * allocated string.
 *
 * @param stream The file stream to read from.
 * @return A pointer to a newly allocated string containing the read line, or
 *         NULL if memory allocation failed. The returned C string must be
 *         deallocated using the standard C library function `free()` when no
 *         longer needed.
 **/
static inline string_t *string_readf(FILE *stream) {
  return string_readfd(fileno(stream));
}

/**
 * Checks if the given string is empty (length is 0).
 *
 * @param s The string_t object to check.
 * @return true if the string is empty, false otherwise.
 */
static inline bool string_empty(const string_t *s) { return s->len == 0; }

/**
 * Returns the length (number of characters) of the string.
 *
 * @param s The string to get the length of.
 * @return The length of the string.
 */
static inline size_t string_len(const string_t *s) { return s->len; }

/**
 * Prints the contents of the string to the standard output.
 *
 * @param s The string to print.
 * @return The number of characters printed, or a negative value if an error
 *         occurred.
 **/
static inline int string_print(const string_t *s) {
  return printf("%.*s", (int)s->len, s->buf);
}

/**
 * Prints the contents of the string to the specified file stream.
 *
 * @param s The string to print.
 * @param f The file stream to print to.
 * @return The number of characters printed, or a negative value if an error
 *         occurred.
 **/
static inline int string_printf(const string_t *s, FILE *f) {
  return fprintf(f, "%.*s", (int)s->len, s->buf);
}

/**
 * Prints the contents of the string followed by a newline character to the
 * standard output.
 *
 * @param s The string to print.
 * @return The number of characters printed, or a negative value if an error
 *         occurred.
 */
static inline int string_println(const string_t *s) {
  return printf("%.*s\n", (int)s->len, s->buf);
}

/**
 * Prints the contents of the string followed by a newline character to the
 * specified file stream.
 *
 * @param s The string to print.
 * @param f The file stream to print to.
 * @return The number of characters printed, or a negative value if an error
 *         occurred.
 */
static inline int string_printlnf(const string_t *s, FILE *f) {
  return fprintf(f, "%.*s\n", (int)s->len, s->buf);
}

/**********************************************************************/

typedef char (*charfunc_t)(char);
typedef bool (*boolfunc_t)(char);

/**
 * Applies a specified character transformation function to each character of
 * the input string.
 *
 * @param func The character transformation function to apply.
 * @param str The input string.
 * @return A pointer to a newly allocated string containing the result of
 *         applying the function func
 *         to each character of the input string, or NULL if memory allocation
 *         failed. The returned string must be deallocated using the standard C
 *         library function `free()` when no longer needed.
 **/
string_t *string_map(charfunc_t func, const string_t *str);

/**
 * Filters the characters of the input string using a specified predicate
 * function.
 *
 * @param func The predicate function that determines which characters to
 *        include.
 * @param str The input string.
 * @return A pointer to a newly allocated string containing the characters
 *         from the input string that satisfy the predicate function, or NULL
 *         if memory allocation failed. The returned string must be deallocated
 *         using the standard C library function `free()` when no longer needed.
 **/
string_t *string_filter(boolfunc_t func, const string_t *str);

/**********************************************************************
 *                        String Vector                               *
 **********************************************************************/

typedef struct st_strvec {
  size_t cap;
  int top;
  string_t **buf;
} string_vector_t;

/**
 * Splits a string into multiple substrings based on a delimiter character.
 *
 * @param str The string to split.
 * @param delimiter The delimiter character used for splitting.
 * @return A pointer to a newly allocated string vector object containing the
           resulting substrings, or NULL if memory allocation failed. The
 *         returned vector must be deallocated using `string_vector_deepfree()`.
 * @note The input string `str` is not modified, and the caller retains
 *       ownership of it.
 **/
string_vector_t *string_split(const string_t *str, char delimiter);

/**
 * Splits the given string into a vector of substrings based on the provided
 * delimiter.
 *
 * This function takes a string `str` and a delimiter `delimiter`. It splits
 * the `str` into  multiple substrings using the `delimiter` as a separator.
 * The resulting substrings are stored in a dynamically allocated string vector,
 * which should be freed after use.
 *
 * @param str The input string to be split.
 * @param delimiter The delimiter string used for splitting.
 * @return A dynamically allocated string vector containing the resulting
 *         substrings. It should be freed after use using
 *         `string_vector_deepfree()`.
 **/
string_vector_t *string_ssplit(const string_t *str, string_t *delimiter);

/**
 * Creates an empty string_vector.
 *
 * @return A pointer to a newly allocated empty stringt object, or NULL if
 *         memory allocation failed. The returned vector must be deallocated
 *         using `string_vector_free()` when no longer needed.
 **/
string_vector_t *string_vector_empty();

/**
 * Creates a new string_vector initialized with the provided string.
 *
 * @param str The initial string to add to the vector.
 * @return A pointer to a newly allocated string vector containing the provided
 *         string, or NULL if memory allocation failed. The returned vector
 *         must be deallocated using. `string_vector_free()`.
 * @note It is the caller's responsibility to free the allocated memory using
 *       `string_vector_free()`.
 */
string_vector_t *string_vector_new(string_t *str);

/**
 * Deallocates memory associated with a string_vector.
 *
 * @param svec The string_vector_t object to be deallocated.
 * @note This function frees the memory of the vector itself, but NOT the
 *       memory of its elements.
 */

void string_vector_free(string_vector_t *svec);

/**
 * Deallocates memory associated with a string vecto and its contained string
 * objects.
 *
 * @param svec The string_vector to be deallocated.
 * @note This function frees the memory of the vector itself and its elements.
 **/
void string_vector_deepfree(string_vector_t *svec);

/**
 * Adds a string to a string_vector
 *
 * @param svec The string_vector_t object.
 * @param str The string to add.
 * @note The vector takes ownership of the provided string, and the caller
 *       should not deallocate it manually.
 **/
void string_vector_add(string_vector_t *svec, string_t *str);

/**
 * Finds the index of a string in a string_vector_t object.
 *
 * @param svec The string vector
 * @param str The string to find.
 * @return The index of the first occurrence of the string in the vector,
 *         or -1 if not found.
 **/
int string_vector_find(const string_vector_t *svec, const string_t *str);

/**
 * Removes and returns the string at a specific index in a string_vector_t
 * object.
 *
 * @param svec The string_vector_t object.
 * @param index The index of the string to remove.
 * @return A pointer to the removed string, or  NULL if the index is out of
 *         bounds. The returned string must be deallocated using the standard C
 *         library function `free()` when no longer needed.
 **/
string_t *string_vector_remove(string_vector_t *svec, size_t index);

/**
 * Compares two string_vector objects for equality.
 *
 * @param a The first string_vector.
 * @param b The second string_vector
 * @return true if the two vectors are equal (have the same strings in the same
 *          order), false otherwise.
 **/
bool string_vector_equal(const string_vector_t *a, const string_vector_t *b);

/**
 * Retrieves the string at the specified index from the string vector.
 * If the index is out of bounds, NULL is returned.
 *
 * @param svec The string vector from which to retrieve the string.
 * @param index The index of the string to retrieve.
 * @return A pointer to the string at the specified index,
 *         or NULL if the index is out of bounds.
 */
static inline string_t *string_vector_get(const string_vector_t *svec,
                                          size_t index) {
  return ((int)index > svec->top) ? NULL : svec->buf[index];
}

/**
 * Returns the number of strings currently stored in the string vector.
 *
 * @param svec The string vector to query.
 * @return The number of strings in the string vector.
 */
static inline size_t string_vector_len(const string_vector_t *svec) {
  return (size_t)(svec->top + 1);
}

/**********************************************************************/

typedef string_t *(*strfunc_t)(string_t *);
typedef bool (*strboolfunc_t)(string_t *);
typedef string_t *(*reducefunc_t)(string_t *value, string_t *element);

/**
 * Applies a function to each string in a string vector and creates a new
 * vector of results.
 *
 * @param func The function to apply to each string in the vector.
 * @param svec The string vector
 * @return A pointer to a newly allocated string vectorcontaining the results
           of applying `func` to each string, or NULL if memory allocation
 *         failed. The returned vector must be deallocated using
 *         `string_vector_deepfree()`.
 * @note The input vector `svec` is not modified, and the caller retains
 *       ownership of it.
 **/
string_vector_t *string_vector_map(strfunc_t func, const string_vector_t *svec);

/**
 * Filters the strings in a string vector object based on a filtering function.
 *
 * @param func The filtering function to apply to each string in the vector.
 * @param svec The string_vector.
 * @return A pointer to a newly allocated string vector containing the strings
 *         that satisfy the filtering function, or NULL if memory allocation
 *         failed. The returned vector must be deallocated using
 *         `string_vector_deepfree()`.
 * @note The input vector `svec` is not modified, and the caller retains
 *       ownership of it.
 **/
string_vector_t *string_vector_filter(strboolfunc_t func,
                                      const string_vector_t *svec);

/**
 * Filters the strings in a string vector based on a filtering function.
 *
 * @param func The filtering function to apply to each string in the vector.
 * @param svec The string_vector.
 * @return A pointer to a newly allocated string_vector_t object containing
 *         the strings that satisfy the filtering function, or NULL if memory
 *         allocation failed. The returned vector must be deallocated using
 *         `string_vector_deepfree()`.
 * @note The input vector `svec` is not modified, and the caller retains
 *        ownership of it.
 **/
string_vector_t *string_vector_filter(strboolfunc_t func,
                                      const string_vector_t *svec);

/**
 * Reduces the string vector to a single string using a reduction function.
 *
 * This function applies the given reduction function `func` to each element
 * of the input string vector `svec` and an accumulator `initializer`. The
 * `func` should take two string arguments - the accumulator value and an
 * element from the vector - and return a new string  which will be used as
 * the updated accumulator for the next iteration. If the `initializer`
 * is `NULL`, an empty string is used as the initial accumulator.
 *
 * @param func The reduction function to be applied to each element of the
 *        string vector.
 * @param svec The input string vector.
 * @param initializer The initial accumulator value (can be NULL for an empty
          string).
 * @return A dynamically allocated string that represents the accumulated
 *         value after applying the reduction function to each element of the
 *         string vector. The returned string must be deallocated
 *         using the standard C library function `free()` when no longer needed.
 */
string_t *string_vector_reduce(reducefunc_t func, const string_vector_t *svec,
                               string_t *initializer);

/**********************************************************************/

/**
 * Returns the version string of the libstring library.
 *
 * @return A pointer to the null-terminated version string.
 **/
const char *libstring_version();
