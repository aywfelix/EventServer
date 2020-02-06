<!-- -*- mode: markdown -*-  -->

Building MariaCpp library
---------------------------

**MariaCpp** is compiled using _cmake_, so you can follow instructions
from _MariaDB_ or _MariaDB Connector/C_ build docs.

Shortly:

    cmake .
    cmake . -LH
    make
    make test
    sudo make install



Testing MariaCpp library
------------------------

Command:

    make test

will execute some automatic tests.
These tests require working connection to MariaDB/MySQL database with
permission to create temporary table.
By default, it will connect with following credentials:

* host: localhost
* user: test
* password:
* schema: test

You can modify it by changing cmake variables:
`TEST_DB_URI`, `TEST_DB_USER`, `TEST_DB_PASSWD`.


