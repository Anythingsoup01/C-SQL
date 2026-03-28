#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory.h>

namespace anythingsoup {

enum class ColumnType {
  _NULL = 0,
  INTEGER,
  FLOAT,
  TEXT,
  BLOB,
};

ColumnType sqlite3_type_to_column_type(int type);

struct Column {
public:
  template <typename T>
  Column(const char *name, T value, ColumnType type, size_t size);

  template <typename T>
  T GetData() {
    return *(T*)Data;
  }

private:
  std::string Name;
  ColumnType Type;
  char Data[128];
  size_t Size;
};

struct Row {
  std::vector<Column> Columns;
  int ColumnCount;
};

using Table = std::vector<Row>;

struct Database {
  std::string Name;
  std::unordered_map<std::string, Table> Tables;
};

}
