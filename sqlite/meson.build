cc = meson.get_compiler('c')
lib_dir = meson.current_source_dir() + '/Win64'
libs = []
libs += cc.find_library('sqlite3', dirs: lib_dir)

sqlite3_dep = declare_dependency(
  dependencies: libs,
  )
