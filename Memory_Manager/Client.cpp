#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\Memory_Manager\MemoryManager.cpp"
#include "..\MPointers\MPointer.tpp"

#pragma comment(lib, "ws2_32.lib")

int main() {
    //MPointer<int>::Init(6060);
    //MPointer<int> ptr = MPointer<int>::New();
    //*ptr = 122323;
    //MPointer<double>::Init(6060);
    //MPointer<double> ptrDouble = MPointer<double>::New();
    //*ptrDouble = 122323;
    MPointer<float>::Init(6060);
    MPointer<float> ptrFloat = MPointer<float>::New();
    *ptrFloat = 122323;
    return 0;
}