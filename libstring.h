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


string_t *string_colored(const char *str, enum stringcolor c);
string_t *string_new(const char *str);
string_t *string_clone(const string_t *str);
string_t *string_readfd(int fd);


string_t *string_concat(const string_t *s1, const string_t *s2);
string_t *string_trim(const string_t *str);

/**********************************************************************/

typedef struct st_stack {
   string_t **a;  
   size_t cap;
   int top;
} string_stack_t;


void string_stack_free(string_stack_t *s);
bool string_stack_is_empty(string_stack_t *s);
string_t *string_stack_pop(string_stack_t *s);

string_stack_t *string_split(const string_t *str, char delimiter);


/**********************************************************************/


int string_compare(const string_t *s1, const string_t *s2);
bool  string_equal(const string_t *s1, const string_t *s2);

/**********************************************************************/

typedef char (*charfunc_t)(char);
typedef bool (*boolfunc_t)(char);

string_t *string_map(charfunc_t func, const string_t *str);
string_t *string_filter(boolfunc_t func, const string_t *str);

/**********************************************************************/


string_t *string_substring(const string_t *str, size_t start, size_t end);
char *string_tocstr(const string_t *str);
int   string_substring_index(const string_t *str, const string_t *substring);
bool  string_is_substring(const string_t *str, const string_t *sub, size_t off);

/**********************************************************************/

static inline char string_index(const string_t *s, size_t index) {
  return (index < s->len) ? s->buf[index] : -1;
}

static inline string_t *string_read() {
  return string_readfd(STDIN_FILENO);
}

static inline bool string_empty(const string_t *s) {
  return s->len == 0;
}

static inline int string_len(const string_t *s) {
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
