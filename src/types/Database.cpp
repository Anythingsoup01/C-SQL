#include "types/Database.h"

#include <sqlite3.h>

namespace anythingsoup {


ColumnType sqlite3_type_to_column_type(int type) {
  switch (type) {
    case SQLITE_NULL: return ColumnType::_NULL;
    case SQLITE_INTEGER: return ColumnType::INTEGER;
    case SQLITE_FLOAT: return ColumnType::FLOAT;
    case SQLITE_TEXT: return ColumnType::TEXT;
    case SQLITE_BLOB: return ColumnType::BLOB;
    default: return ColumnType::_NULL;
  }
}

template <typename T>
Column::Column(const char *name, T value, ColumnType type, size_t size)
: Name(name), Type(type), Size(size) {
  static_assert(false, "invalid type");
}

template <>
Column::Column(const char *name, int value, ColumnType type, size_t size)
: Name(name), Type(type), Size(size) {
  memset(Data, 0, sizeof(Data));
  memcpy(Data, &value, sizeof(int));
}

template <>
Column::Column(const char *name, float value, ColumnType type, size_t size)
: Name(name), Type(type), Size(size) {
  memset(Data, 0, sizeof(Data));
  memcpy(Data, &value, sizeof(float));
}

template <>
Column::Column(const char *name, void *value, ColumnType type, size_t size)
: Name(name), Type(type), Size(size) {
  memset(Data, 0, sizeof(Data));
  memcpy(Data, value, sizeof(void *));
}

template <>
Column::Column(const char *name, const void *value, ColumnType type, size_t size)
: Name(name), Type(type), Size(size) {
  memset(Data, 0, sizeof(Data));
  memcpy(Data, value, size);
}

template <>
Column::Column(const char *name, std::nullptr_t value, ColumnType type, size_t size)
: Name(name), Type(type), Size(size) {
  memset(Data, 0, sizeof(Data));
}



}
