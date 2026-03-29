#include "internal/string.h"

namespace anythingsoup {

StringStorage::StringStorage(uint64_t storageSize)
: m_Arena(storageSize) {

}

StringStorage::~StringStorage() {}


ManagedString StringStorage::StringIntern(const char* str) {
  return m_Table.intern(str, m_Arena);
}

}
