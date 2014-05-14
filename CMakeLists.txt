set(LLVM_LINK_COMPONENTS support bitwriter bitreader irreader transformutils)
set(LLVM_REQUIRES_EH 1)

add_llvm_tool(C0_llvm_preprocessor
  C0PreProc.cpp
  spot1.cpp
  )
