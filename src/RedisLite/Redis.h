#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "../HashMap/hmap.h"

using namespace std;

class RedisLite {
public:
    void run();

private:
    HashMap<string, string> storage;

    void printCommands();
    void handleSet(stringstream& ss);
    void handleGet(stringstream& ss);
    void handleDel(stringstream& ss);
    void handleExists(stringstream& ss);
    void handleCount();
    void handlePrint();
    void handleStats();
    void handleClear();
};

// ---------------------------------------------------------
// run() — read, parse, dispatch. No HashMap calls live here.
// ---------------------------------------------------------
void RedisLite::run() {
    string line;
    cout << "===== Welcome to Redis Lite CLI =====" << endl;
    printCommands();

    while (true) {
        cout << "-> ";
        if (!getline(cin, line)) break;   // EOF (e.g. Ctrl+D)
        if (line.empty()) continue;

        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "SET")          handleSet(ss);
        else if (command == "GET")     handleGet(ss);
        else if (command == "DEL")     handleDel(ss);
        else if (command == "EXISTS")  handleExists(ss);
        else if (command == "COUNT")   handleCount();
        else if (command == "PRINT")   handlePrint();
        else if (command == "STATS")   handleStats();
        else if (command == "CLEAR")   handleClear();
        else if (command == "HELP")    printCommands();
        else if (command == "EXIT")    break;
        else cout << "Unknown command. Type HELP for the command list." << endl;
    }
}

void RedisLite::printCommands() {
    cout << "Commands -" << endl;
    cout << "  SET <key> <value>" << endl;
    cout << "  GET <key>" << endl;
    cout << "  DEL <key>" << endl;
    cout << "  EXISTS <key>" << endl;
    cout << "  COUNT" << endl;
    cout << "  PRINT" << endl;
    cout << "  STATS" << endl;
    cout << "  CLEAR" << endl;
    cout << "  HELP" << endl;
    cout << "  EXIT" << endl << endl;
}

// ---------------------------------------------------------
// SET <key> <value>
// ---------------------------------------------------------
void RedisLite::handleSet(stringstream& ss) {
    string key, value;
    ss >> key;
    getline(ss, value);                       // grabs the rest of the line
    if (!value.empty() && value[0] == ' ')     // strip the leading space
        value.erase(0, 1);

    if (key.empty() || value.empty()) {
        cout << "Usage: SET <key> <value>" << endl;
        return;
    }

    storage.set(key, value);   // overwrite-on-duplicate handled inside set()
    cout << "OK" << endl;
}

// ---------------------------------------------------------
// GET <key>
// exists() check first — get() exit(1)s on a missing key,
// so we must never call it blind.
// ---------------------------------------------------------
void RedisLite::handleGet(stringstream& ss) {
    string key;
    ss >> key;

    if (key.empty()) {
        cout << "Usage: GET <key>" << endl;
        return;
    }

    if (storage.exists(key)) {
        cout << storage.get(key) << endl;
    } else {
        cout << "Not Found" << endl;
    }
}

// ---------------------------------------------------------
// DEL <key>
// Same exists()-first guard as handleGet() — remove() also
// exit(1)s on a missing key.
// ---------------------------------------------------------
void RedisLite::handleDel(stringstream& ss) {
    string key;
    ss >> key;

    if (key.empty()) {
        cout << "Usage: DEL <key>" << endl;
        return;
    }

    if (storage.exists(key)) {
        storage.remove(key);
        cout << "Deleted" << endl;
    } else {
        cout << "Not Found" << endl;
    }
}

// ---------------------------------------------------------
// EXISTS <key>
// ---------------------------------------------------------
void RedisLite::handleExists(stringstream& ss) {
    string key;
    ss >> key;

    if (key.empty()) {
        cout << "Usage: EXISTS <key>" << endl;
        return;
    }

    cout << (storage.exists(key) ? "True" : "False") << endl;
}

// ---------------------------------------------------------
// COUNT
// ---------------------------------------------------------
void RedisLite::handleCount() {
    cout << storage.size() << endl;
}

// ---------------------------------------------------------
// PRINT
// ---------------------------------------------------------
void RedisLite::handlePrint() {
    storage.print();
}

// ---------------------------------------------------------
// STATS
// bucketCount is a public field on HashMap, not a function —
// accessed directly, no parentheses.
// ---------------------------------------------------------
void RedisLite::handleStats() {
    cout << "Elements   : " << storage.size() << endl;
    cout << "Buckets    : " << storage.bucketCount << endl;
    cout << "LoadFactor : " << storage.loadFactor() << endl;
}

// ---------------------------------------------------------
// CLEAR
// Calls the HashMap's own clear() directly — resets the
// bucket array and count without hashing/removing each key
// individually.
// ---------------------------------------------------------
void RedisLite::handleClear() {
    storage.clear();
    cout << "Cleared" << endl;
}