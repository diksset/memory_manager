// менеджер памяти блочного типа
#include <iostream>
#include "prototypes.h"

int Blocks = 1;
char memory[10000]; //доступная память

struct {
	int Handle;
	int Size;
	int Begin; 
	bool Storage;
} Block[30];

void InitMemory(void) {
	Block[0].Handle = 0;
	Block[0].Size = sizeof(memory);
	Block[0].Begin = 0;
	Block[0].Storage = false;
}

int AllocMemory(int Size) {
	for (int i = 0; i < 30; i++) {
		if (!Block[i].Storage && Block[i].Size > Size) {
			Block[i + 1].Size = Block[i].Size - Size;
			Block[i].Size = Size;
			Block[i + 1].Begin = Block[i].Begin + Size;
			Block[i].Storage = true;
			Block[i].Handle = Blocks;
			Blocks++;

			return Block[i].Handle;
		}
		if (!Block[i].Storage && Block[i].Size == Size) {
			Block[i + 1].Size = 0;
			Block[i + 1].Begin = Block[i].Begin + Size;
			Block[i].Storage = true;
			Block[i].Handle = Blocks;
			Blocks++;

			return Block[i].Handle;
		}
	}
	return 0;
}

void FreeMemory(int MemoryHandle) { 
	int tmp = 0;
	for (int i = 0; i < 30; i++) {
		if (Block[i].Handle == MemoryHandle) {
			Block[i].Storage = false;
			tmp = i;
			break;
		}
	}
	if (tmp) { //утряска
		while (Block[tmp + 1].Storage) {
			Block[tmp] = Block[tmp + 1];
			Block[tmp].Begin = Block[tmp - 1].Begin + Block[tmp - 1].Size;
			Block[tmp + 1].Storage = false;
			char* oldMem = &memory[Block[tmp + 1].Begin];
			char* newMem = &memory[Block[tmp].Begin];
			for (int j = 0; j < Block[tmp + 1].Size; j++) {
				newMem[j] = oldMem[j];
				oldMem[j] = 0;
			}
			tmp++;
		}
		if (!Block[tmp + 1].Handle) {
			Block[tmp].Handle = 0;
			Block[tmp + 1].Size += Block[tmp].Size;
			Block[tmp].Size = 0;
			Block[tmp + 1].Begin = Block[tmp - 1].Begin + Block[tmp - 1].Size;
		}
	}
}

int WriteMemory(int MemoryHandle, int Offset, int Size, void* Data) { //запись данных в память
	if (Offset < 0 || Size <= 0) return 1;
	for (int i = 0; i < 30; i++) {
		if (Block[i].Handle == MemoryHandle) {
			if (Offset + Size <= Block[i].Size) {
				char* memPtr = &memory[Block[i].Begin + Offset];
				char* dataPtr = (char*)Data;
				for(int j = 0;j < Size; j++) {
					memPtr[j] = dataPtr[j];
				}
				return 0;
			}
		}
	}
	return 2;
}

int ReadMemory(int MemoryHandle, int Offset, int Size, void* Data) {
	if (Offset < 0 || Size <= 0) return 1;
	for (int i = 0; i < 30; i++) {
		if (Block[i].Handle == MemoryHandle) {
			if (Offset + Size <= Block[i].Size) {
				char* memPtr = &memory[Block[i].Begin + Offset];
				char* dataPtr = (char*)Data; 
				for (int j = 0; j < Size; j++) {
					dataPtr[j] = memPtr[j];
				}
				return 0;
			}
		}
	}
	return 2;
}

void DumpMemory(void) {
	for (int i = 0; i < 30; i++) {
		if (Block[i].Size)
		{
			printf("H:%i ", Block[i].Handle);
			printf("B:%i ", Block[i].Begin);
			printf("S:%i ", Block[i].Size);
			if (Block[i].Storage) printf("Yes ");
			else printf("No ");
			char* ptr = &memory[Block[i].Begin];
			for (int j = 0; j < 10; j++) printf("%i", ptr[j]);
			printf("\n");
		}
	}
}