#pragma once

#include <cstdint>
#include <memory.h>
#include <unordered_map>
#include <vector>
#include <string_view>

#include <stdlib.h>

namespace anythingsoup {

constexpr uint64_t generate_hash(const std::string_view& str)
{
  uint64_t hash = 14695981039346656037ull;
  for (unsigned char c : str)
  {
    hash ^= c;
    hash *= 1099511628211ull;
  }
    return hash;
}


// This is a basic managed string utilized by the domain, but can be used basically anywhere
struct ManagedString
{
  const char* Data = nullptr;

  ManagedString() = default;

  ManagedString(const char *data, uint32_t size)
    : Data(data) {}

  ManagedString(const ManagedString& other) 
  : Data(other.Data) {}

  ManagedString& operator=(const ManagedString& other) {
    if (this != &other) {
      Data = other.Data;
    }
    return *this;
  }

  inline const char* c_str() const { return Data ? Data : ""; }

  inline const bool empty() const { return Data == nullptr; }

  bool operator==(const ManagedString& rhs) const {
    return this->Data == rhs.Data;
  }
};

// This is a const char* memory storage for all our items and doesn't allow for comparisons
struct ManagedStringArena
{
  char* Base = nullptr;
  size_t Capacity = 0;
  size_t Offset = 0;

  ManagedStringArena(size_t size)
  {
    Base = (char*)malloc(size);
    Capacity = size;
    Offset = 0;
  }

  ~ManagedStringArena()
  {
    free(Base);
    Base = nullptr;
    Capacity = 0;
    Offset = 0;
  }

  char* alloc(size_t len)
  {
    if (Offset + len > Capacity) return nullptr;
    char* ptr = Base + Offset;
    Offset += len;
    return ptr;
  }

  void reset() { Offset = 0; }
};


// This is for strings we plan to compare, ie field names, and are stored seperately
// so we can store them and their pointers for pointers comparisons instead of strncmp(s)
struct ManagedStringTable
{
  std::unordered_map<uint64_t, std::vector<const char*>> Table;

  ManagedString intern(const char* s, ManagedStringArena& arena)
  {
    uint64_t hash = generate_hash(s);

    size_t len = strlen(s);

    auto it = Table.find(hash);
    if (it != Table.end())
    {
      for (auto *str : it->second) {
        if (strlen(str) != len) continue;

        if (strncmp(str, s, len) == 0) {
          return { str, (uint32_t)strlen(s) };
        }
      }
    }

    len = len + 1;
    char* buf = arena.alloc(len);
    if (!buf) return { nullptr, 0 };

    memcpy(buf, s, len);
    Table[hash].push_back(buf);

    return { buf, (uint32_t)(len - 1) };
  }

  void clear() { Table.clear(); }
};

class StringStorage {
public:
  StringStorage(uint64_t storageSize);
  ~StringStorage();

  //
  //  Returns Managed String Object
  //
  //  Converts a const char * to a managed string literal
  ManagedString StringIntern(const char *str);
private:
  ManagedStringArena m_Arena;
  ManagedStringTable m_Table;
};

}
