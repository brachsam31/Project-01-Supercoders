#include <iostream>
#include "Dynamicarray.h"
using namespace std;

int main() {

    // ── append ────────────────────────────────────────────────────────────
    cout << "=== append ===\n";
    DynamicArray<int> a;
    a.append(10); a.append(20); a.append(30); a.append(40);
    a.print();          // cap=4, full
    a.append(50);       // triggers resize cap 4→8
    a.print();

    // ── insert ────────────────────────────────────────────────────────────
    cout << "\n=== insert ===\n";
    a.insert(0, 99);            a.print();   // front
    a.insert(3, 77);            a.print();   // middle
    a.insert(a.size(), 55);     a.print();   // end — same as append

    // ── remove ────────────────────────────────────────────────────────────
    cout << "\n=== remove ===\n";
    a.remove(0);                a.print();   // front
    a.remove(2);                a.print();   // middle
    a.remove(a.size() - 1);     a.print();   // last

    // ── get ───────────────────────────────────────────────────────────────
    cout << "\n=== get ===\n";
    cout << "get(0) = " << a.get(0) << "\n";
    cout << "get(2) = " << a.get(2) << "\n";

    // ── size and capacity ─────────────────────────────────────────────────
    cout << "\n=== size and capacity ===\n";
    cout << "size=" << a.size() << "  cap=" << a.capacity() << "\n";

    // ── resize trigger — watch capacity double at each threshold ──────────
    cout << "\n=== resize trigger ===\n";
    DynamicArray<int> b;
    cout << "start: cap=" << b.capacity() << "\n";
    for (int i = 1; i <= 10; i++) {
        b.append(i * 10);
        cout << "append(" << i*10 << ")  size=" << b.size() << "  cap=" << b.capacity() << "\n";
    }

    // ── copy constructor — modifying copy should not touch original ────────
    cout << "\n=== copy constructor ===\n";
    DynamicArray<int> c(b);
    c.append(999);
    cout << "original: "; b.print();
    cout << "copy:     "; c.print();

    // ── copy assignment — same independence check ─────────────────────────
    cout << "\n=== copy assignment ===\n";
    DynamicArray<int> d;
    d = b;
    d.remove(0);
    cout << "original: "; b.print();
    cout << "assigned: "; d.print();

    // ── self assignment guard ─────────────────────────────────────────────
    cout << "\n=== self assignment ===\n";
    b = b;
    cout << "b after b=b: "; b.print();

    // ── insert edge cases ─────────────────────────────────────────────────
    cout << "\n=== insert at every position ===\n";
    DynamicArray<int> e;
    e.append(1); e.append(3); e.append(5);
    e.print();
    e.insert(0, 0);     e.print();   // before everything
    e.insert(2, 2);     e.print();   // middle
    e.insert(4, 4);     e.print();   // middle again
    e.insert(e.size(), 6); e.print();// after everything

    // ── remove until empty ────────────────────────────────────────────────
    cout << "\n=== remove until empty ===\n";
    DynamicArray<int> f;
    f.append(1); f.append(2); f.append(3);
    f.print();
    while (f.size() > 0) {
        f.remove(0);
        cout << "after remove: "; f.print();
    }

    // ── string type ───────────────────────────────────────────────────────
    cout << "\n=== string type ===\n";
    DynamicArray<string> s;
    s.append("hello"); s.append("world"); s.append("foo");
    s.insert(1, "there");
    s.print();
    s.remove(2);
    s.print();
    cout << "get(0)=" << s.get(0) << "  get(1)=" << s.get(1) << "\n";

    // ── float type ────────────────────────────────────────────────────────
    cout << "\n=== float type ===\n";
    DynamicArray<float> fl;
    fl.append(1.1f); fl.append(2.2f); fl.append(3.3f);
    fl.insert(1, 1.5f);
    fl.print();
    fl.remove(2);
    fl.print();

    // ── const get ─────────────────────────────────────────────────────────
    cout << "\n=== const get ===\n";
    const DynamicArray<int> g(b);
    cout << "const get(0)=" << g.get(0) << "\n";
    cout << "const get(1)=" << g.get(1) << "\n";

    // ── error: out of range ───────────────────────────────────────────────
    cout << "\n=== error: get out of range ===\n";
    a.get(999);

    return 0;
}