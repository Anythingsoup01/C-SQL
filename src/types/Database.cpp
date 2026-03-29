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


Column::Column(const ManagedString &name, void *value, ColumnType type)
: Name(name), Type(type) {
  Data = value;
}


}
