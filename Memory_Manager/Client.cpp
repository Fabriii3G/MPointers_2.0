#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\Memory_Manager\MemoryManager.cpp"
#include "..\MPointers\MPointer.tpp"

#pragma comment(lib, "ws2_32.lib")

int main() {
    MPointer<int>::Init(6060);
    MPointer<int> ptr = MPointer<int>::New();
    *ptr = 122323;
    MPointer<double> ptrDouble = MPointer<double>::New();
    MPointer<float> ptrFloat = MPointer<float>::New();

    return 0;
}