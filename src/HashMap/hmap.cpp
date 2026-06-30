#include <iostream>
#include "hmap.h"
using namespace std;

int main() {

    cout << "=== int key ===\n";
    HashMap<int, string> m;
    m.set(1, "one"); m.set(2, "two"); m.set(3, "three");
    m.set(10, "ten"); m.set(20, "twenty");
    m.print();
    cout << "get(2)  = " << m.get(2)  << "\n";
    cout << "get(10) = " << m.get(10) << "\n";

    cout << "\n=== update existing key ===\n";
    m.set(2, "TWO updated");
    cout << "get(2) = " << m.get(2) << "\n";

    cout << "\n=== exists / remove ===\n";
    cout << "exists(3)  = " << (m.exists(3)  ? "yes" : "no") << "\n";
    cout << "exists(99) = " << (m.exists(99) ? "yes" : "no") << "\n";
    m.remove(3);
    cout << "after remove(3), exists(3) = " << (m.exists(3) ? "yes" : "no") << "\n";

    cout << "\n=== string key ===\n";
    HashMap<string, int> scores;
    scores.set("alice", 95); scores.set("bob", 87); scores.set("carol", 91);
    scores.print();
    cout << "alice = " << scores.get("alice") << "\n";

    cout << "\n=== char key ===\n";
    HashMap<char, string> alpha;
    alpha.set('a', "apple"); alpha.set('b', "banana"); alpha.set('c', "cherry");
    alpha.print();
    cout << "b = " << alpha.get('b') << "\n";

    cout << "\n=== resize test ===\n";
    HashMap<int, int> big;
    for (int i = 0; i < 30; i++) big.set(i, i * i);
    cout << "size=" << big.size() << "  buckets=" << big.bucketCount << "\n";
    cout << "get(15) = " << big.get(15) << "  expected=" << 15*15 << "\n";
    cout << "get(29) = " << big.get(29) << "  expected=" << 29*29 << "\n";

    cout << "\n=== keys() and values() ===\n";
    HashMap<string, int> kv;
    kv.set("x", 10); kv.set("y", 20); kv.set("z", 30);
    DynamicArray<string> ks = kv.keys();
    DynamicArray<int>    vs = kv.values();
    cout << "keys:   "; for (int i = 0; i < ks.size(); i++) cout << ks.get(i) << " "; cout << "\n";
    cout << "values: "; for (int i = 0; i < vs.size(); i++) cout << vs.get(i) << " "; cout << "\n";

    cout << "\n=== merge ===\n";
    HashMap<string, int> ma, mb;
    ma.set("a", 1); ma.set("b", 2);
    mb.set("b", 99); mb.set("c", 3);
    ma.merge(mb);
    cout << "a=" << ma.get("a") << " b=" << ma.get("b") << " c=" << ma.get("c") << "\n";
    cout << "b should be 2 (ma wins): " << (ma.get("b") == 2 ? "PASS" : "FAIL") << "\n";

    cout << "\n=== user-defined key (Point) ===\n";
    HashMap<Point, string> grid;
    grid.set({0,0}, "origin"); grid.set({1,0}, "right");
    grid.set({0,1}, "up");    grid.set({1,1}, "diagonal");
    grid.print();
    cout << "get({0,0}) = " << grid.get({0,0}) << "\n";
    cout << "get({1,1}) = " << grid.get({1,1}) << "\n";

    return 0;
}