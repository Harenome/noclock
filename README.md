No Clock
========

[Assignement](http://icps.u-strasbg.fr/~violard/TER_2014-2015/TER_2014_2015_elim_clocks.pdf).

Requirements
------------

The following tools are required to build this program:

- `C99` development tools and the GNU libc.
- `lex >= 2.5.37` or flex
- `yacc >= 1.9 20130925` or `GNU Bison >= 3.0.2`
- `ISL >= 0.14` with `GMP`

### Ubuntu LTS (10.04, 12.04, 14.04)

~~~{.bash}
$ apt-get install build-essential
$ apt-get install ???
~~~

### Fedora 21

~~~{.bash}
$ yum -y groupinstall "Development Tools"
$ yum -y install byacc bison bison-devel flex flex-devel
$ yum -y install isl isl-devel
~~~

### Tested on

This program has been tested on the following setups:

OS           | C Compiler  | Yacc            | Lex    | ISL
-------------|-------------|-----------------|--------|-----------
Fedora 21    | gcc 4.9.2   | yacc 1.9        | 2.5.37 | 0.14 GMP
Fedora 21    | gcc 4.9.2   | GNU Bison 3.0.2 | 2.5.37 | 0.14 GMP
Ubuntu 10.04 | ???         | ???             | ???    | ???
Ubuntu 12.04 | ???         | ???             | ???    | ???
Ubuntu 14.04 | ???         | ???             | ???    | ???

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

The following command will install the project, if root privileges are
available:

~~~{.bash}
$ make install
~~~

To specify another location:

~~~{.bash}
$ make install PREFIX=$HOME/local
~~~

Examples
--------

~~~{.bash}
$ build/bin/noclock examples/E1.x10p2
~~~

Note that `examples/E10.x10p2` does not work.

License
-------

This work is licensed under the MIT License. See the `COPYING` file for further
information.
