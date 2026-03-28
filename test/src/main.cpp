#include <csql.h>

int main() {

  anythingsoup::csql csql;

  if (!csql.AddDatabase("credentials.db"))
    return -1;

  return 0;
}
