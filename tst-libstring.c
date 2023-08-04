#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "libstring.h"

#define IDENT 20
#define PASSED  printf("%s %s %s\n", COLOR_GREEN, "PASSED", COLOR_DEFAULT)
#define FAILED  printf("%s %s %s\n", COLOR_RED,   "FAILED", COLOR_DEFAULT)

/***********************************************************************/

char to_upper(char c) {
  return (char) toupper(c);
}


bool is_upper(char c) {
  return isupper(c);
}

/***********************************************************************/

static void verify(const char *name, string_t *s1, string_t *s2) {
  printf("Test %s: %*s",name, (int)(IDENT-strlen(name)),"");
  string_equal(s1,s2) ? PASSED : FAILED;  
  free(s1);
  free(s2);
}

/***********************************************************************/
           
static void verify_neg(const char *name, string_t *s1, string_t *s2) {
  printf("Test %s: %*s",name, (int)(IDENT-strlen(name)),"");
  !string_equal(s1,s2) ? PASSED : FAILED;  
  free(s1);
  free(s2);
}

/***********************************************************************/

static void verify_bool(const char *name, string_t *s1, string_t *s2, bool b) {
  printf("Test %s: %*s",name, (int)(IDENT-strlen(name)),"");
  b ? PASSED : FAILED;  
  free(s1);
  free(s2);
}


/***********************************************************************/
                  
void tst_colored() {
  string_t *s = string_colored("String regression tests", CYAN);
  string_println(s);
  free(s);
}

void tst_concat1() {
  string_t *s1 = string_new("Hello ");
  string_t *s2 = string_new("World");
  string_t *s = string_concat(s1,s2);
  verify("concat 1",s,string_new("Hello World"));

  free(s1);
  free(s2);
}

void tst_concat2() {
  string_t *s1 = string_new("🤔🙏");
  string_t *s2 = string_new("👍");
  string_t *s = string_concat(s1,s2);
  verify("concat 2",s,string_new("🤔🙏👍"));
  free(s1);
  free(s2);
}

void tst_trim1() {
  string_t *s1 = string_new("\t \t   ABC \n\n  \t");
  string_t *s2 = string_trim(s1);

  verify("trim1", s2, string_new("ABC"));
  free(s1);
}


void tst_trim2() {
  string_t *s1 = string_new("\t  \n\n\n");
  string_t *s2 = string_trim(s1);

  verify("trim2", s2, string_new(""));
  free(s1);
}

void tst_map() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_map(to_upper, s1);

  verify("map", s2, string_new("HELLO WORLD!"));
  free(s1);
}
                

void tst_filter() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_filter(is_upper, s1);

  verify("filter", s2, string_new("HW"));
  free(s1);
}


void tst_equal1() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_new("Hello World!");
  verify("equal 1", s1, s2);
}

void tst_equal2() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_new("Hallo Welt!");
  verify_neg("equal 2", s1, s2);
}


void tst_equal3() {
  string_t *s1 = string_new("Hello World! ");
  string_t *s2 = string_new("Hello World!");
  verify_neg("equal 3", s1, s2);
}


void tst_equal4() {
  string_t *s1 = string_new("");
  string_t *s2 = string_new("");
  verify("equal 4", s1, s2);
}

void tst_compare1() {
  string_t *s1 = string_new("ABC");
  string_t *s2 = string_new("ABC");
  verify_bool("compare 1", s1, s2, string_compare(s1,s2) == 0);
}


void tst_compare2() {
  string_t *s1 = string_new("Hello");
  string_t *s2 = string_new("Hallo");
  verify_bool("compare 2", s1, s2, string_compare(s1,s2) > 0);
}


void tst_compare3() {
  string_t *s1 = string_new("ABCD");
  string_t *s2 = string_new("ABCDE");
  verify_bool("compare 3", s1, s2, string_compare(s1,s2) < 0);
}


void tst_compare4() {
  string_t *s1 = string_new("");
  string_t *s2 = string_new("");
  verify_bool("compare 4", s1, s2, string_compare(s1,s2) == 0);
}


void tst_readfd() {
  int pfd[2];
  assert(pipe(pfd) != -1);

  string_t *s1 = string_new("Hello World!");
  char *s = string_tocstr(s1);
  assert(write(pfd[1], s, strlen(s)) == (ssize_t) strlen(s)) ;
  string_t *s2 = string_readfd(pfd[0]);
  
  verify("readfd", s1, s2);
  close(pfd[0]);
  close(pfd[1]);
  free(s);
}

void tst_substring() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_substring(s1,6,10);
  verify("substring", s2, string_new("World"));
  free(s1);
}



void tst_substring_index1() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_new("World");
  int r = string_substring_index(s1,s2);
  verify_bool("substring index 1", s1, s2, r==6);
}


void tst_substring_index2() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_new("World");
  int r = string_substring_index(s2,s1);
  verify_bool("substring index 1", s1, s2, r==-1);
}

void tst_substring_index3() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_new("Hallo");
  int r = string_substring_index(s2,s1);
  verify_bool("substring index 3", s1, s2, r==-1);
}

void tst_substring_index4() {
  string_t *s1 = string_new("");
  string_t *s2 = string_new("");
  int r = string_substring_index(s2,s1);
  verify_bool("substring index 4", s1, s2, r==0);
}


void tst_is_substring1() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_new("World");
  verify_bool("is substring 1", s1, s2, string_is_substring(s1,s2,6));
}


void tst_is_substring2() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_new("World");
  verify_bool("is substring 2", s1, s2, !string_is_substring(s1,s2,2));
}



int main() {
  tst_colored();
  tst_concat1();
  tst_concat2();
  tst_trim1();
  tst_trim2();
  tst_map();
  tst_filter();
  tst_equal1();
  tst_equal2();
  tst_equal3();
  tst_equal4();
  tst_compare1();
  tst_compare2();
  tst_compare3();
  tst_compare4();
  tst_readfd();
  tst_substring_index1();
  tst_substring_index2();
  tst_substring_index3();
  tst_substring_index4();
  tst_is_substring1();
  tst_is_substring2();
}


