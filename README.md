Opts
==============================================

    Created By: Michael D. Lowis
    Email:      mike@mdlowis.com

About This Project
----------------------------------------------
This project is an implementation of a very lightweight command-line options
parser. It is capable of interpreting the Unix/GNU style command line
argument flags. It is designed to be platform independent and can be used as
a static library or directly as source.

Design
----------------------------------------------
The library is designed to work like something of a database for your
command line. At initialization time you feed in the arguments vector (your
data records) and a list of option definitions (your schema). The library
then parses the options and stores them internally for querying later.
Several querying functions are provided that cover the most common use cases
for options handling.

All of the query functions take an option name and an option tag as
parameters. These two arguments will be used to search the list of parsed
options for any that match both parameters. In some cases only one of the
parameters is needed for a query. To facilitate this, the query functions
will interpret a NULL pointer to match everything. An example of this would
be selecting all parsed options with the tag "foo":

    const char** foo_lst = opts_select(NULL,"foo");

In the example above, the NULL would match *all* parsed options and from that
group only those having the tag "foo" would be returned. Another common
scenario would be searching by parameter name:

    const char** includes = opts_select("I", NULL);

In this scenario all of the options that were parsed having the name 'I'
would be returned regardless of their tag value. This would equate to all of
the instances of '-I' that would occur on the command line for an invocation
of gcc.

With this design you should be able to let the library handle your options
parsing while you focus on your application logic, using appropriate queries
to change behavior where necessary.

License
----------------------------------------------
Unless explicitly stated otherwise, all code and documentation contained within
this repository is released under the BSD 2-Clause license. The text for this
license can be found in the LICENSE.md file.

Requirements For Building
----------------------------------------------
This project requires the following external tools to build successfully:

* A POSIX compliant 'make' utility
* A C99 capable C compiler

Build Instructions
----------------------------------------------
This project can be built by executing the following command:

    make

You should be left with a static library that you can use as you please.
