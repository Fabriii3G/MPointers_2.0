#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "C:\Users\alear\CLionProjects\MPointers_2.0\Memory_Manager\MemoryManager.cpp"
#include "C:\Users\alear\CLionProjects\MPointers_2.0\MPointers\MPointer.tpp"

#pragma comment(lib, "ws2_32.lib")

int main() {
    MPointer<int>::Init(6060);
    MPointer<int> ptr = MPointer<int>::New();
    *ptr = 123;
    return 0;
}