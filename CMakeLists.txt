set(LLVM_LINK_COMPONENTS bitwriter bitreader transformutils)

add_llvm_example(C0PreProc
  C0PreProc.cpp
  spot1.cpp
  )
