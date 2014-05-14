set(LLVM_LINK_COMPONENTS support bitwriter bitreader irreader transformutils)

add_llvm_tool(C0_llvm_preprocessor
  C0PreProc.cpp
  spot1.cpp
  )
