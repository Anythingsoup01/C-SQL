#pragma once

#include "base_db.h"

#include "types/Database.h"

#include "internal/string/string.h"

namespace anythingsoup {



//
//  The standard Database Handler Class of the library.
//
class csql : public base_db {
public:
  csql();
  virtual ~csql();

  virtual bool AddDatabase(const char *filePath) override;

  Database GetDatabase(const char *dbName) { if (!m_DatabaseMap.contains(dbName)) return {}; return m_DatabaseMap.at(dbName); }
private:
  std::unordered_map<std::string, Database> m_DatabaseMap;
  StringStorage m_StringStorage;
};

}
