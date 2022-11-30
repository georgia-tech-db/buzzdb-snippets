# BuzzDB Snippets
We create a hash_function that will calculate the hash based on the size of the storage. Then, in the main function, we input all the values that need to be stored as a vector of values and input the size of the storage. We return a vector of pairs with the specific size in which the first value is the value itself and second value is the offset. The vactor of pairs are initialized to be all -1.

In the test file, we create a test vector that has values of [2, 0, 9, 10, 16, 5] and the size of the storage to be 7 as the example shown in the class lecture slides. The correct output after our method should be [(0,0), (-1,1), (2,0), (9,1), (16,2),(10,2),(5,1)].
Same setup instructions as BuzzDB

Dependencies

```
apt install bison flex
```
