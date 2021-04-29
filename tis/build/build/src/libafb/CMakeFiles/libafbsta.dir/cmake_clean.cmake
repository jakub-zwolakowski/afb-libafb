file(REMOVE_RECURSE
  "libafb.a"
  "libafb.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/libafbsta.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
