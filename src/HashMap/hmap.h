#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../DynamicArray/Dynamicarray.h"
#include "../LinkedList/Linkedlist.h"
using namespace std;

// Hash — compiler picks the right specialization based on K
// for user-defined types: write your own specialization (see Point below)
template<typename K>
struct Hash {
    int operator()(const K& key, int buckets) const;
};

template<>
struct Hash<int> {
    int operator()(const int& key, int buckets) const {
        unsigned u = (unsigned) key;
        u = u * 2654435761u;
        return (int)(u % (unsigned) buckets);
    }
};

template<>
struct Hash<char> {
    int operator()(const char& key, int buckets) const {
        return Hash<int>()((int)(unsigned char) key, buckets);
    }
};

template<>
struct Hash<float> {
    int operator()(const float& key, int buckets) const {
        unsigned bits;
        memcpy(&bits, &key, sizeof(float));
        return (int)(bits % (unsigned) buckets);
    }
};

template<>
struct Hash<double> {
    int operator()(const double& key, int buckets) const {
        unsigned long long bits;
        memcpy(&bits, &key, sizeof(double));
        bits ^= bits >> 33;
        bits *= 0xff51afd7ed558ccdULL; //produces good miing of bits, chosen by the designers of the MurmurHash3 algorithm
        bits ^= bits >> 33;
        return (int)(bits % (unsigned long long) buckets);
    }
};

template<>
struct Hash<string> {
    int operator()(const string& key, int buckets) const {
        unsigned long h = 5381; //DJB2 algorithm
        for (char c : key)
            h = ((h << 5) + h) + (unsigned char) c;
        return (int)(h % (unsigned long) buckets);
    }
};

template<>
struct Hash<const char*> {
    int operator()(const char* const& key, int buckets) const {
        unsigned long h = 5381;
        const char* p = key;
        while (*p) h = ((h << 5) + h) + (unsigned char)(*p++);
        return (int)(h % (unsigned long) buckets);
    }
};

// Pair<K,V> — one key-value entry stored inside a bucket chain
template<typename K, typename V>
struct Pair {
    K key;
    V val;
    Pair() {}
    Pair(const K& k, const V& v) : key(k), val(v) {}
    bool operator==(const Pair& o) const { return key == o.key; }
};


template<typename K, typename V>
ostream& operator<<(ostream& os, const Pair<K,V>& p) {
    return os << p.key << ":" << p.val;
}

// HashMap<K, V>
template<typename K, typename V>
class HashMap {

public:
    int bucketCount;

private:
    DynamicArray< SinglyLinkedList< Pair<K,V> > > buckets;
    int n;
    Hash<K> hasher;

    static const int P = 3;

    void error(const char* msg) const { cout << "HashMap: " << msg << "\n"; exit(1); }

    int homeIndex(const K& key) const {
        return hasher(key, bucketCount);
    }

    void rehash(int newCount) {
        DynamicArray< SinglyLinkedList< Pair<K,V> > > nb;
        for (int i = 0; i < newCount; i++)
            nb.append(SinglyLinkedList< Pair<K,V> >());

        for (int i = 0; i < bucketCount; i++) {
            typename SinglyLinkedList< Pair<K,V> >::Node* c = buckets.get(i).getHead();
            while (c) {
                int h = hasher(c->data.key, newCount);
                bool placed = false;
                for (int p = 0; p < P; p++) {
                    int pr = (h + p) % newCount;
                    if (nb.get(pr).size() == 0) {
                        nb.get(pr).insertBack(c->data);
                        placed = true;
                        break;
                    }
                }
                if (!placed) nb.get(h).insertBack(c->data);
                c = c->next;
            }
        }
        buckets = nb;
        bucketCount = newCount;
    }

public:

    HashMap() : bucketCount(8), n(0) {
        for (int i = 0; i < bucketCount; i++)
            buckets.append(SinglyLinkedList< Pair<K,V> >());
    }

    ~HashMap() {}

    int   size()       const { return n; }
    float loadFactor() const { return (float) n / (float) bucketCount; }

