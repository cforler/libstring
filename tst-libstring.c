#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "libstring.h"

#define IDENT 25
#define PASSED  printf("%s %s %s\n", COLOR_GREEN, "PASSED", COLOR_DEFAULT)
#define FAILED  printf("%s %s %s\n", COLOR_RED,   "FAILED", COLOR_DEFAULT)

/***********************************************************************/
/***********************************************************************/

char to_upper(char c) {
  return (char) toupper(c);
}

bool is_upper(char c) {
  return isupper(c);
}

/***********************************************************************/
/***********************************************************************/

static void verify_bool(const char *name, string_t *s1, string_t *s2, bool b) {
  printf("Test %s: %*s",name, (int)(IDENT-strlen(name)),"");
  b ? PASSED : FAILED;
  if(s1 != s2)  free(s2);
  free(s1);
}


/***********************************************************************/

static void verify(const char *name, string_t *s1, string_t *s2) {
  verify_bool(name,s1,s2, string_equal(s1,s2));
}

/***********************************************************************/
           
static void verify_neg(const char *name, string_t *s1, string_t *s2) {
  verify_bool(name,s1,s2, !string_equal(s1,s2));
}

/***********************************************************************/
                  
void tst_colored() {
  string_t *s = string_colored("String tests", CYAN);
  string_println(s);
  free(s);
}

/***********************************************************************/

void tst_concat1() {
  string_t *s1 = string_new("Hello ");
  string_t *s2 = string_new("World");
  string_t *s = string_concat(s1,s2);
  verify("concat 1",s,string_new("Hello World"));

  free(s1);
  free(s2);
}

/***********************************************************************/

void tst_concat2() {
  string_t *s1 = string_new("🤔🙏");
  string_t *s2 = string_new("👍");
  string_t *s = string_concat(s1,s2);
  verify("concat 2",s,string_new("🤔🙏👍"));
  free(s1);
  free(s2);
}

/***********************************************************************/

void tst_trim1() {
  string_t *s1 = string_new("\t \t   ABC \n\n  \t");
  string_t *s2 = string_trim(s1);

  verify("trim1", s2, string_new("ABC"));
  free(s1);
}

/***********************************************************************/

void tst_trim2() {
  string_t *s1 = string_new("\t  \n\n\n");
  string_t *s2 = string_trim(s1);

  verify("trim2", s2, string_new(""));
  free(s1);
}

/***********************************************************************/

void tst_map() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_map(to_upper, s1);

  verify("map", s2, string_new("HELLO WORLD!"));
  free(s1);
}
                
/***********************************************************************/

void tst_filter() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_filter(is_upper, s1);

  verify("filter", s2, string_new("HW"));
  free(s1);
}

/***********************************************************************/

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

void tst_equal5() {
  string_t *s1 = string_new("😜̸̳︦");
  string_t *s2 = string_new("😜\u0333\u0338\uFE26");
  verify("equal 5", s1, s2);
}

/***********************************************************************/

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

/***********************************************************************/

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

/***********************************************************************/

void tst_substring() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_substring(s1,6,10);
  verify("substring", s2, string_new("World"));
  free(s1);
}

/***********************************************************************/

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

/***********************************************************************/

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

/***********************************************************************/

void tst_readline() {
  string_t *s1 = string_new("#include <stdlib.h>");
  FILE *f = fopen("tst-libstring.c", "r");
  string_t *s2 = string_readline(f);
  fclose(f);
  verify("readline", s1, s2);
}

/***********************************************************************/

void tst_repeat1() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_repeat(s1,1);
  verify("repeat 1", s1, s2);
}

/***********************************************************************/

void tst_repeat2() {
  string_t *s1 = string_new("Foo");
  string_t *s2 = string_new("FooFooFooFooFoo");
  string_t *s3 = string_repeat(s1,5);
  verify("repeat 2", s2, s3);
  free(s1);
}

/***********************************************************************/

void tst_repeat3() {
  string_t *s1 = string_new("Hello World!");
  string_t *s2 = string_new("");
  string_t *s3 = string_repeat(s1,0);
  verify("repeat 3", s2, s3);
  free(s1);
}


/***********************************************************************/

void string_tests() {
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
  tst_equal5();
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
  tst_readline();
  tst_repeat1();
  tst_repeat2();
  tst_repeat3();
  }

/**********************************************************************/
/**********************************************************************/

void test_strvec_new() {
  char *name =  "string vector new";
  string_t *str = string_new("Hello World!");
  string_vector_t *svec = string_vector_new(str);
  printf("Test %s: %*s",name, (int)(IDENT-strlen(name)),"");
  if((string_vector_len(svec) == 1) && (svec->buf[0] == str)) PASSED;
  else FAILED;  
  free(str);
  string_vector_free(svec);
}

/**********************************************************************/

