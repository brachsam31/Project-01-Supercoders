# Day 2 Design Journal 
## June 24, 2026
### Section 1 - Specific Bug
During LinkedList implementation, inserting a node at the front sometimes disconnected the remaining list because the pointers were updated in the wrong order.

### Section 2 - Failed Attempt 
Initially, I modified the head pointer before reconnecting the existing nodes. This broke the links between nodes. Similarly, during deletion I updated only the head pointer and forgot that, in a single-node list, both head and tail refer to the same node. This created a dangling pointer. To solve these issues, I drew pointer diagrams on paper and tested the implementation using empty lists, single-node lists, and multiple-node lists.

### Section 3 - Memory Diagram
### Section 4 - Learning Reflection
Today I realized that implementing a LinkedList is mainly about maintaining correct pointer relationships rather than writing complex algorithms. A small mistake in pointer order can disconnect the entire list or leave dangling pointers. I also learned the importance of testing edge cases such as empty lists and single-node lists because these cases often expose pointer-related bugs.