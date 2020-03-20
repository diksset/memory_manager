#pragma once

void InitMemory(void);
int AllocMemory(int Size);
void FreeMemory(int MemoryHandle);
int WriteMemory(int MemoryHandle, int Offset, int Size, void* Data);
int ReadMemory(int MemoryHandle, int Offset, int Size, void* Data);
void DumpMemory(void);