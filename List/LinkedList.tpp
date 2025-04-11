#ifndef LINKEDLIST_TPP
#define LINKEDLIST_TPP

#include <cstddef>
#include <iostream>
#include "..\MPointers\MPointer.tpp"


struct Node {
    int data;
    MPointer<Node> next = MPointer<Node>::New();

    Node(int value){
        data = value;
        next = nullptr; }
};


class LinkedList {
private:
    MPointer<Node> head = MPointer<Node>::New();

public:
    LinkedList(){
        head = nullptr;
    }

    void append(int value) {
        MPointer<Node> newNode = MPointer<Node>::New();
        *newNode = Node(value); // Asignar el dato (data = 10)
        if (head) {
            head = newNode;
        } else {
            MPointer<Node> current = head;
            while (current->next) {
                current = (*current).next;
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