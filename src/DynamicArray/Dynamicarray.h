#pragma once
#include <iostream>
#include <cstdlib>
using namespace std;

template<typename T>
class DynamicArray {

private:
    T*  arr;
    int n;
    int cap;

    // error function that is used whenever something invalid happens
    void error(const char* msg) const {
        cout << "Error: " << msg << "\n";
        exit(1); //this is prebuilt functions that immediately terminates the program.
    }

    // just asks malloc for a raw block big enough to hold capacity T's
    // no constructors run here — just bytes
    T* allocate(int capacity) {
        T* block = (T*) malloc(capacity * sizeof(T));
        if (!block) error("out of memory");
        return block;
    }

    // builds a T object at this address using placement new, memory is raw before calling this
    void construct(T* ptr, const T& val) {
        new (ptr) T(val);
    }

    // calls T's destructor manually, does not free memory it only cleans up the object
    void destroy(T* ptr) {
        ptr->~T();
    }

    // double the capacity
    // copy all live objects into new block, destroy old ones, free old block
    void resize(int newCap) {
        T* newBlock = allocate(newCap);
        for (int i = 0; i < n; i++) {
            construct(newBlock + i, arr[i]);
            destroy(arr + i);
        }
        free(arr);
        arr = newBlock;
        cap = newCap;
    }

public:

    // start with 4 raw slots, nothing live yet
    DynamicArray() : n(0), cap(4) {
        arr = allocate(cap);
    }

    // destroy all live objects first, then free the block
    ~DynamicArray() {
        for (int i = 0; i < n; i++)
            destroy(arr + i);
        free(arr);
    }

    // fresh block, copy each element in properly
    DynamicArray(const DynamicArray& other) : n(other.n), cap(other.cap) {
        arr = allocate(cap);
        for (int i = 0; i < n; i++)
            construct(arr + i, other.arr[i]);
    }

    // clean up what we have, then copy from other
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;
        for (int i = 0; i < n; i++)
            destroy(arr + i);
        free(arr);
        n   = other.n;  //copy metadata
        cap = other.cap;
        arr = allocate(cap);
        for (int i = 0; i < n; i++)
            construct(arr + i, other.arr[i]);
        return *this;
    }

    int size()     const { return n;   }
    int capacity() const { return cap; }

    // if full, double first then construct the new value at arr[n] which is raw storage
    void append(const T& val) {
        if (n == cap)
            resize(cap * 2);
        construct(arr + n, val);
        n++;
    }

    /// First create an extra element at the end
    // Shift elements to the right so there is an empty spot
    // Insert the new value into that spot
    void insert(int index, const T& val) {
        if (index < 0 || index > n)
            error("insert: index out of range");
        if (n == cap)
            resize(cap * 2);
        construct(arr + n, arr[n - 1]);
        for (int i = n - 1; i > index; i--)
            arr[i] = arr[i - 1];
        arr[index] = val;
        n++;
    }

    // shift everything left using assignment
    // destroy the last slot which is now a duplicate
    void remove(int index) {
        if (index < 0 || index >= n)
            error("remove: index out of range");
        for (int i = index; i < n - 1; i++)
            arr[i] = arr[i + 1];
        destroy(arr + (n - 1));
        n--;
    }

    T& get(int index) {
        if (index < 0 || index >= n)
            error("get: index out of range");
        return arr[index];
    }
    const T& get(int index) const {
        if (index < 0 || index >= n)
            error("get: index out of range");
        return arr[index];
    }

    void print() const {
        cout << "[ ";
        for (int i = 0; i < n; i++)
            cout << arr[i] << " ";
        cout << "]  size=" << n << "  cap=" << cap << "\n";
    }
};