void test_strvec_add() {
  string_t *s1 = string_new("Hello");
  string_t *s2 = string_new("World");
  string_vector_t *svec = string_vector_new(s1);
  string_vector_add(svec, s2);
  bool result = (string_vector_len(svec) == 2) &&
    (svec->buf[0] == s1) && (svec->buf[1] == s2);
  verify_bool("string vector add",s1,s2,result);
  string_vector_free(svec);
}

/**********************************************************************/

void test_strvec_resize() {
  char *name =  "string vector resize";
  
  string_t *str = string_new("Hello");
  string_vector_t *svec = string_vector_new(str);
  
  size_t cap = svec->cap;
  
  for(size_t i=0;i<2*cap;i++) 
    string_vector_add(svec, str);

  bool result = 4*cap == svec->cap;

  for(size_t i=0;i<2*cap;i++) 
    if (svec->buf[i] != str) {
      result = false;
      break;
    }
  
  printf("Test %s: %*s",name, (int)(IDENT-strlen(name)),"");
  result ? PASSED : FAILED;  
  free(str);
  string_vector_free(svec);
  
}

/**********************************************************************/

void test_strvec_find1() {
  string_t *s1 = string_new("Hello");
  string_t *s2 = string_new("World");
  string_vector_t *svec = string_vector_new(s1);
  string_vector_add(svec, s2);

  
  verify_bool("string vector find 1",s1,s2, string_vector_find(svec,s2) == 1);
  string_vector_free(svec);
}


/**********************************************************************/

void test_strvec_find2() {
  string_t *s1 = string_new("Hello");
  string_t *s2 = string_new("World");
  string_vector_t *svec = string_vector_new(s1);
  string_vector_add(svec, s1);

  verify_bool("string vector find 2",s1,s2, string_vector_find(svec,s2) == -1);
  string_vector_free(svec);
}

/**********************************************************************/

void test_strvec_remove1() {
  string_t *s1 = string_new("Hello");
  string_vector_t *svec = string_vector_new(s1);
  string_t *s2 = string_vector_remove(svec,0);

  bool result = (string_vector_len(svec) == 0) && (s1 == s2);
  verify_bool("string vector remove 1",s1,s2,result);
  string_vector_free(svec);
}


void test_strvec_remove2() {
  string_t *s1 = string_new("Hello");
  string_vector_t *svec = string_vector_new(s1);
  string_vector_remove(svec,0);
  string_t *s2 = string_vector_remove(svec,0);
  
  bool result = (string_vector_len(svec) == 0) && (s2 ==NULL);
  
  verify_bool("string vector remove 2",s1,s2,result);
  string_vector_free(svec);
}

void test_strvec_remove3() {
  string_t *s1 = string_new("FOO");
  string_t *s2 = string_new("BAR");
  string_t *s3 = string_new("BAZ");
  string_t *s4 = string_new("FOOBAR");

  string_vector_t *svec = string_vector_new(s1);
  string_vector_add(svec, s2);
  string_vector_add(svec, s3);
  string_vector_add(svec, s4);

  string_vector_remove(svec,1);

  bool result = (string_vector_len(svec)==3) &&  (string_vector_get(svec, 0) == s1)
    &&  (string_vector_get(svec, 1) == s3)   &&  (string_vector_get(svec, 2) == s4);
  
  verify_bool("string vector remove 3",s1,s2,result);

  free(s3);
  free(s4);
  string_vector_free(svec);
}

/**********************************************************************/


string_t * strtoupper(string_t *str) {
  return string_map(to_upper, str);
}


void test_strvec_map() {
   string_t *s1 = string_new("Hello ");
   string_t *s2 = string_new("World!");
   string_t *r1 = string_new("HELLO ");
   string_t *r2 = string_new("WORLD!");
   string_vector_t *svec = string_vector_new(s1);
   string_vector_add(svec, s2);
   string_vector_t *rvec = string_vector_map(strtoupper, svec);

   bool result = string_equal(rvec->buf[0],r1) && string_equal(rvec->buf[1],r2);
   verify_bool("string vector map",s1,s2,result);
   free(r1);
   free(r2);
   string_vector_free(svec);
   string_vector_deepfree(rvec);
}

/**********************************************************************/

bool strisupper(string_t *str) {
  for(size_t i=0; i < string_len(str); i++) 
    if(!isupper(str->buf[i])) return false;
  
  return true;
}


void test_strvec_filter() {
   string_t *s1 = string_new("Hello");
   string_t *s2 = string_new("WORLD");
   string_vector_t *svec = string_vector_new(s1);
   string_vector_add(svec, s2);
   string_vector_t *rvec = string_vector_filter(strisupper, svec);

   bool result = (string_vector_len(rvec)==1) && string_equal(rvec->buf[0],s2);
   verify_bool("string vector map",s1,s2,result);

   string_vector_free(svec);
   string_vector_deepfree(rvec);
}

/**********************************************************************/  

void test_strvec_split1() {
  string_t *str = string_new("Green,Blue,White,Black,Red,Yellow,Magenta");
  string_vector_t *svec = string_split(str, ';');

  bool result = (string_vector_len(svec)==1) && string_equal(svec->buf[0],str);
  verify_bool("string vector split 1",str, str, result);
  
  string_vector_deepfree(svec);
}


