set(MODULE TRUE)

set( LLVM_USED_LIBS
  clangFrontend
  clangAST
  )

set( LLVM_LINK_COMPONENTS support mc)

add_clang_library(FindUninstantiatedTemplates FindUninstantiatedTemplates.cpp)

set_target_properties(FindUninstantiatedTemplates
  PROPERTIES
  LINKER_LANGUAGE CXX
  PREFIX "")
