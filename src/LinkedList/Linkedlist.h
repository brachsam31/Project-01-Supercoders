#pragma once
#include <iostream>
#include <cstdlib>
using namespace std;

// base class — just the contract, no implementation
// every derived class must implement all of these
template<typename T>
class LinkedList {
public:
    // pure virtual functions, these forces every derived class to implement these functions. 
    virtual void insertFront(const T& val)       = 0;
    virtual void insertBack(const T& val)        = 0;
    virtual void deleteFront()                   = 0;
    virtual void deleteBack()                    = 0;
    virtual void insert(int index, const T& val) = 0;
    virtual void remove(int index)               = 0;
    virtual bool search(const T& val)    const   = 0;
    virtual T    get(int index)          const   = 0;
    virtual int  size()                  const   = 0;
    virtual void print()                 const   = 0;

    // without this, deleting via base pointer won't call derived destructor
    virtual ~LinkedList() {}
};


// one pointer per node — less memory, but deleteBack is O(N)
template<typename T>
class SinglyLinkedList : public LinkedList<T> {

public:
    struct Node {
        T     data;
        Node* next;
    };

private:
    Node* head;
    Node* tail;
    int   n;

    void error(const char* msg) const {
        cout << "SinglyList: " << msg << "\n";
        exit(1);
    }

    Node* makeNode(const T& val) {
        Node* node = (Node*) malloc(sizeof(Node));
        if (!node) error("out of memory");
        new (&node->data) T(val);
        node->next = nullptr;
        return node;
    }

    void freeNode(Node* node) {
        node->data.~T();
        free(node);
    }

    // walks to the node just before index
    // used by insert and remove to avoid repeating the same loop
    Node* nodeBefore(int index) const {
        Node* curr = head;
        for (int i = 0; i < index - 1; i++)
            curr = curr->next;
        return curr;
    }

public:

    SinglyLinkedList() : head(nullptr), tail(nullptr), n(0) {}

    ~SinglyLinkedList() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;   // save next before freeing
            freeNode(curr);
            curr = next;
        }
    }

    SinglyLinkedList(const SinglyLinkedList& other) : head(nullptr), tail(nullptr), n(0) {
        Node* curr = other.head;
        while (curr != nullptr) {
            insertBack(curr->data);
            curr = curr->next;
        }
    }

    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this == &other) return *this;
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            freeNode(curr);
            curr = next;
        }
        head = nullptr; tail = nullptr; n = 0;
        curr = other.head;
        while (curr != nullptr) {
            insertBack(curr->data);
            curr = curr->next;
        }
        return *this;
    }

    int   size()    const override { return n;    }
    Node* getHead() const          { return head; }

    void insertFront(const T& val) override {
        Node* node = makeNode(val);
        node->next = head;
        head       = node;
        if (n == 0) tail = node;
        n++;
    }

    // O(1) only because we keep a tail pointer
    void insertBack(const T& val) override {
        Node* node = makeNode(val);
        if (n == 0) { head = node; tail = node; }
        else        { tail->next = node; tail = node; }
        n++;
    }

    // save the link before rewiring — do it the other way and the rest is lost
    void insert(int index, const T& val) override {
        if (index < 0 || index > n) error("insert: index out of range");
        if (index == 0) { insertFront(val); return; }
        if (index == n) { insertBack(val);  return; }
        Node* prev = nodeBefore(index);
        Node* node = makeNode(val);
        node->next = prev->next;
        prev->next = node;
        n++;
    }

    void deleteFront() override {
        if (n == 0) error("deleteFront: list is empty");
        Node* toFree = head;
        head         = head->next;
        freeNode(toFree);
        toFree = nullptr;
        if (n == 1) tail = nullptr;
        n--;
    }

    // no prev pointer so we walk from head — this is the O(N) cost of singly
    void deleteBack() override {
        if (n == 0) error("deleteBack: list is empty");
        if (n == 1) {
            freeNode(head);
            head = nullptr; tail = nullptr;
            n--; return;
        }
        Node* curr = head;
        while (curr->next != tail)
            curr = curr->next;
        freeNode(tail);
        tail       = curr;
        tail->next = nullptr;
        n--;
    }

    void remove(int index) override {
        if (index < 0 || index >= n) error("remove: index out of range");
        if (index == 0)     { deleteFront(); return; }
        if (index == n - 1) { deleteBack();  return; }
        Node* prev   = nodeBefore(index);
        Node* toFree = prev->next;
        prev->next   = toFree->next;
        freeNode(toFree);
        toFree = nullptr;
        n--;
    }

    bool search(const T& val) const override {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == val) return true;
            curr = curr->next;
        }
        return false;
    }

    // no random access — must walk from head every time
    T get(int index) const override {
        if (index < 0 || index >= n) error("get: index out of range");
        Node* curr = head;
        for (int i = 0; i < index; i++)
            curr = curr->next;
        return curr->data;
    }

    // used by HashMap to remove a specific key from a chain
    bool removeVal(const T& val) {
        Node* curr = head;
        Node* prev = nullptr;
        while (curr != nullptr) {
            if (curr->data == val) {
                if (prev) prev->next = curr->next;
                else      head       = curr->next;
                if (curr == tail) tail = prev;
                freeNode(curr);
                n--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    void clear() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            freeNode(curr);
            curr = next;
        }
        head = nullptr; tail = nullptr; n = 0;
    }

    void print() const override {
        Node* curr = head;
        cout << "[ ";
        while (curr != nullptr) {
            cout << curr->data;
            if (curr->next != nullptr) cout << " -> ";
            curr = curr->next;
        }
        cout << " ]  size=" << n << "  singly\n";
    }
};


