No Clock
========

[Assignement](http://icps.u-strasbg.fr/~violard/TER_2014-2015/TER_2014_2015_elim_clocks.pdf).

Fetch this project:

~~~{.bash}
$ git clone https://github.com/Harenome/noclock.git
$ cd noclock
~~~

Build requirements
------------------

The following tools are required to build this program:

- `C99` development tools and the GNU libc.
- `lex`/`flex` >= `2.5.37`
- `yacc`/`byacc` >= `1.9 20130925` or `GNU Bison` >= `3.0.2`
- `libgmp` >= `5.0.2`
- `libisl` >= `0.14`

The following tools are optionnal:

- `doxygen`
- `graphviz`/`dot`

### Ubuntu LTS (10.04, 12.04, 14.04)

#### Ubuntu 12.04 or 14.04

Install the required tools:

~~~{.bash}
$ apt-get install build-essential
$ apt-get install bison byacc flex

On Ubuntu 12.04:

~~~{.bash}
$ apt-get install libgmp10 libgmp-dev-lts
~~~

On Ubuntu 14.04:

~~~{.bash}
$ apt-get install libgmp10 libgmp-dev
~~~


Install the optionnal tools:

~~~{.bash}
$ apt-get install doxygen graphviz
~~~

Install ISL 0.14:

~~~{.bash}
$ wget http://isl.gforge.inria.fr/isl-0.14.tar.gz
$ tar -xf isl-0.14.tar.gz
$ cd isl-0.14.tar.gz
$ ./configure PREFIX=/usr/local
$ make
$ sudo make install
$ sudo ldconfig
~~~

If you do not wish to install ISL into a "standard" location, or do not have
sufficient privileges, do not forget to set `LDLIBS` when compiling `noclock`
and to set `LD_LIBRARY_PATH` when running `noclock`.

For instance:

~~~{.bash}
$ make LDLIBS=-L$HOME/lib
$ LD_LIBRARY_PATH=$HOME/lib build/bin/noclock
~~~

### Fedora 21

Install the required tools:

~~~{.bash}
$ yum groupinstall "Development Tools"
$ yum install byacc bison bison-devel flex flex-devel
$ yum install isl isl-devel
~~~

Install the optionnal tools:

~~~{.bash}
$ yum install doxygen graphviz
~~~

### Tested on

Building has been tested on the following setups:

OS           | C Compiler  | Yacc            | Lex    | ISL  | GMP
-------------|-------------|-----------------|--------|------|-------
Fedora 21    | gcc 4.9.2   | yacc 1.9        | 2.5.37 | 0.14 | 6.0.0
Fedora 21    | clang 3.5.0 | yacc 1.9        | 2.5.37 | 0.14 | 6.0.0
Fedora 21    | gcc 4.9.2   | GNU Bison 3.0.2 | 2.5.37 | 0.14 | 6.0.0
Ubuntu 12.04 | gcc 4.6.3   | yacc 1.9        | 2.5.35 | 0.14 | 5.0.2
Ubuntu 12.04 | gcc 4.6.3   | GNU Bison 2.5   | 2.5.35 | 0.14 | 5.0.2
Ubuntu 14.04 | gcc 4.8.2   | yacc 1.9        | 2.5.35 | 0.14 | 5.0.2
Ubuntu 14.04 | gcc 4.8.2   | GNU Bison 3.0.2 | 2.5.35 | 0.14 | 5.1.3

Building
--------

### Default

~~~{.bash}
$ make
~~~

The binary will be located in `build/bin`.

### Special targets

One can explicitly compile the project in debug or in release mode:

~~~{.bash}
$ make debug
$ make release
~~~

### Compiling tools

By default, the `Makefile` uses `gcc`, `lex` and `yacc`. This can be
overriden using the default GNU make variables. For instance:

~~~{.bash}
$ make CC=clang
$ make CC=clang YACC=bison LEX=flex
~~~

Installation
------------

Once building is successful, the following command will install the project,
if root privileges are available (or at least if the current user is allowed
to write to `/usr/local`):

~~~{.bash}
$ make install
~~~

To specify another location:

~~~{.bash}
$ make install PREFIX=$HOME/local
~~~

Documentation
-------------

### User documentation

A manpage is provided. If the program has been installed via `make install`,
the manpage should most likely available via:

~~~{.bash}
$ man noclock
~~~

Otherwise, it can be found in the `man/man1`:

~~~{.bash}
$ man man/man1/noclock.1
~~~

Help can also be printed via the program using the `--help` or `-h` options.

### Developper documentation

Developper documentation is available via the use of `Doxygen`. The following
commands can be used to generate the documentation:

~~~{.bash}
$ make doc
$ doxygen
~~~

The generated documentation shall be located in `build/doc`.

Usage and Examples
------------------

Some example programs are located in `examples/`, but please note that
`examples/E10.x10p2` does not work.

### Basic usage

If the program has been install via `make install`, it should be available in
`$PATH`:

~~~{.bash}
$ noclock <input_file> [output_file]
~~~

Otherwise, the binary is located in `build/bin`:

~~~{.bash}
$ build/bin/noclock <input_file> [output_file]
~~~

### Examples

To remove clocks from an X10 program (for instance `examples/E1.x10p2`):

~~~{.bash}
$ build/bin/noclock examples/E1.x10p2
$ build/bin/noclock --verbose examples/E1.x10p2
~~~

To store the result in another file:

~~~{.bash}
$ build/bin/noclock examples/E1.x10p2 output
~~~

Input and output files can also be specified using `-i` and `-o` (or `--input`
and `--output`):

~~~{.bash}
$ build/bin/noclock -o output -i examples/E1.x10p2
~~~

If life is sad, colours can be disabled:

~~~{.bash}
$ build/bin/noclock --no-colours examples/E1.x10p2
~~~

License
-------

Copyright (c) 2015 Harenome RAZANAJATO RANAIVOARIVONY

This work is licensed under the MIT License. See the `COPYING` file for further
information.
