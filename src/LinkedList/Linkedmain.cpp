#include "Linkedlist.h"
#include <iostream>
using namespace std;

int main() {

    cout << "singly via base pointer\n";
    LinkedList<int>* s = new SinglyLinkedList<int>();
    s->insertBack(10);
    s->insertBack(20);
    s->insertBack(30);
    s->insertFront(5);
    s->print();

    s->insert(2, 99);    s->print();
    s->remove(2);        s->print();
    s->deleteFront();    s->print();
    s->deleteBack();     s->print();   // singly so this one's O(N), has to walk

    cout << "search(20) = " << (s->search(20) ? "found" : "not found") << "\n";
    cout << "search(999) = " << (s->search(999) ? "found" : "not found") << "\n";
    cout << "get(0) = " << s->get(0) << "\n";

    delete s;   // gotta go through the virtual destructor or ~SinglyLinkedList never runs

    cout << "\ndoubly via base pointer\n";
    LinkedList<int>* d = new DoublyLinkedList<int>();
    d->insertBack(10);
    d->insertBack(20);
    d->insertBack(30);
    d->insertFront(5);
    d->print();

    d->insert(2, 99);    d->print();
    d->remove(2);        d->print();
    d->deleteFront();    d->print();
    d->deleteBack();     d->print();   // O(1) here since it's doubly

    delete d;

    cout << "\nsingle node edge cases\n";
    SinglyLinkedList<int> single;
    single.insertBack(42);
    single.print();
    single.deleteBack();
    cout << "size after deleteBack on the only node = " << single.size() << "\n";

    DoublyLinkedList<int> singleD;
    singleD.insertBack(42);
    singleD.deleteFront();
    cout << "doubly size after deleteFront on the only node = " << singleD.size() << "\n";

    cout << "\ninsert/remove right at the front and right at the end\n";
    SinglyLinkedList<int> edge;
    edge.insertBack(1); edge.insertBack(2); edge.insertBack(3);
    edge.insert(0, 0);            edge.print();
    edge.insert(edge.size(), 4);  edge.print();
    edge.remove(0);                edge.print();
    edge.remove(edge.size() - 1);  edge.print();

    cout << "\nremoveVal — this is the one HashMap actually calls on a chain\n";
    SinglyLinkedList<int> chain;
    chain.insertBack(1); chain.insertBack(2); chain.insertBack(3);
    chain.print();
    cout << "removeVal(2) = " << (chain.removeVal(2) ? "true" : "false") << "\n";
    chain.print();
    cout << "removeVal(2) again = " << (chain.removeVal(2) ? "true" : "false") << "\n";

    cout << "\nclear, then make sure it's still usable after\n";
    chain.clear();
    chain.print();
    cout << "size after clear = " << chain.size() << "\n";
    chain.insertBack(777);
    chain.print();

    DoublyLinkedList<int> chainD;
    chainD.insertBack(1); chainD.insertBack(2); chainD.insertBack(3);
    cout << "removeVal(1), removing the head = " << (chainD.removeVal(1) ? "true" : "false") << "\n";
    chainD.print();
    cout << "removeVal(3), removing the tail = " << (chainD.removeVal(3) ? "true" : "false") << "\n";
    chainD.print();

    cout << "\ncopy constructor — has to be a real deep copy\n";
    DoublyLinkedList<int> orig;
    orig.insertBack(1); orig.insertBack(2); orig.insertBack(3);
    DoublyLinkedList<int> copyCtor(orig);
    copyCtor.insertBack(999);
    cout << "orig:     "; orig.print();       // shouldn't have 999
    cout << "copyCtor: "; copyCtor.print();

    cout << "\ncopy assignment — same deal\n";
    DoublyLinkedList<int> copyAssign;
    copyAssign.insertBack(-1);
    copyAssign = orig;
    copyAssign.insertBack(888);
    cout << "orig:       "; orig.print();
    cout << "copyAssign: "; copyAssign.print();

    cout << "\nself assignment, shouldn't blow up\n";
    orig = orig;
    orig.print();

    cout << "\nnon-int type just to be sure it's actually generic\n";
    DoublyLinkedList<string> str;
    str.insertBack("hello");
    str.insertBack("world");
    str.insertFront("hey");
    str.print();
    str.remove(1);
    str.print();

    cout << "\nempty list\n";
    SinglyLinkedList<int> empty;
    cout << "size=" << empty.size() << "\n";
    empty.print();

    cout << "\nerror paths, uncomment these one at a time to check exit(1)\n";
    // empty.deleteFront();
    // empty.deleteBack();
    // empty.get(0);
    // edge.remove(999);
    // orig.insert(999, 0);
    cout << "all commented out, reached the end fine\n";

    return 0;
}