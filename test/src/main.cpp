#include <csql.h>

int main() {

  anythingsoup::csql csql;

  if (!csql.AddDatabase("test/db/credentials.db"))
    return -1;

  anythingsoup::Database db = csql.GetDatabase("credentials.db");
  anythingsoup::Table tab = db.Tables.at("users");

  return 0;
}
