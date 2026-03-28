#include "csql.h"

#include <sqlite3.h>

#include <iostream>


namespace anythingsoup {

csql::~csql() {

}

bool csql::AddDatabase(const char *filePath) {
  sqlite3 *db;
  if (sqlite3_open(filePath, &db) != SQLITE_OK) {
    return false;
  }

  sqlite3_stmt *stmt;
  const char* command = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";

  if (sqlite3_prepare_v2(db, command, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
    return false;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    const unsigned char* text = sqlite3_column_text(stmt, 0);
    if (text) {
      std::string tableName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      std::cout << tableName << "\n";
    }
  }


  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return true;
}

}
