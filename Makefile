##===- tools/C0PreProc/Makefile -----------------------*- Makefile -*-===##
# 
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
# 
##===----------------------------------------------------------------------===##
LEVEL := ../..
TOOLNAME := C0_llvm_preprocessor
REQUIRES_EH = 1

LINK_COMPONENTS := bitwriter bitreader irreader transformutils

# This tool has no plugins, optimize startup time.
TOOL_NO_EXPORTS := 1

include $(LEVEL)/Makefile.common
