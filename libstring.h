#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define COLOR_DEFAULT "\033[0m"
#define COLOR_BLACK   "\033[30m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

enum stringcolor {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};

typedef struct {
  size_t len;
  char buf[];
} string_t;


/**
 * Creates a new colored string_t initialized with the provided character array.
 *
 * @param str A null-terminated character array to initialize the string with.
 * @param c The color to apply to the string (specified by the enum stringcolor).
 * @return A pointer to the newly allocated and colored string containing a copy
 *         of the input character array, or NULL if memory allocation failed.
 *         The returned string must be deallocated using the standard C library function `free()`
 *         when no longer needed.
 */
string_t *string_colored(const char *str, enum stringcolor c);


/**
 * Allocates a new string_t initialized with the provided character array.
 *
 * @param str A null-terminated character array to initialize the string with.
 * @return A pointer to the newly allocated string_t object containing a copy of the
 *         input character array, or NULL if memory allocation failed.
 *         The returned string must be deallocated using the standard C library function `free()`
 *         when no longer needed.
 **/
string_t *string_new(const char *str);

/**
 * Creates a new string_t object as a copy of the provided string.
 *
 * @param str The source string_t object to be cloned.
 * @return A pointer to the newly allocated string_t containing a copy of the
 *         source string, or NULL if memory allocation failed.
 *         The returned string must be deallocated using the standard C library function `free()`
 *         when no longer needed.
 *
 **/
string_t *string_clone(const string_t *str);

/**
 * Reads up to 64 KB of data from the specified file descriptor and
 * creates a new string containing the read data.
 *
 * @param fd The file descriptor to read from.
 * @return A pointer to the newly allocated string_t containing the read data
 *         or an empty string if an error occurred or the end of the file has been
 *         reached. The returned string must be deallocated using the standard C
 *         library function `free()` when no longer needed.

 **/
string_t *string_readfd(int fd);


/**
 * Reads a line from a file stream, with a maximum of 64 KB of data,
 * and stores it as a newly allocated string.
 *
 * @param stream The file stream to read from.
 * @return A pointer to the newly allocated string containing the read line or an empty
 *         string if an error occurred or the end of the file has been reached.
 *         The returned string must be deallocated using the standard C library function `free()`
 *         when no longer needed.
 **/
string_t *string_readline(FILE *stream);


/**
 * Concatenates two strings and returns a new string containing the combined contents.
 *
 * @param s1 The first string to concatenate.
 * @param s2 The second string to concatenate.
 * @return A pointer to a newly allocated string containing the concatenated
 *         contents of the input strings, or NULL if memory allocation failed.
 *         The returned string must be deallocated using the standard C library function `free()`
 *         when no longer needed.
 **/
string_t *string_concat(const string_t *s1, const string_t *s2);


/**
 * Creates a new string by removing leading and trailing whitespace characters
 * from the provided string.
 *
 * @param str The input string to trim.
 * @return A pointer to a newly allocated string containing the trimmed contents of the input string,
 *         or NULL if memory allocation failed. The returned string must be deallocated using `free()`
 *         when no longer needed.
 **/
string_t *string_trim(const string_t *str);

/**
 * Compares two strings lexicographically.
 *
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @return An integer greater than, equal to, or less than 0 if s1 is greater than,
 *         equal to, or less than s2, respectively.
 **/
int string_compare(const string_t *s1, const string_t *s2);

/**
 * Checks if two string_t objects have equal contents.
 *
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @return True if the contents of s1 are equal to the contents of s2, false otherwise.
 **/
bool  string_equal(const string_t *s1, const string_t *s2);


/**
 * Creates a new string that represents a substring of the input string.
 *
 * @param str The input string.
 * @param start The starting index of the substring.
 * @param end The ending index of the substring 
 * @return A pointer to a newly allocated string containing the specified substring,
 *         or NULL if memory allocation failed or an empty string if the indices are out of bounds.
 *         The returned string must be deallocated using the standard C library function `free()`
 *         when no longer needed.
 **/
string_t *string_substring(const string_t *str, size_t start, size_t end);


/**
 * Converts a string to a null-terminated C string.
 *
 * @param str The input string.
 * @return A pointer to a null-terminated C string containing the contents of the input string,
 *         or NULL if memory allocation failed.
 *         The returned C string must be deallocated using the standard C library function `free()`
 *         when no longer needed.
 **/
char *string_tocstr(const string_t *str);


/**
 * Finds the first occurrence of a substring within a string.
 *
 * @param str The input string to search in.
 * @param substring The substring to search for.
 * @return The index of the first occurrence of the substring in the input string, or -1 if not found.
 **/
int   string_substring_index(const string_t *str, const string_t *substring);

/**
 * Checks if a substring appears in the input string at a specified offset.
 *
 * @param str The input string to search in.
 * @param sub The substring to search for.
 * @param off The offset within the input string to start the search.
 * @return True if the substring appears in the input string at the specified offset, false otherwise.
 */
bool  string_is_substring(const string_t *str, const string_t *sub, size_t off);


/**********************************************************************/

typedef char (*charfunc_t)(char);
typedef bool (*boolfunc_t)(char);

string_t *string_map(charfunc_t func, const string_t *str);
string_t *string_filter(boolfunc_t func, const string_t *str);

/**********************************************************************
 *                        String Vector                               * 
 **********************************************************************/


typedef struct st_strvec {
  size_t cap;
  int top;
  string_t **buf;  
} string_vector_t;


typedef string_t * (*strfunc_t)(string_t *);
typedef bool (*strboolfunc_t)(string_t *);

string_vector_t *string_vector_empty();
string_vector_t *string_vector_new(string_t *str);
void string_vector_free(string_vector_t *svec);
void string_vector_deepfree(string_vector_t *svec);

void string_vector_add(string_vector_t *svec,  string_t *str);
int string_vector_find(const string_vector_t *svec, const string_t *str);
string_t *string_vector_remove(string_vector_t *svec, size_t index);
bool string_vector_equal(const string_vector_t *a, const string_vector_t *b);


string_vector_t *string_vector_map(strfunc_t func, const string_vector_t *svec);
string_vector_t *string_vector_filter(strboolfunc_t func,
				      const string_vector_t *svec);

/**********************************************************************/

string_vector_t *string_split(const string_t *str, char delimiter);

/**********************************************************************/


static inline string_t *string_vector_get(const string_vector_t *svec,
                                          size_t index) {
  return ((int) index > svec->top) ? string_new("") : svec->buf[index];
}

static inline size_t string_vector_len(const string_vector_t *svec) {
  return (size_t)(svec->top+1);
}

static inline char string_get(const string_t *s, size_t index) {
  return (index < s->len) ? s->buf[index] : -1;
}

static inline string_t *string_read() {
  return string_readfd(STDIN_FILENO);
}

static inline string_t *string_readf(FILE *stream) {
  return string_readfd(fileno(stream));
}


static inline bool string_empty(const string_t *s) {
  return s->len == 0;
}

static inline size_t string_len(const string_t *s) {
  return s->len;
}

/**********************************************************************/

static inline int string_print(const string_t *s) {
  return  printf("%.*s", (int) s->len,s->buf);
}

static inline int string_printf(const string_t *s, FILE *f) {
  return  fprintf(f,"%.*s", (int) s->len, s->buf);
}

static inline int string_println(const string_t *s) {
  return printf("%.*s\n", (int) s->len, s->buf);
}

static inline int string_printlnf(const string_t *s, FILE *f) {
  return  fprintf(f,"%.*s\n",(int) s->len, s->buf);
}
