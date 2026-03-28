#pragma once

#include <cstdint>
#include <memory.h>
#include <unordered_map>
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

enum class StringType : uint8_t {
  Literal,
  Heap,
  Arena,
  Interned
};

// This is a basic managed string utilized by the domain, but can be used basically anywhere
struct ManagedString
{
  const char* Data = nullptr;
  uint64_t    Size = 0;
  StringType  Type = StringType::Literal;

  inline const char* c_str() const { return Data ? Data : ""; }
  inline const size_t length() const { return Size; }

  inline const bool empty() const { return Size == 0 || Data == nullptr; }

  bool operator==(const ManagedString& rhs) const
  {
    if (this->Type == StringType::Interned &&
      rhs.Type  == StringType::Interned)
    {
      return this->Data == rhs.Data;
    }

    if (this->Size != rhs.Size)
      return false;

    return this->Size == 0 || memcmp(this->Data, rhs.Data, this->Size) == 0;
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
  std::unordered_map<uint64_t, const char*> Table;

  ManagedString intern(const char* s, ManagedStringArena& arena)
  {
    uint64_t hash = generate_hash(s);

    auto it = Table.find(hash);
    if (it != Table.end())
    {
      return { it->second, (uint64_t)strlen(s), StringType::Interned };
    }

    size_t len = strlen(s) + 1;
    char* buf = arena.alloc(len);
    if (!buf) return { nullptr, 0, StringType::Interned };

    memcpy(buf, s, len);
    Table[hash] = buf;

    return { buf, (uint64_t)(len - 1), StringType::Interned };
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
  ManagedString StringLiteral(const char *str);

  //
  //  Returns Managed String Object
  //
  //  Converts a const char * to a managed string heap
  ManagedString StringHeap(const char *str);

  //
  //  Returns Managed String Object
  //
  //  Converts a const char * to a managed string literal
  ManagedString StringArena(const char *str);

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
