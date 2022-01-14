#pragma once
#include <common/types.h>
namespace os {
struct MemoryChunk {
  MemoryChunk *next;
  MemoryChunk *prev;
  bool allocated;
  common::size_t size;
};

class MemoryManager {
private:
  MemoryChunk *head;

public:
  static MemoryManager *activeMemoryManager;

  MemoryManager(common::size_t start, common::size_t size);
  ~MemoryManager();

  void *malloc(common::size_t size);
  void free(void *ptr);
};
} // namespace os

void *operator new(unsigned int size);
void *operator new[](unsigned int size);

void operator delete(void *ptr);
void operator delete[](void *ptr);

// placement new operator
void *operator new(unsigned int size, void *ptr);
void *operator new[](unsigned int size, void *ptr);
void operator delete(void *, unsigned int);
void operator delete[](void *, unsigned int);