Workspace = {
  name = "csql-wks",
}

Project = {
  name = "csql",
  kind = "StaticLib",
  language = "C++",
  dialect = "23",


  files = {
    "src/*.cpp",
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
    "test/src/*.cpp",
  },

  includedirs = {
    "include"
  },

  links = {
    "csql"
  }
}
