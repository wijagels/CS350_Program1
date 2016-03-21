# Lab 5
## How to use
``./lab5 input_file.txt``

Or, you can pipe the input in instead

``./lab4 | ./lab5``

## What it does
This currently runs a few algorithms on the stream of input references.
When a page is referenced, the system looks for it in the main memory table, then swap.
If it's in swap, it will be switched into main memory, and a fault will be generated.

## How to implement page replacement algorithms
Adding new algorithms is pretty easy, just implement a few different methods in your class.
``void add_hook(std::vector<Page>::iterator);``
``void access_hook(std::vector<Page>::iterator);``
``std::vector<Page>::iterator select_victim();``
Within pages, your algorithm can store an arbitrary number of attributes as ``uint64_t`` in ``Page::attrs``.
For example, Lfu stores a counter in every page at the head of the attributes list, and selects victims by choosing the minimum counter.
The constructor should take a single argument, of type ``PageTable<YourAlgorithmClass>*``.
It would be a good idea to store this argument, because it will not be given to you later.
The reason it's not a constant reference is becaues we want to be able to get normal iterators out of it in the ``select_victim()`` function.
