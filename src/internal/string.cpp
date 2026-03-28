#include "internal/string/string.h"

namespace anythingsoup {

StringStorage::StringStorage(uint64_t storageSize)
: m_Arena(storageSize) {

}

StringStorage::~StringStorage() {}

ManagedString StringStorage::StringLiteral(const char* s) {
  return {
    .Data = s,
    .Size = (uint64_t)strlen(s),
    .Type = StringType::Literal
  };
}

ManagedString StringStorage::StringHeap(const char* s) {
  size_t len = strlen(s) + 1;
  char* buf = new char[len];
  memcpy(buf, s, len);
  return {
    .Data = buf,
    .Size = (uint64_t)(len - 1),
    .Type = StringType::Heap
  };
}

ManagedString StringStorage::StringArena(const char* s) {
  if (!s) return { nullptr, 0, StringType::Arena };

  size_t len = strlen(s);
  // Allocate space for string + null terminator
  char* buf = m_Arena.alloc(len + 1);
  if (!buf) return { nullptr, 0, StringType::Arena };

  memcpy(buf, s, len + 1); // Copy including null terminator

  return { buf, static_cast<uint64_t>(len), StringType::Arena };
}

ManagedString StringStorage::StringIntern(const char* str) {
  return m_Table.intern(str, m_Arena);
}

}
