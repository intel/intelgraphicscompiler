#=========================== begin_copyright_notice ============================
#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#
#============================ end_copyright_notice =============================

add_executable(CMCLTranslatorTool Main.cpp)

# LLVM dependencies
igc_get_llvm_targets(LLVM_LIBS
  Core
  Support
  IRReader
# Not really required but VS linker fails on its dependency from Support
  Demangle
  BitWriter)

target_link_libraries(CMCLTranslatorTool
  CMCLSupport
  ${LLVM_LIBS})
