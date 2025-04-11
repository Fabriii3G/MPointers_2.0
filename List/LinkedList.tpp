/*
#ifndef LINKEDLIST_TPP
#define LINKEDLIST_TPP

#include <iostream>
//#include "..\MPointers\MPointer.tpp"

struct Node {
    int data;
    MPointer<Node> next;

    Node() : data(0), next() {}  // constructor por defecto con inicialización
    Node(int value) : data(value), next() {}  // constructor con valor
};

class LinkedList {
private:
    MPointer<Node> head;
    bool isHeadSet = false;

public:
    LinkedList() : head() {}

    void append(int value) {
        MPointer<Node> newNode = MPointer<Node>::New();
        *newNode = Node(value);

        if (!isHeadSet) {
            head = newNode;
            isHeadSet = true;
        } else {
            MPointer<Node> current = head;
            while ((*current).next.GetID() != -1) {
                current = (*current).next;
            }
            (*current).next = newNode;
        }
    }

    void print() const {
        if (!isHeadSet) {
            std::cout << "Lista vacía.\n";
            return;
        }

        MPointer<Node> current = head;
        while ((*current).next.GetID() != -1) {
            std::cout << (*current).data << " -> ";
            current = (*current).next;
        }
        std::cout << (*current).data << " -> null\n";
    }
};

#endif // LINKEDLIST_TPP
*/