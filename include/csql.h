#pragma once

#include "base_db.h"

#include <unordered_map>
#include <string>

namespace anythingsoup {

struct Database {
  std::string Name;
  std::unordered_map<std::string, std::string> m_Table;
};

//
//  The standard Database Handler Class of the library.
//
class csql : public base_db {
public:
  csql() = default;
  virtual ~csql();

  virtual bool AddDatabase(const char *filePath) override;
private:
  std::unordered_map<std::string, Database> m_DatabaseMap;
};

}
