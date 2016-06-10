#include <iostream>
#include <unordered_map>


class SmallAllocator {
private:
        unsigned int* Memory;
        std::unordered_map<unsigned int, unsigned int> memoryMapper;
        unsigned int *currentFreeMemory;
public:

        SmallAllocator(){
            unsigned int array[1048576];
            Memory = &array[0];
            currentFreeMemory = Memory;
        };

        void *Alloc(unsigned int Size) {
            unsigned int* returnPointer = currentFreeMemory;
            currentFreeMemory += Size;

            memoryMapper.insert({{*returnPointer},{Size}});

            return returnPointer;
        };

        void *ReAlloc(void *Pointer, unsigned int Size) {
            unsigned int* returnPointer = currentFreeMemory;
            unsigned int* oldPointer = (unsigned int*)Pointer;
            std::unordered_map<unsigned int,unsigned int>::const_iterator got = memoryMapper.find(*oldPointer);
            unsigned int oldSize = got->second;

            for (int i = 0; i < oldSize; i++) {
                returnPointer[i] = oldPointer[i];
            }

            memoryMapper.erase(*oldPointer);
            memoryMapper.insert({{*returnPointer},{Size}});

            currentFreeMemory += Size;

            return returnPointer;
        };

        void Free(void *Pointer) {

        };
};

int main () {

    SmallAllocator A1;
    int * A1_P1 = (int *) A1.Alloc(sizeof(int));
    A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);

    SmallAllocator A2;
    int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;

    int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;

    A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 20; i++) {
        if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
    }
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
    A2.Free(A2_P1);
    A2.Free(A2_P2);


    return 0;
}