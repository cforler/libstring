# libstring: A Lightweight C String Library
`libstring` is a lightweight C library designed to simplify common
string operations while ensuring memory safety and preventing common
pitfalls. It aims to provide an intuitive and safe interface for
string manipulation, making it easier to work with strings in C
programs.


## Features

- **Immutability**: All string structures are treated as immutable
  objects. Any return value that is a reference points to a freshly
  allocated heap object, which must be freed after usage.

- **Safe Memory Handling**: Most of the memory allocation and deallocation are
  handled internally, reducing the risk of memory leaks and dangling
  pointers.

- **Error Handling**: In case of an error, functions return NULL or
  other appropriate error indicators.

- **Functional Programming Support**: `libstring` includes basic
  `map()` and `filter()` functions, enabling the implementation of
  string manipulation functions in a functional programming style.

- **String Vectors**: Support for dynamic arrays of strings, allowing
  easy manipulation of collections of strings.

## Installation

To use `libstring` in your project, follow these steps:

1. Clone the repository: `git clone https://github.com/your-username/libstring.git`
2. Navigate to the repository directory: `cd libstring`
3. Build the shared library `libstring.so`
   ```bash
   make

4. Install the library and header files:

  ```bash
   sudo make install
   ```
This will copy libstring.so to /usr/lib and libstring.h to /usr/include.


## Misc

- You can also generate HTML documentation using Doxygen:

   ```bash
   make html
   ```
The HTML documentation will be generated in the `html` directory.

- Uninstall the library:

```bash
   sudo make uninstall
```
This will remove the installed `libstring.so` and `libstring.h` files.


## Usage

Here's a quick example of how to use `libstring` after the installation:

```c
#include <stdlib.h>
#include <libstring.h>
 
int main() {
    string_t *str = string_new("Hello world!");

    string_println(str);
    free(str);
}
```

## Dependencies
* gcc     (http://gcc.gnu.org/) 
* make    (http://www.gnu.org/software/make/)


## License
This project is licensed under the MIT License - see the LICENSE file
for details.


## Acknowledgments
This library features an intuitive API, as its documentation was
primarily composed by chatGPT 3.5. Therefore, I extend my gratitude to
chatGPT from OpenAI for its contribution.

