#ifndef LINKEDLIST_TPP
#define LINKEDLIST_TPP

#include <cstddef>
#include <iostream>
#include "..\MPointers\MPointer.h"

template <typename T>
struct Node {
    T data;
    MPointer<Node<T>> next;

    Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    MPointer<Node<T>> head;
    size_t size;
public:
    LinkedList() : head(nullptr), size(0) {}

    void append(const T& value) {
         MPointer<Node<T>> newNode = new Node<T>(value);
        if (!head) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    void remove(Node<T>* node) {
        if (!node) return;
        if (head == node) {
            head = head->next;
            delete node;
            size--;
            return;
        }
        Node<T>* current = head;
        while (current && current->next != node) {
            current = current->next;
        }
        if (current) {
            current->next = node->next;
            delete node;
            size--;
        }
    }


    size_t getSize() const { return size; }
    MPointer<Node<T>> getHead() const { return head; }

    void print() const {
         MPointer<Node<T>> current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // LINKEDLIST_TPP
