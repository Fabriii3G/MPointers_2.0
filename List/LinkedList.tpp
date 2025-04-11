#ifndef LINKEDLIST_TPP
#define LINKEDLIST_TPP

#include <cstddef>
#include <iostream>
#include "...\MPointers\MPointer.tpp"


struct Node {
    int data;
    MPointer<Node> next;

    Node(const T& value) : data(value), next(nullptr) {}
};


class LinkedList {
private:
    MPointer<Node> head;

public:
    LinkedList() : head(nullptr) {}

    void append(const T& value) {
        MPointer<Node> newNode = new Node(value);
        if (!head) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }

    }


    void print() const {
        MPointer<Node> current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // LINKEDLIST_TPP