// two pointers per node — more memory, but deleteBack is O(1)
template<typename T>
class DoublyLinkedList : public LinkedList<T> {

public:
    struct Node {
        T     data;
        Node* next;
        Node* prev;
    };

private:
    Node* head;
    Node* tail;
    int   n;

    void error(const char* msg) const {
        cout << "DoublyList: " << msg << "\n";
        exit(1);
    }

    Node* makeNode(const T& val) {
        Node* node = (Node*) malloc(sizeof(Node));
        if (!node) error("out of memory");
        new (&node->data) T(val);
        node->next = nullptr;
        node->prev = nullptr;
        return node;
    }

    void freeNode(Node* node) {
        node->data.~T();
        free(node);
    }

    Node* nodeBefore(int index) const {
        Node* curr = head;
        for (int i = 0; i < index - 1; i++)
            curr = curr->next;
        return curr;
    }

public:

    DoublyLinkedList() : head(nullptr), tail(nullptr), n(0) {}

    ~DoublyLinkedList() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            freeNode(curr);
            curr = next;
        }
    }

    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), n(0) {
        Node* curr = other.head;
        while (curr != nullptr) {
            insertBack(curr->data);
            curr = curr->next;
        }
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this == &other) return *this;
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            freeNode(curr);
            curr = next;
        }
        head = nullptr; tail = nullptr; n = 0;
        curr = other.head;
        while (curr != nullptr) {
            insertBack(curr->data);
            curr = curr->next;
        }
        return *this;
    }

    int   size()    const override { return n;    }
    Node* getHead() const          { return head; }

    void insertFront(const T& val) override {
        Node* node = makeNode(val);
        node->next = head;
        if (head != nullptr) head->prev = node;
        head = node;
        if (n == 0) tail = node;
        n++;
    }

    void insertBack(const T& val) override {
        Node* node = makeNode(val);
        if (n == 0) { head = node; tail = node; }
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        n++;
    }

    void insert(int index, const T& val) override {
        if (index < 0 || index > n) error("insert: index out of range");
        if (index == 0) { insertFront(val); return; }
        if (index == n) { insertBack(val);  return; }
        Node* prev = nodeBefore(index);
        Node* next = prev->next;
        Node* node = makeNode(val);
        node->next = next;
        node->prev = prev;
        prev->next = node;
        if (next != nullptr) next->prev = node;
        n++;
    }

    void deleteFront() override {
        if (n == 0) error("deleteFront: list is empty");
        Node* toFree = head;
        head         = head->next;
        if (head != nullptr) head->prev = nullptr;
        freeNode(toFree);
        toFree = nullptr;
        if (n == 1) tail = nullptr;
        n--;
    }

    // O(1) — just jump to tail->prev, no walking needed
    // this is exactly why doubly linked exists
    void deleteBack() override {
        if (n == 0) error("deleteBack: list is empty");
        if (n == 1) {
            freeNode(head);
            head = nullptr; tail = nullptr;
            n--; return;
        }
        Node* toFree = tail;
        tail         = tail->prev;
        tail->next   = nullptr;
        freeNode(toFree);
        toFree = nullptr;
        n--;
    }

    void remove(int index) override {
        if (index < 0 || index >= n) error("remove: index out of range");
        if (index == 0)     { deleteFront(); return; }
        if (index == n - 1) { deleteBack();  return; }
        Node* prev   = nodeBefore(index);
        Node* toFree = prev->next;
        Node* next   = toFree->next;
        prev->next = next;
        if (next != nullptr) next->prev = prev;
        freeNode(toFree);
        toFree = nullptr;
        n--;
    }

    bool search(const T& val) const override {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == val) return true;
            curr = curr->next;
        }
        return false;
    }

    T get(int index) const override {
        if (index < 0 || index >= n) error("get: index out of range");
        Node* curr = head;
        for (int i = 0; i < index; i++)
            curr = curr->next;
        return curr->data;
    }

    bool removeVal(const T& val) {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == val) {
                if (curr->prev) curr->prev->next = curr->next;
                else            head             = curr->next;
                if (curr->next) curr->next->prev = curr->prev;
                else            tail             = curr->prev;
                freeNode(curr);
                n--;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    void clear() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            freeNode(curr);
            curr = next;
        }
        head = nullptr; tail = nullptr; n = 0;
    }

    void print() const override {
        Node* curr = head;
        cout << "[ ";
        while (curr != nullptr) {
            cout << curr->data;
            if (curr->next != nullptr) cout << " -> ";
            curr = curr->next;
        }
        cout << " ]  size=" << n << "  doubly\n";
    }
};