#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>

#include "libstring.h"

#define BUF_LEN 65536


string_t *string_colored(const char *str, enum stringcolor c) {
  int n =  strlen(str) + strlen(COLOR_DEFAULT) + strlen(COLOR_BLACK);
  string_t *s = malloc(sizeof(string_t) + n);
  s->len = n;
  switch(c) {
  case BLACK:   memcpy(s->buf,COLOR_BLACK,strlen(COLOR_BLACK));     break;
  case RED:     memcpy(s->buf,COLOR_RED,strlen(COLOR_RED));         break;
  case GREEN:   memcpy(s->buf,COLOR_GREEN,strlen(COLOR_GREEN));     break;
  case YELLOW:  memcpy(s->buf,COLOR_YELLOW,strlen(COLOR_YELLOW));   break;
  case BLUE:    memcpy(s->buf,COLOR_BLUE,strlen(COLOR_BLUE));       break;
  case MAGENTA: memcpy(s->buf,COLOR_MAGENTA,strlen(COLOR_MAGENTA)); break;
  case CYAN:    memcpy(s->buf,COLOR_CYAN,strlen(COLOR_MAGENTA));    break;
  case WHITE:   memcpy(s->buf,COLOR_WHITE,strlen(COLOR_WHITE));     break;
  default: break;
  }
  memcpy(&(s->buf[strlen(COLOR_BLACK)]), str, strlen(str));
  n -=  strlen(COLOR_DEFAULT);
  memcpy(&(s->buf[n]), COLOR_DEFAULT, strlen(COLOR_DEFAULT));
  return s;
}

/**********************************************************************/

string_t *string_nnew(const char *str, size_t len) {
  string_t *s = malloc(sizeof(string_t) + len);
  s->len = len;
  memcpy(s->buf, str, len);
  return s;
}

/**********************************************************************/

string_t *string_new(const char *str) {
  return string_nnew(str, strlen(str));
}

/**********************************************************************/

string_t *string_clone(const string_t *str) {
  return string_nnew(str->buf, str->len);
}

/**********************************************************************/

string_t *string_readfd(int fd) {
  char buf[BUF_LEN];
  int n = read(fd ,buf, BUF_LEN-1);
  if(n<0) return string_new("");
  buf[n] = '\0';
  return string_new(buf);
}

/**********************************************************************/

string_t *string_concat(const string_t *s1, const string_t *s2) {
  string_t *s = malloc(sizeof(string_t) + s1->len + s2->len);
  s->len = s1->len + s2->len;
  memcpy(s->buf, s1->buf, s1->len);
  memcpy(s->buf+s1->len, s2->buf, s2->len);
  return s;
}

/**********************************************************************/

string_t *string_trim(const string_t *str) {
  size_t l=0;
  int r= str->len-1;
  while(l < str->len && isspace(str->buf[l])) l+=1;
  while(r>=0 && isspace(str->buf[r])) r-=1;
  r+=1;
  
  if(l> (size_t) r) return string_new("");

  return string_nnew( &(str->buf[l]), r-l);
}

/**********************************************************************/

string_t *string_map(charfunc_t fun, const string_t *str) {
  string_t *s = string_clone(str);
  for(size_t i=0;i<s->len;i++) {
    s->buf[i] = fun(s->buf[i]);
  }
  return s;                             
}

/**********************************************************************/

string_t *string_filter(boolfunc_t fun, const string_t *str) {
  int c=0;
  char *tmp = malloc(str->len);
  memcpy(tmp,str->buf,str->len);
  for(size_t i=0; i < str->len; i++)
    if(fun(str->buf[i])) tmp[c++] = str->buf[i]; 
  tmp[c] = '\0';
  string_t *s = string_new(tmp);
  free(tmp);
  return s;                             
}

/**********************************************************************/

int string_compare(const string_t *s1,const string_t *s2) {
  int n = (s1->len < s2->len) ? s1->len : s2->len;
  int r = strncmp(s1->buf, s2->buf, n);

  return r ? r : (int)(s1->len - s2->len);
}

/**********************************************************************/

bool string_equal(const string_t *s1, const string_t *s2) {
  if(s1->len != s2->len) return false;
  return strncmp(s1->buf,s2->buf,s1->len) == 0 ? true : false;   
}

/**********************************************************************/

string_t *string_substring(const string_t *str, size_t start, size_t end){
  if(!(start <= end) && (end <= str->len))
    return string_new("");

  return string_nnew(&(str->buf[start]), end-start);
}

/**********************************************************************/

char *string_tocstr(const string_t *str) {
  char *cstr = malloc(str->len +1);
  memcpy(cstr, str->buf, str->len);
  cstr[str->len] = '\0';
  return cstr;
}

/**********************************************************************/

int string_substring_index(const string_t *str, const string_t *substring) {
  if(substring->len == 0) return 0;
  if(substring->len > str->len) return -1;
  
  for(size_t i=0; i< str->len - substring->len; i++) 
    for(size_t j=0; str->buf[i+j] == substring->buf[j]; j++)
      if( (j+1) == substring->len-1) return i;
  
  return -1;
}

/**********************************************************************/

bool string_is_substring(const string_t *str, const string_t *sub, size_t off) {
  if(sub->len+off > str->len) return false;
  if(sub->len == 0) return true;
  return strncmp(&(str->buf[off]), sub->buf, sub->len) ? false : true;
  
}

/**********************************************************************/


string_stack_t *string_split(const string_t *str, char delimiter);




/*************************************************************************
 *                         String Stack                                  *
 *************************************************************************/

string_stack_t *string_stack_init(size_t cap) {
  string_stack_t *s = malloc(sizeof(string_stack_t));
  s->a = malloc(cap * sizeof(string_t *));
  s->cap = cap;
  s->top = -1;
  return s;
}

void string_stack_free(string_stack_t *s) {
   free(s->a);
   free(s);
}

bool string_stack_is_empty(string_stack_t *s) {
    return s->top == -1;
}

bool string_stack_is_full(string_stack_t *s) {
    return (size_t) (s->top + 1) == s->cap; 
}

void string_stack_resize(string_stack_t *s) {
  string_t **cs = malloc(2 * s->cap * sizeof(string_t *));
  memcpy(cs,s->a, s->cap * sizeof(string_t *));
  s->cap *= 2;
  free(s->a);
  s->a = cs;
}

void string_stack_push(string_stack_t *s, string_t *str) {
  if (string_stack_is_full(s)) string_stack_resize(s);
  s->top += 1;
  s->a[s->top] = str;
}

string_t *string_stack_pop(string_stack_t *s) {
  if(string_stack_is_empty(s)) return string_new("");
  s->top -=1;
  return s->a[s->top+1];
}

