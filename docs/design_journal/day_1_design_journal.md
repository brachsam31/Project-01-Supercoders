# Day 1 Design Journal 
## June 22, 2026
### Section 1 - Specific Bug
#### Runtime Output 
while testing menu-driven DynamicArray program, the application entered an infinite loop when multiple append operations were performen consecutively. The program repeatedly displayed the menu without allowing normal execution of the remaining operation. Also in this menu driven approch we used new() and delete() keyword which didn't helped much in understanding memory management. 

### Section 2 - Failed Attempt 
Today I implemented the Dynamic Array using new[] and delete[] for memory management, Initially I believed the infinite loop was caused by the append() or resize() implementation because the issue appeared while inserting multiple elements at once. By adding print statements and tracing the execution flow, I discovered that the menu loop and input handling logic were incorrect, causing the program to repeatedly execute the same section.

### Section 3 - Memory Diagram
### Section 4 - Learning Reflection
Today I learned how a Dynamic Array can be implemented using new[] and delete[] for dynamic memory allocation. I realized that not every runtime problem comes from the data structure itself; sometimes the issue is in the surrounding program logic, such as the menu loop or input handling.