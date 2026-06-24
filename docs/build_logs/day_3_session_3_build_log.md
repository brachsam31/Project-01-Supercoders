Date: June 24

Duration:
2 Hours (2:00 PM – 4:00 PM)

Goal:
Continue LinkedList implementation by adding search and deletion functionality.

Problem Encountered:
While implementing deleteFront(), I initially forgot to handle the case where the list contains only one node. After deleting the node, the head pointer was updated correctly, but the tail pointer still pointed to the deleted node.

What I Tried:
Used print statements and small test cases to track the values of head and tail after deletion.

Implemented:

* search()
* deleteFront()

Tested the implementation with:

* Empty list
* Single-node list
* Multiple-node list

Verified that search works correctly for both existing and missing values.

Outcome:
Successfully implemented search() and deleteFront().

Verified that:

* search() traverses the list correctly.
* deleteFront() updates the head pointer properly.
* Empty-list and single-node edge cases are handled correctly.
* Tail is reset correctly when the last node is removed.

Improved my understanding of pointer updates during node deletion.

Next Step:
Implement deleteBack(), insert(index), and the remaining LinkedList operations.