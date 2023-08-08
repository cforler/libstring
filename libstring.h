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


static inline string_t *string_vector_get(string_vector_t *svec, size_t index) {
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
