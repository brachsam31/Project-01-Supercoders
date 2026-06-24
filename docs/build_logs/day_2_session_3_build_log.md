Date: June 23

Duration:
2 Hours (4:30 PM – 6:30 PM)

Goal:
Start LinkedList implementation by understanding its memory layout and implementing the first core functions.

Problem Encountered:
Coming from DynamicArray, I was used to storing elements in contiguous memory. In LinkedList, every node is allocated separately on the heap and connected using pointers, which made it harder to visualize how the structure grows.

What I Tried:
Started by designing the Node structure.

Studied:

* data field
* next pointer
* head pointer
* tail pointer

Drew memory diagrams to understand how nodes are connected.

Implemented:

* LinkedList constructor
* insertBack()

Used print statements to verify that new nodes were being linked correctly and that the tail pointer was updating properly after each insertion.

Outcome:
Successfully started the LinkedList implementation.

Completed:

* Node structure
* LinkedList constructor
* insertBack()

Improved my understanding of:

* Heap allocation for individual nodes
* Pointer-based connections between nodes
* Difference between DynamicArray and LinkedList memory layouts
* Role of head and tail pointers

Next Step:
Implement insertFront(), traversal, and basic search functionality.