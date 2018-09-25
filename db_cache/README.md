DBCache kata complete with following
====================================


DB backend - sqlite3 in Serialized multi-threading mode
(see https://www.sqlite.org/threadsafe.html)

Data generated - 20 predefined varchar strings as keys
                 100K random short strings (10 - 30 symbols) as data

Performance tested - caching improves performance by 2 orders of magnitude

3rd party libs used - Boost (for shared_mutex, deadline_timer, io_service, split, bind)
                      sqlite3 lib

synchronization in cache - kata directives achieved through combination of:
                           1-per-record  timed_mutex   with timeout (1s)      to allow per-record sync granularity
                           1-per-table   shared_mutex  with upgradeable lock

final test - 1145s took direct access to sqlite3 DB
             11s   took cached access to sqlite3 DB