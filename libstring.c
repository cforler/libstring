#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>


#define unlikely(x)    __builtin_expect(!!(x), 0)

#include "libstring.h"

#define BUF_LEN 65536
#define CAP_DEFAULT 10


string_t *string_colored(const char *str, enum stringcolor c) {
  int n =  strlen(str) + strlen(COLOR_DEFAULT) + strlen(COLOR_BLACK);
  string_t *s = malloc(sizeof(string_t) + n);
  if(unlikely(s == NULL)) return NULL;
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
  if(unlikely(s == NULL)) return NULL;
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
  if(unlikely(n<0) ) return string_new("");
  buf[n] = '\0';
  return string_new(buf);
}

/**********************************************************************/

string_t *string_readline(FILE *stream) {
  char buf[BUF_LEN];
  if(!fgets(buf, BUF_LEN-1, stream)) return string_new("");
  buf[strlen(buf)-1] = '\0';
  return string_new(buf);
}


/**********************************************************************/

string_t *string_concat(const string_t *s1, const string_t *s2) {
  string_t *s = malloc(sizeof(string_t) + s1->len + s2->len);
  if(unlikely(s == NULL)) return NULL;
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
  
  if( unlikely(l> (size_t) r)) return string_new("");

  return string_nnew( &(str->buf[l]), r-l);
}

/**********************************************************************/

string_t *string_map(charfunc_t fun, const string_t *str) {
  string_t *s = string_clone(str);
  if(unlikely(s == NULL)) return NULL;
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
  if(unlikely(!(start <= end) && (end <= str->len))) return NULL;

  return string_nnew(&(str->buf[start]), end-start);
}

/**********************************************************************/

string_t *string_repeat(const string_t *str, size_t times) {
  size_t n =  str->len * times;
  if( unlikely(!n)) return string_new("");
  
  string_t *s = malloc(sizeof(string_t) + n);
  if( unlikely(!s)) return NULL;
  
  s->len = n;
  for(size_t i=0; i < times; i++)
    memcpy(&(s->buf[i*(str->len)]), str->buf, str->len);
  
  return s;
}

/**********************************************************************/


char *string_tocstr(const string_t *str) {
  char *cstr = malloc(str->len +1);
  if( unlikely(cstr==NULL)) return NULL;
  memcpy(cstr, str->buf, str->len);
  cstr[str->len] = '\0';
  return cstr;
}

/**********************************************************************/

static int string_substring_index_offset(const string_t *str, const string_t *substring, size_t offset) {
  if(unlikely(substring->len == 0)) return offset;
  if(unlikely(substring->len > (str->len - offset) )) return -1;

  for(size_t i=offset; i<= str->len - substring->len; i++) {
    for(size_t j=0; j < substring->len; j++) {
      if(str->buf[i+j] != substring->buf[j]) break;
      if( (j+1) == substring->len) return i;
    }
  }
  return -1;
}


int string_substring_index(const string_t *str, const string_t *substring) {
  return string_substring_index_offset(str,substring, 0);
}


/**********************************************************************/

bool string_is_substring(const string_t *str, const string_t *sub, size_t off) {
  if(unlikely(sub->len+off > str->len)) return false;
  if(unlikely(sub->len == 0)) return true;
  return strncmp(&(str->buf[off]), sub->buf, sub->len) ? false : true;
  
}



/**********************************************************************/

string_t *string_replace_char(const string_t *str, char old, char new) {
  string_t *s =  string_clone(str);

  for(size_t i=0; i < s->len; i++) 
    s->buf[i] = (s->buf[i] == old) ? new : s->buf[i];

  return s;
}


/**********************************************************************/

/*
 * Implementation of the string_replace() function is based on the
 * suggestion by OpenAI's GPT-3.5
 */

string_t *string_replace(const string_t *str, const string_t *old, const string_t *new) {
  size_t n = 0;
  int offset = 0;
  
  while( (offset = string_substring_index_offset(str, old, offset)) > -1  ) {
    offset+=old->len;
    n += 1;
  }
  
  if(n==0) return string_clone(str);
  size_t len =  str->len + n * (new->len - old->len);
  string_t *s = malloc(sizeof(string_t) + len);
  s->len = len;  
  
  char *t = s->buf;
  int curr = 0;
  offset=0;
  while(n--) {
    offset = string_substring_index_offset(str, old, offset);
    memcpy(t, str->buf+curr, offset-curr);
    t += offset-curr;

    memcpy(t, new->buf, new->len);
    t += new->len;
    curr = offset + old->len;
    offset+=old->len;
  }
  memcpy(t, str->buf+curr, str->len-curr);
  
  
  return s;
}
  


/**********************************************************************/

string_vector_t *string_split(const string_t *str, char delimiter) {
  string_vector_t *svec = string_vector_empty();
  size_t start = 0;
  for(size_t i=0; i< string_len(str); i++)
    if(str->buf[i]==delimiter) {
      string_vector_add(svec, string_nnew(&(str->buf[start]), i-start));
      start = i+1;
    }
  string_vector_add(svec, string_nnew(&(str->buf[start]), string_len(str)-start));
  return svec;
}


