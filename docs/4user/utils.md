## Utilities

### Allocator

`Allocator` is Interface for continuous memory allocation algorithms. Each allocation algorithm is a derived class of `class Allocator`, which implements two virtual functions.

1. `size_t malloc(size_t len)`: allocate continuous memory of length `@len`, return the start of it.
2. `void free(size_t ptr)`: free continuous memory starting from `@ptr`. 

`FFMA` is a simple first fit memory allocator, it maintains a list of free memory segments. When it needs to allocate continuous memory, the first segment that is large enough will be selected. 