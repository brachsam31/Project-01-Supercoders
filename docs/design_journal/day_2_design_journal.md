# Day 2 Design Journal 
## June 23, 2026
### Section 1 - Specific Bug
#### Runtime Output 
While testing the alloctor style DynamicArray, the output was: 
[10 20 30 40] size=4 cap=64 
[10 20 30 40 50] size=5 cap=128 
--- remove --- 
[10 20 77 30 40 50 55 55] size=8 cap=128 
Problems observed in this was that capacity jumped directly from 4 to 64.
and the array resized on every append instead of only when full and removed elements still appeared because the logical size was not updated correctly.

### Section 2 - Failed Attempt 
After observing that manual memory management will be better in terms of learning if I want to study memory management better. Initially I believed that calling malloc() immediately created objects in memory similar to new(). This misunderstanding caused confusion while implementing insertion and copying.

### Section 3 - Memory Diagram
### Section 4 - Learning Reflection
Today my understanding of memory management changed significantly. I learned that allocating memory and constructing objects are two separate operations. malloc() only reserves raw memory, while placement new creates actual objects inside that memory. I also understood why STL containers manually manage object lifetime using placement new, explicit destructor calls, and free(). This approach gives much greater control over memory and helped me understand how std::vector works internally.