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