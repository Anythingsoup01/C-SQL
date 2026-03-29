#include "csql.h"

#include <sqlite3.h>
#include <iostream>

#include <filesystem>


namespace anythingsoup {



csql::csql()
  : m_StringStorage(1024*1024), m_ValueStorage(1024*1024) {
}

csql::~csql() {

}

bool csql::AddDatabase(const char *filePath) {
  Database newDB;
  sqlite3 *db;

  std::filesystem::path fPath(filePath);

  if (!std::filesystem::exists(fPath)) {
    return false;
  }

  if (sqlite3_open(fPath.c_str(), &db) != SQLITE_OK) {
    return false;
  }

  sqlite3_stmt *stmt;
  const char* command = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";

  if (sqlite3_prepare_v2(db, command, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << "\n";
    return false;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    const unsigned char* tabName = sqlite3_column_text(stmt, 0);

    if (!tabName)
      continue;

    ManagedString tableName = m_StringStorage.StringIntern(reinterpret_cast<const char *>(tabName));
    std::cout << "TABLE NAME - " << tableName.c_str() << "\n";

    sqlite3_stmt *stmt;
    std::stringstream command; command << "SELECT * FROM " << tableName.Data << ";";

    if (sqlite3_prepare_v2(db, command.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
      std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << "\n";
      continue;
    }

    Table table;

    int columnCount = sqlite3_column_count(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      Row row;
      row.ColumnCount = columnCount;
      for (int i = 0; i < columnCount; i++) {

        ManagedString colName = m_StringStorage.StringIntern(sqlite3_column_name(stmt, i));
        ColumnType type = sqlite3_type_to_column_type(sqlite3_column_type(stmt, i));

        void *dataPtr = nullptr;

        switch (type) {
          case ColumnType::_NULL: {
            std::cout << colName.c_str() << ": " << "NULL\t";
            break;
          }
          case ColumnType::INTEGER: {
            int val = sqlite3_column_int(stmt, i);
            dataPtr = (void *)m_ValueStorage.Push(val);

            std::cout << colName.c_str() << ": " << val << "\t";
            break;
          }
          case ColumnType::FLOAT: {
            float val = sqlite3_column_double(stmt, i);
            dataPtr = (void *)m_ValueStorage.Push(val);

            std::cout << colName.c_str() << ": " << val << "\t";
            break;
          }
          case ColumnType::TEXT: {
            ManagedString val = m_StringStorage.StringIntern((const char *)sqlite3_column_text(stmt, i));
            dataPtr = (void *)val.Data;

            std::cout << colName.c_str() << ": " << val.c_str() << "\t";
            break;
          }
          // This needs more attention than I can give right now
          case ColumnType::BLOB: {
            continue;
          }
        }
        std::cout << "\n";

        Column col(colName, dataPtr, type);
        row.Columns.push_back(std::move(col));

      }
      table.push_back(std::move(row));
    }

    newDB.Name = m_StringStorage.StringIntern(fPath.filename().c_str());
    newDB.Tables.emplace(std::pair<std::string, Table>(tableName.c_str(), table));
  }

  m_DatabaseMap.emplace(std::pair<std::string, Database>(newDB.Name.Data, newDB));

  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return true;
}

}
