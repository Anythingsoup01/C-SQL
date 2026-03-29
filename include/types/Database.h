#pragma once

#include <unordered_map>
#include <vector>

#include "internal/string.h"
#include <filesystem>

namespace anythingsoup {

enum class ColumnType : uint8_t {
  _NULL = 0,
  INTEGER,
  FLOAT,
  TEXT,
  BLOB,
};

ColumnType sqlite3_type_to_column_type(int type);

struct Column {
public:
  Column(const ManagedString &name, void * value, ColumnType type);

  template <typename T>
  T GetData() {
    if (Type == ColumnType::TEXT || Type == ColumnType::BLOB) {
      return reinterpret_cast<T>(Data); 
    }
    return *reinterpret_cast<T*>(Data);
  }

private:
  void *Data;
  ManagedString Name;
  ColumnType Type;
};

struct Row {
  std::vector<Column> Columns;
  int ColumnCount;
};

using Table = std::vector<Row>;

struct Database {
  ManagedString Name;
  std::filesystem::path Path;
  std::unordered_map<std::string, Table> Tables;
};

}
