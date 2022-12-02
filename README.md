# BuzzDB Snippets

Same setup instructions as BuzzDB

Dependencies

```
apt install bison flex
```

# Linear Hashing

You can build and run the test suite with the following commands:
- `cd build`
- `cmake -DCMAKE_BUILD_TYPE=Debug ..`
- `make check`
- `ctest -R linear_hashing_test -V`

Files:
- `src/include/utils/linear_hashing.h`
- `src/utils/linear_hashing.cc`
- `test/unit/utils/linear_hashing_test.cc`

## Architecture
I chose to implement tombstones for the deletion logic without replacing the tombstoned entries. In a real implementation, this would be acceptable for a hashtable with a high base number of entries since the capacity would slowly shrink. As the hashtable implementation focus is on minimalism, I decided not to implement the logic for cleaning up the tombstones.

## HashTable
The hashtable has a backing vector of Entries. Entries contain a key, value, tombstone boolean, and full boolean. The tombstone boolean is flagged when the key is deleted and the full boolean is flagged when the spot in the vector is filled by an insert.

## Lookup
The lookup algorithm returns the value associated with the key. If the key is not found, it returns -1.

## Insert
The key value pair is inserted into the backing table.
- If another entry with the same key is already in the table, it is replaced with the new value.
- If a tombstone with the same key is in the table, the tombstone entry is revived with the new value.
- If the table is full, no entry is inserted.

## Erase
The entry associated with the key is flagged as a tombstone and the value at the location is returned. If the key does not exist in the table, the function returns -1.

# Tests
I included 15 tests which cover almost all of the possibilities that the table could encounter.

General:
- InitTable

For insert:
- InsertOne
- InsertFive
- InsertFull
- InsertOverflow
- InsertReplace
- InsertFiveCollision
- InsertThreeOneCollision

For erase:
- EraseOne
- EraseAll
- EraseThreeOneCollision
- EraseKeyNotFound

For lookup:
- LookupOne
- LookupEraseThreeOneCollision
- LookupKeyNotFound