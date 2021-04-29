file(REMOVE_RECURSE
  "libafb.pdb"
  "libafb.so"
  "libafb.so.4"
  "libafb.so.4.0"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/libafbso.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
