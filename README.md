# Redes de Computadores [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/urbanogilson/Redes-de-Computadores/blob/master/LICENSE)

Redes de Computadores - Trabalhos Práticos (UFMG)

# Quickstart

## Example

#### Code

```c
// code.c
#include <stdio.h>
int main()
{
  printf("Hello World!\n");
  return 0;
}
```
#### Makefile

```Makefile
# Makefile
all: hello
hello: code.o
  gcc code.o -o hello
code.o:
  gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g code.c
clean:
  rm -rf *o hello
```

#### Compile

    $ make
    
#### Run

    $ ./hello
    Hello World!


## License

The contents of this repository are covered under the [MIT License](https://github.com/urbanogilson/Redes-de-Computadores/blob/master/LICENSE).
