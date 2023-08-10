# libstring
Libstring is an almost lightweight C library with the goal of enabling
you to perform simple string operations without shooting yourself in
the foot. Here are some features and reasons supporting this claim:

1. Any string struct is treated as an immutable object. Consequently,
   any return value that is a reference points to a freshly allocated
   heap object which must be freed after usage.

2. New string objects should be created using the pseudo constructors
   `string_colored()`, `string_new()`, and `string_read()`.

3. There are no NULL references, only empty strings, which makes it
   more difficult to trigger a segfault.

4. This library supports basic `map()` and `filter()` functions to
   facilitate the implementation of string manipulation functions such
   as `to_upper()`.

5. This library also includes the `string_split()` function, which
   divides a string into a vector of strings, thereby supporting
   vectors of strings."


## Documentation
Work in progress

## Dependencies
* gcc     (http://gcc.gnu.org/) 
* make    (http://www.gnu.org/software/make/)
