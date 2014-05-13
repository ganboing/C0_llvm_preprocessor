##===- examples/C0PreProc/Makefile -----------------------*- Makefile -*-===##
# 
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
# 
##===----------------------------------------------------------------------===##
LEVEL=../..
TOOLNAME=C0PreProc
EXAMPLE_TOOL = 1
LINK_COMPONENTS := bitwriter bitreader irreader transformutils

include $(LEVEL)/Makefile.common
