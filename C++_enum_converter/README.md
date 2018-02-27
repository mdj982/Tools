# USAGE

    $ make
    ...
    $ ./es_converter
    Put any definition of enum type or vector<string> type:
    enum T_t { AA, BB, CC };

    vector<string> str_of_T_t = {
        "AA", "BB", "CC"
    };

    enum fruits_t {
         APPLE, BANANA, CHERRY
    };

    Put any definition of enum type or vector<string> type:
    ^C

## SPEC

Program is able to convert both enum type and vector<string> type in c++.

Program is confirmed to run correctly when compilable syntax (without using varibale) is put.
