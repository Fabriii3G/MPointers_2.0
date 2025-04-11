#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\Memory_Manager\MemoryManager.cpp"
#include "..\MPointers\MPointer.tpp"
//#include "..\List\LinkedList.tpp"

#pragma comment(lib, "ws2_32.lib")

int main() {
    MPointer<float>::Init(6060);
    MPointer<float> ptr = MPointer<float>::New();
    MPointer<float> ptr1 = MPointer<float>::New();
    MPointer<float> ptr2 = MPointer<float>::New();
    *ptr = 3.14;
    *ptr2 = 123456;
    ptr1 = ptr;

    *ptr = 4.17;

    return 0;
}