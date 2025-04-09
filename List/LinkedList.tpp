#ifndef LINKEDLIST_TPP
#define LINKEDLIST_TPP

#include <cstddef>
#include <iostream>
#include "C:\Users\XPC\OneDrive - Estudiantes ITCR\Escritorio\Proyecto 1\MPointers_2.0\MPointers\MPointer.tpp"

template <typename T>
struct Node {
    T data;
    Mpointer<Node<T>> next;

    Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Mpointer<Node<T>> head;
    size_t size;
public:
    LinkedList() : head(nullptr), size(0) {}

    void append(const T& value) {
         Mpointer<Node<T>> newNode = new Node<T>(value);
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
    Mpointer<Node<T>> getHead() const { return head; }

    void print() const {
         Mpointer<Node<T>> current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // LINKEDLIST_TPP
