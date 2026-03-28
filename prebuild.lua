Workspace = {
  name = "csql-wks",
}

Project = {
  name = "csql",
  kind = "StaticLib",
  language = "C++",
  dialect = "23",

  pch = "csqlpch.h",

  files = {
    "src/*.c",
  },

  includedirs = {
    "include/",
  },

  links = {
    "sqlite3",
  },
}

Project = {
  name = "test",
  kind = "ConsoleApp",
  language = "C++",
  dialect = "23",

  files = {
    "test/src/*.c",
  },

  includedirs = {
    "include"
  },

  links = {
    "csql"
  }
}
