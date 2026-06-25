#include <iostream>
#include <cstdlib>
using namespace std;

template<typename T>
class LinkedList {

private:

    struct Node {
        T     data;
        Node* next;
        Node* prev;   // only wired up when isDoubly = true
    };

    Node* head;
    Node* tail;
    int   n;
    bool  isDoubly;

    void error(const char* msg) const {
        cout << "Error: " << msg << "\n";
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

    // walks to the node just before index
    // used by insert and remove to avoid repeating traversal logic
    Node* nodeBefore(int index) const {
        Node* curr = head;
        for (int i = 0; i < index - 1; i++)
            curr = curr->next;
        return curr;
    }

public:

    // isDoubly = true  → doubly linked, O(1) deleteBack
    // isDoubly = false → singly linked, O(N) deleteBack
    LinkedList(bool doubly = false) : head(nullptr), tail(nullptr), n(0), isDoubly(doubly) {}

    ~LinkedList() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            freeNode(curr);
            curr = next;
        }
    }

    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), n(0), isDoubly(other.isDoubly) {
        Node* curr = other.head;
        while (curr != nullptr) {
            insertBack(curr->data);
            curr = curr->next;
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this;
        Node* curr = head;
        while (curr != nullptr) {
            Node* next = curr->next;
            freeNode(curr);
            curr = next;
        }
        head     = nullptr;
        tail     = nullptr;
        n        = 0;
        isDoubly = other.isDoubly;
        curr     = other.head;
        while (curr != nullptr) {
            insertBack(curr->data);
            curr = curr->next;
        }
        return *this;
    }

    int  size()     const { return n;        }
    bool doubly()   const { return isDoubly; }

    // ── insertFront ───────────────────────────────────────────────────────
    void insertFront(const T& val) {
        Node* node = makeNode(val);
        node->next = head;
        if (isDoubly && head != nullptr)
            head->prev = node;          // old head now points back to new node
        head = node;
        if (n == 0) tail = node;
        n++;
    }

    // ── insertBack ────────────────────────────────────────────────────────
    void insertBack(const T& val) {
        Node* node = makeNode(val);
        if (n == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            if (isDoubly)
                node->prev = tail;      // new node points back to old tail
            tail = node;
        }
        n++;
    }

    // ── insert at index ───────────────────────────────────────────────────
    void insert(int index, const T& val) {
        if (index < 0 || index > n) error("insert: index out of range");
        if (index == 0) { insertFront(val); return; }
        if (index == n) { insertBack(val);  return; }

        Node* prev = nodeBefore(index);
        Node* next = prev->next;
        Node* node = makeNode(val);

        // wire next and prev first, then attach to the list
        node->next = next;
        node->prev = isDoubly ? prev : nullptr;

        prev->next = node;
        if (isDoubly && next != nullptr)
            next->prev = node;          // node after also points back to new node

        n++;
    }

    // ── deleteFront ───────────────────────────────────────────────────────
    void deleteFront() {
        if (n == 0) error("deleteFront: list is empty");
        Node* toFree = head;
        head         = head->next;
        if (isDoubly && head != nullptr)
            head->prev = nullptr;       // new head has no predecessor
        freeNode(toFree);
        toFree = nullptr;
        if (n == 1) tail = nullptr;
        n--;
    }

    // ── deleteBack ────────────────────────────────────────────────────────
    // this is the only method that behaves differently based on the flag
    // doubly → O(1) jump using prev pointer
    // singly → O(N) walk from head to find second-to-last
    void deleteBack() {
        if (n == 0) error("deleteBack: list is empty");
        if (n == 1) {
            freeNode(head);
            head = nullptr;
            tail = nullptr;
            n--;
            return;
        }

        if (isDoubly) {
            // jump straight to second-to-last using prev — O(1)
            Node* toFree = tail;
            tail         = tail->prev;
            tail->next   = nullptr;
            freeNode(toFree);
            toFree = nullptr;
        } else {
            // no prev pointer — walk from head — O(N)
            Node* curr = head;
            while (curr->next != tail)
                curr = curr->next;
            freeNode(tail);
            tail       = curr;
            tail->next = nullptr;
        }
        n--;
    }

    // ── remove at index ───────────────────────────────────────────────────
    void remove(int index) {
        if (index < 0 || index >= n) error("remove: index out of range");
        if (index == 0)     { deleteFront(); return; }
        if (index == n - 1) { deleteBack();  return; }

        Node* prev   = nodeBefore(index);
        Node* toFree = prev->next;
        Node* next   = toFree->next;

        prev->next = next;
        if (isDoubly && next != nullptr)
            next->prev = prev;          // fix back link too

        freeNode(toFree);
        toFree = nullptr;
        n--;
    }

    // ── search ────────────────────────────────────────────────────────────
    bool search(const T& val) const {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == val) return true;
            curr = curr->next;
        }
        return false;
    }

    // ── get ───────────────────────────────────────────────────────────────
    // no random access in linked lists — must walk from head O(N)
    T get(int index) const {
        if (index < 0 || index >= n) error("get: index out of range");
        Node* curr = head;
        for (int i = 0; i < index; i++)
            curr = curr->next;
        return curr->data;
    }

    void print() const {
        Node* curr = head;
        cout << "[ ";
        while (curr != nullptr) {
            cout << curr->data;
            if (curr->next != nullptr) cout << " -> ";
            curr = curr->next;
        }
        cout << " ]  size=" << n << "  " << (isDoubly ? "doubly" : "singly") << "\n";
    }
};


int main() {

    cout << "======= SINGLY LINKED =======\n";
    LinkedList<int> singly(false);
    singly.insertBack(10);
    singly.insertBack(20);
    singly.insertBack(30);
    singly.insertFront(5);
    singly.print();

    singly.insert(2, 99);    singly.print();
    singly.remove(2);        singly.print();
    singly.deleteFront();    singly.print();
    singly.deleteBack();     singly.print();

    cout << "\n======= DOUBLY LINKED =======\n";
    LinkedList<int> doubly(true);
    doubly.insertBack(10);
    doubly.insertBack(20);
    doubly.insertBack(30);
    doubly.insertFront(5);
    doubly.print();

    doubly.insert(2, 99);    doubly.print();
    doubly.remove(2);        doubly.print();
    doubly.deleteFront();    doubly.print();
    doubly.deleteBack();     doubly.print();

    cout << "\n======= EDGE CASES =======\n";
    LinkedList<int> e(true);
    e.insertBack(42);
    e.print();
    e.deleteBack();
    cout << "after deleteBack on single node: size=" << e.size() << "\n";

    cout << "\n======= COPY =======\n";
    LinkedList<int> a(true);
    a.insertBack(1); a.insertBack(2); a.insertBack(3);
    LinkedList<int> b(a);
    b.print();
    LinkedList<int> c(false);
    c = a;
    c.print();

    cout << "\n======= STRING TYPE =======\n";
    LinkedList<string> s(true);
    s.insertBack("hello");
    s.insertBack("world");
    s.insertFront("hey");
    s.print();
    s.remove(1);
    s.print();

    return 0;
}