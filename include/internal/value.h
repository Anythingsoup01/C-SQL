#pragma once

#include <memory.h>

#include <stdlib.h>

namespace anythingsoup {


// This is a const char* memory storage for all our items and doesn't allow for comparisons
struct ManagedValueArena
{
  char* Base = nullptr;
  size_t Capacity = 0;
  size_t Offset = 0;

  ManagedValueArena(size_t size)
  {
    Base = (char *)malloc(size);
    Capacity = size;
    Offset = 0;
  }

  ~ManagedValueArena()
  {
    free(Base);
    Base = nullptr;
    Capacity = 0;
    Offset = 0;
  }


  char* alloc(size_t len, size_t alignment = 1)
  {
    size_t padding = (alignment - (Offset % alignment)) % alignment;

    if (Offset + padding + len > Capacity) return nullptr;

    Offset += padding;
    char* ptr = Base + Offset;
    Offset += len;
    return ptr;
  }

  template <typename T>
  T* Push(T value) {
    char *memory = alloc(sizeof(T), alignof(T));
    if (!memory) return nullptr;

    T* typePtr = reinterpret_cast<T*>(memory);
    return typePtr;
  }

  void reset() { Offset = 0; }
};

}
