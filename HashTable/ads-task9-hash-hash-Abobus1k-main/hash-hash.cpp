#include <iostream>
#include <fstream>
using namespace std;
const int kTableSize = 300007;
class Node {
public:
    int key;
    Node* next;

    Node(int key) : key(key), next(nullptr) {}
};

class HashTable {
public:
    HashTable() : table(new Node* [kTableSize]()) {}

    ~HashTable() {
        for (int i = 0; i < kTableSize; ++i) {
            Node* node = table[i];
            while (node != nullptr) {
                Node* next = node->next;
                delete node;
                node = next;
            }
        }
        delete[] table;
    }

    bool add(int key) {
        int index = hash(key);
        Node* node = table[index];
        while (node != nullptr) {
            if (node->key == key) {
                return false;
            }
            node = node->next;
        }
        node = new Node(key);
        node->next = table[index];
        table[index] = node;
        return true;
    }

    bool remove(int key) {
        int index = hash(key);
        Node* node = table[index];
        Node* prev = nullptr;
        while (node!=nullptr) {
            if (node->key == key) {
                if (prev!=nullptr) {
                    prev->next = node->next;
                }
                else {
                    table[index] = node->next;
                }
                delete node;
                return true;
            }
            prev = node;
            node = node->next;
        }
        return false;
    }

    bool contains(int key) {
        int index = hash(key);
        Node* node = table[index];
        while (node!=nullptr) {
            if (node->key == key) {
                return true;
            }
            node = node->next;
        }
        return false;
    }

private:
    Node** table;

    static int hash(int key) {
        int a = 17;
        int b = 31;
        int p = 300017;
        if (key < 0) {
            key = -key;
        }
        return ((a * key + b) % p) % kTableSize;
    }
};

int main(int argc, char* argv[]) {
    HashTable set;
    string ifilename = argv[1];
    string ofilename = argv[2];
    ifstream inputFile(ifilename);
    ofstream outputFile(ofilename);
    int N;
    inputFile >> N;
    for (int i = 0; i < N; i++) {
        char command;
        inputFile >> command;
        if (command == '+')
        {
            int x;
            inputFile >> x;
            set.add(x);
        }
        else if (command == '-')
        {
            int x;
            inputFile >> x;
            set.remove(x);
        }
        else
        {
            int x;
            inputFile >> x;
            if (set.contains(x))
            {
                outputFile << "true" << '\n';
            }
            else
            {
                outputFile << "false" << '\n';
            }
        }
    }
    inputFile.close();
    outputFile.close();
    return 0;
}
