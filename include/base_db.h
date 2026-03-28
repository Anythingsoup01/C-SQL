#pragma once

namespace anythingsoup {

class base_db {
public:
  virtual ~base_db() = default;

  virtual bool AddDatabase(const char *filePath) = 0;
};

}
