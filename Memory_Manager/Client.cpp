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
    cout<< "dd";
    MPointer<int> ptrDouble = MPointer<int>::New();
    *ptr = 123;
    cout<< "dd";
    //MPointer<int> ptrDouble = MPointer<int>::New();
    //*ptrDouble = 12433232323;
    return 0;
}