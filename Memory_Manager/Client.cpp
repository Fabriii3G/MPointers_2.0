#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\Memory_Manager\MemoryManager.cpp"
#include "..\MPointers\MPointer.tpp"
//#include "..\List\LinkedList.tpp"

#pragma comment(lib, "ws2_32.lib")

int main() {
    MPointer<int>::Init(6060);
    MPointer<int> ptr = MPointer<int>::New();
    MPointer<int> ptr1 = MPointer<int>::New();
    MPointer<int> ptr2 = MPointer<int>::New();
    *ptr = 122323;
    *ptr1 = 123123;
    *ptr2 = 123456;
    return 0;
}