Clang: Find Uninstantiated Templates
====================================

These are the first steps towards uninstantiated templates showing up as
non-executed code in llvm's GCOV.  Classic GCC GCOV will ignore the uninstantiated
function template "foo":

    template <typename T>
    foo() {}
    
    template <typename T>
    bar() {}
    
    int main() {
      bar<int>();
      return 0;
    }

This is a clang plugin that finds such uninstantiated templates.

The next step is to embed meta data of this in the generated LLVM intermediate
language and finally embed calls to the coverage runtime library for such
empty blocks.

Usage
-----

Check this out into the "examples" directory of clang, add it to the CMakeLists.txt
and then simply rebuild clang.

You can use the plugin like this:

    clang++ -cc1 -load FindUninstantiatedTemplates.so -plugin find-uitpl tpl_finder_target.cpp