/**********************************************************************/

string_vector_t *string_ssplit(const string_t *str, string_t *delimiter) {
  string_vector_t *svec = string_vector_empty();
  int idx = string_substring_index(str, delimiter);
  if(idx==-1) {
    string_vector_add(svec, string_clone(str));
    return svec;
  }
  string_vector_add(svec, string_nnew(str->buf, idx));
  idx += string_len(delimiter);  
  
  for(size_t i=idx; i<= string_len(str)-string_len(delimiter); i++) {
    for(size_t j=0; str->buf[i+j] == delimiter->buf[j]; j++)
      if( (j+1) == delimiter->len-1) {
        string_vector_add(svec, string_nnew(&(str->buf[idx]), i-idx));
        idx = i + string_len(delimiter);
        i += string_len(delimiter);  
        break;
      }
  }
  if( (size_t) idx <= string_len(str))
    string_vector_add(svec, string_nnew(&(str->buf[idx]), string_len(str)-idx));
    
  return svec;
}


/*************************************************************************
 *                           String Vector                               *
 *************************************************************************/

string_vector_t *string_vector_empty() {
  string_vector_t *svec = malloc(sizeof(string_vector_t));
  if(unlikely(svec==NULL)) return NULL;
  svec->buf = malloc(CAP_DEFAULT * sizeof(string_t *));
  svec->cap = CAP_DEFAULT;
  svec->top = -1;
  return svec;
}

string_vector_t *string_vector_new(string_t *str) {
  string_vector_t *svec = string_vector_empty();
  if(unlikely(svec==NULL)) return NULL;
  string_vector_add(svec, str);
  return svec;
}
  
bool string_vector_is_empty(const string_vector_t *svec) {
    return svec->top == -1;
}


void string_vector_free(string_vector_t *svec) {
  free(svec->buf);
  free(svec);
}

void string_vector_deepfree(string_vector_t *svec){
  for(int i=0;i <= svec->top; i++) free(svec->buf[i]);
  string_vector_free(svec);
}

bool string_vector_is_full(string_vector_t *svec) {
    return (size_t) (svec->top + 1) == svec->cap; 
}

void string_vector_resize(string_vector_t *svec) {
  svec->buf = realloc(svec->buf, 2 * svec->cap * sizeof(string_t *));
  svec->cap *= 2;
}

int string_vector_find(const string_vector_t *svec, const string_t *str) {
  for(int i= 0; i<=svec->top; i++) 
    if (string_equal(svec->buf[i] , str)) return i;
  return -1;
}

void string_vector_add(string_vector_t *svec,  string_t *str) {
  if (string_vector_is_full(svec)) string_vector_resize(svec);
  svec->top += 1;
  svec->buf[svec->top] = str;
}


/**********************************************************************/


string_t *string_vector_remove(string_vector_t *svec, size_t index) {
  if ((int)index > svec->top) return NULL;
  string_t *str = svec->buf[index];
  for(int i=index; i<svec->top; i++) svec->buf[i] = svec->buf[i+1];
  svec->top -=1;
  return str;
}


/**********************************************************************/

bool string_vector_equal(const string_vector_t *a, const string_vector_t *b) {
  if(a->top != b->top) return false;
  for(int i=0;i<=a->top;i++) if(!string_equal(a->buf[i],b->buf[i])) return false;
  return true;   
}

/**********************************************************************/

string_vector_t *string_vector_map(strfunc_t func,
                                   const string_vector_t *svec) {
  if(string_vector_len(svec) == 0)  return string_vector_empty();

  string_vector_t *res = malloc(sizeof(string_vector_t));
  if(unlikely(!res)) return NULL;
  res->buf = malloc(svec->cap * sizeof(string_t *));
  res->cap = svec->cap;
  res->top = svec->top;
  memcpy(res->buf, svec->buf, svec->top * sizeof(string_t *));
  
  for(int i = 0; i<= svec->top; i++) res->buf[i] = func(svec->buf[i]);

  return res;
}

/**********************************************************************/

string_vector_t *string_vector_filter(strboolfunc_t func,
                                      const string_vector_t *svec) {
  string_vector_t *res = string_vector_empty();
  if(unlikely(!res)) return NULL;
  for(int i = 0; i<= svec->top; i++)
    if(func(svec->buf[i])) string_vector_add(res, string_clone(svec->buf[i])); 
  
  return res;     
}

/**********************************************************************/

string_t *string_vector_reduce(reducefunc_t func,
                               const string_vector_t *svec,
                               string_t *initializer) {
  string_t *val = (initializer) ? string_clone(initializer) : string_new("");
  
  for(int i = 0; i<= svec->top; i++) {
    string_t *t = func(val,svec->buf[i]);
    free(val);
    val = t;
  }
  return val;
}


/**********************************************************************/


const char *libstring_version() {
  return LIBSTRING_VERSION;
}