    void set(const K& key, const V& val) {
        if (loadFactor() >= 0.7f)
            rehash(bucketCount * 2);

        int h = homeIndex(key);

        // update if key already exists
        for (int p = 0; p < P; p++) {
            int pr = (h + p) % bucketCount;
            typename SinglyLinkedList< Pair<K,V> >::Node* c = buckets.get(pr).getHead();
            while (c) {
                if (c->data.key == key) { c->data.val = val; return; }
                c = c->next;
            }
        }

        // insert into first empty probe slot
        for (int p = 0; p < P; p++) {
            int pr = (h + p) % bucketCount;
            if (buckets.get(pr).size() == 0) {
                buckets.get(pr).insertBack(Pair<K,V>(key, val));
                n++;
                return;
            }
        }

        // all probe slots full — chain at home bucket
        buckets.get(h).insertBack(Pair<K,V>(key, val));
        n++;
    }

    V& get(const K& key) const {
        int h = homeIndex(key);
        for (int p = 0; p < P; p++) {
            int pr = (h + p) % bucketCount;
            typename SinglyLinkedList< Pair<K,V> >::Node* c = buckets.get(pr).getHead();
            while (c) {
                if (c->data.key == key) return c->data.val;
                c = c->next;
            }
        }
        error("get: key not found");
        return buckets.get(0).getHead()->data.val;
    }

    bool exists(const K& key) const {
        int h = homeIndex(key);
        for (int p = 0; p < P; p++) {
            int pr = (h + p) % bucketCount;
            typename SinglyLinkedList< Pair<K,V> >::Node* c = buckets.get(pr).getHead();
            while (c) {
                if (c->data.key == key) return true;
                c = c->next;
            }
        }
        return false;
    }

    void remove(const K& key) {
        int h = homeIndex(key);
        for (int p = 0; p < P; p++) {
            int pr = (h + p) % bucketCount;
            typename SinglyLinkedList< Pair<K,V> >::Node* c = buckets.get(pr).getHead();
            while (c) {
                if (c->data.key == key) {
                    buckets.get(pr).removeVal(c->data);
                    n--;
                    return;
                }
                c = c->next;
            }
        }
        error("remove: key not found");
    }

    void clear() {
    buckets = DynamicArray< SinglyLinkedList< Pair<K,V> > >();
    bucketCount = 8;
    for (int i = 0; i < bucketCount; i++)
        buckets.append(SinglyLinkedList< Pair<K,V> >());
    n = 0;
}

    DynamicArray<K> keys() const {
        DynamicArray<K> result;
        for (int i = 0; i < bucketCount; i++) {
            typename SinglyLinkedList< Pair<K,V> >::Node* c = buckets.get(i).getHead();
            while (c) { result.append(c->data.key); c = c->next; }
        }
        return result;
    }

    DynamicArray<V> values() const {
        DynamicArray<V> result;
        for (int i = 0; i < bucketCount; i++) {
            typename SinglyLinkedList< Pair<K,V> >::Node* c = buckets.get(i).getHead();
            while (c) { result.append(c->data.val); c = c->next; }
        }
        return result;
    }

    void merge(const HashMap<K,V>& other) {
        DynamicArray<K> otherKeys = other.keys();
        for (int i = 0; i < otherKeys.size(); i++) {
            const K& k = otherKeys.get(i);
            if (!exists(k))
                set(k, other.get(k));
        }
    }

    void print() const {
        cout << "HashMap  size=" << n << "  buckets=" << bucketCount
             << "  load=" << loadFactor() << "\n";
        for (int i = 0; i < bucketCount; i++) {
            if (buckets.get(i).size() == 0) continue;
            cout << "  [" << i << "] ";
            typename SinglyLinkedList< Pair<K,V> >::Node* c = buckets.get(i).getHead();
            while (c) {
                cout << "{" << c->data.key << " : " << c->data.val << "}";
                if (c->next) cout << " -> ";
                c = c->next;
            }
            cout << "\n";
        }
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// user-defined type example
// ─────────────────────────────────────────────────────────────────────────────
struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
    friend ostream& operator<<(ostream& os, const Point& p) {
        return os << "(" << p.x << "," << p.y << ")";
    }
};

template<>
struct Hash<Point> {
    int operator()(const Point& p, int buckets) const {
        unsigned h = (unsigned)(p.x * 73856093 ^ p.y * 19349663);
        return (int)(h % (unsigned) buckets);
    }
};