/**********************************************************************/  


void  test_strvec_split2() {
  string_t *str = string_new("Green,Blue,White,Black,Red,Yellow,Magenta");
  string_vector_t *svec = string_vector_empty();
  string_vector_add(svec, string_new("Green"));
  string_vector_add(svec, string_new("Blue"));
  string_vector_add(svec, string_new("White"));
  string_vector_add(svec, string_new("Black"));
  string_vector_add(svec, string_new("Red"));
  string_vector_add(svec, string_new("Yellow"));
  string_vector_add(svec, string_new("Magenta"));
   
  string_vector_t *rvec = string_split(str, ',');
  verify_bool("string vector split 2",str, str, string_vector_equal(svec,rvec));

  string_vector_deepfree(svec);
  string_vector_deepfree(rvec);
}



/**********************************************************************/  

void  test_strvec_split3() {
  string_t *str = string_new("Green,Blue,White,Black,,");
  string_vector_t *svec = string_vector_empty();
  string_vector_add(svec, string_new("Green"));
  string_vector_add(svec, string_new("Blue"));
  string_vector_add(svec, string_new("White"));
  string_vector_add(svec, string_new("Black"));
  string_vector_add(svec, string_new(""));
  string_vector_add(svec, string_new(""));
   
  string_vector_t *rvec = string_split(str, ',');
  verify_bool("string vector split 3",str, str, string_vector_equal(svec,rvec));

  string_vector_deepfree(svec);
  string_vector_deepfree(rvec);
}


void  test_strvec_split4() {
  string_t *str = string_new(",Green,Blue,White,Black,");
  string_vector_t *svec = string_vector_empty();
  string_vector_add(svec, string_new(""));
  string_vector_add(svec, string_new("Green"));
  string_vector_add(svec, string_new("Blue"));
  string_vector_add(svec, string_new("White"));
  string_vector_add(svec, string_new("Black"));
  string_vector_add(svec, string_new(""));
   
  string_vector_t *rvec = string_split(str, ',');
  verify_bool("string vector split 4",str, str, string_vector_equal(svec,rvec));

  string_vector_deepfree(svec);
  string_vector_deepfree(rvec);
}

/**********************************************************************/  

void test_strvec_ssplit1() {
  string_t *str = string_new("THISFOOISFOOAFOOTEST");
  string_t *del = string_new("FOO");
  string_vector_t *svec = string_vector_empty();
  string_vector_add(svec, string_new("THIS"));
  string_vector_add(svec, string_new("IS"));
  string_vector_add(svec, string_new("A"));
  string_vector_add(svec, string_new("TEST"));

  
  string_vector_t *rvec = string_ssplit(str, del);
  verify_bool("string vector ssplit 1",str, del, string_vector_equal(svec,rvec));

  string_vector_deepfree(svec);
  string_vector_deepfree(rvec);
}


/**********************************************************************/  

void test_strvec_ssplit2() {
  string_t *str = string_new("THISFOOISFOOANFOO");
  string_t *del = string_new("FOO");
  string_vector_t *svec = string_vector_empty();
  string_vector_add(svec, string_new("THIS"));
  string_vector_add(svec, string_new("IS"));
  string_vector_add(svec, string_new("AN"));
  string_vector_add(svec, string_new(""));
  
  string_vector_t *rvec = string_ssplit(str, del);

  verify_bool("string vector ssplit 2",str, del, string_vector_equal(svec,rvec));

  string_vector_deepfree(svec);
  string_vector_deepfree(rvec);
}

/**********************************************************************/  

void test_strvec_ssplit3() {
  string_t *str = string_new("I😄am😄happy");
  string_t *del = string_new("😄");
  string_vector_t *svec = string_vector_empty();
  string_vector_add(svec, string_new("I"));
  string_vector_add(svec, string_new("am"));
  string_vector_add(svec, string_new("happy"));

  string_vector_t *rvec = string_ssplit(str, del);
  verify_bool("string vector ssplit 3",str, del, string_vector_equal(svec,rvec));

  string_vector_deepfree(svec);
  string_vector_deepfree(rvec);
}


/**********************************************************************/  

void string_vector_tests() {
  string_t *str = string_colored("String vector tests", CYAN);
  string_println(str);
  free(str);
  
  test_strvec_new();
  test_strvec_add();
  test_strvec_resize();
  test_strvec_find1();
  test_strvec_find2();
  test_strvec_remove1();
  test_strvec_remove2();
  test_strvec_remove3();
  test_strvec_map();
  test_strvec_filter();
  test_strvec_split1();
  test_strvec_split2();
  test_strvec_split3();
  test_strvec_split4();
  test_strvec_ssplit1();
  test_strvec_ssplit2();
  test_strvec_ssplit3();
}

/**********************************************************************/
/**********************************************************************/  

int main() {
  string_tests();
  puts("");
  string_vector_tests();
}
