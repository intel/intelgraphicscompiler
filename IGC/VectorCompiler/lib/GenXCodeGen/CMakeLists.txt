#=========================== begin_copyright_notice ============================
#
# Copyright (C) 2017-2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#
#============================ end_copyright_notice =============================

add_subdirectory(TargetInfo)
add_subdirectory(Utils)

set(LLVM_TARGET_DEFINITIONS GenX.td)
tablegen(LLVM GenXGenSubtargetInfo.inc -gen-subtarget)
add_public_tablegen_target(GenXCommonTableGen)

set(GenXCommonTableGen_dependencies)
list(APPEND GenXCommonTableGen_dependencies
  GenXCommonTableGen)

add_custom_target(GenXCommonTableGen_target DEPENDS ${GenXCommonTableGen_dependencies})

set(CODEGEN_SOURCES
  FunctionGroup.cpp
  KillAnalysis.cpp
  GenXAddressCommoning.cpp
  GenXAggregatePseudoLowering.cpp
  GenXAlignmentInfo.cpp
  GenXAnalysisDumper.cpp
  GenXArgIndirection.cpp
  GenXBaling.cpp
  GenXCategory.cpp
  GenXCFSimplification.cpp
  GenXCisaBuilder.cpp
  GenXConstants.cpp
  GenXCoalescing.cpp
  GenXDeadVectorRemoval.cpp
  GenXDebugInfo.cpp
  GenXDepressurizer.cpp
  GenXExtractVectorizer.cpp
  GenXFuncPtrsLowering.cpp
  GenXGotoJoin.cpp
  GenXGEPLowering.cpp
  GenXGlobalValueLowering.cpp
  GenXIMadPostLegalization.cpp
  GenXInlineAsmLowering.cpp
  GenXIntrinsics.cpp
  GenXLayoutBlocks.cpp
  GenXLegalization.cpp
  GenXLiveRanges.cpp
  GenXLiveness.cpp
  GenXLowering.cpp
  GenXLowerAggrCopies.cpp
  GenXLowerJmpTableSwitch.cpp
  GenXEmulate.cpp
  GenXModule.cpp
  GenXNumbering.cpp
  GenXVectorCombiner.cpp
  GenXOCLInfoExtractor.cpp
  GenXOCLRuntimeInfo.cpp
  GenXPatternMatch.cpp
  GenXPostLegalization.cpp
  GenXPrinter.cpp
  GenXPressureTracker.cpp
  GenXPrologEpilogInsertion.cpp
  GenXPromoteArray.cpp
  GenXThreadPrivateMemory.cpp
  GenXPromotePredicate.cpp
  GenXRawSendRipper.cpp
  GenXReduceIntSize.cpp
  GenXInstCombineCleanup.cpp
  GenXRegion.cpp
  GenXRegionCollapsing.cpp
  GenXRematerialization.cpp
  GenXSimdCFConformance.cpp
  GenXSubtarget.cpp
  GenXTargetMachine.cpp
  GenXTidyControlFlow.cpp
  GenXUnbaling.cpp
  GenXUtil.cpp
  GenXVectorDecomposer.cpp
  GenXVisaRegAlloc.cpp
  GenXVisa.cpp
)

add_library(VCCodeGen ${CODEGEN_SOURCES})
add_dependencies(VCCodeGen
  GenXUtilBuild
  GenXCommonTableGen_target
  )
target_include_directories(VCCodeGen
  PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  )

igc_get_llvm_targets(LLVM_LIBS
  Analysis
  BitReader
  Core
  CodeGen
  Demangle
  InstCombine
  Linker
  Support
  Target
  TransformUtils
  ScalarOpts
  ipo
  )

target_link_libraries(VCCodeGen
  ${LLVM_LIBS}

  LLVMGenXIntrinsics

  VCHeaders
  VCTransforms
  VCTargetInfo
  VCSupport
  VCGeneralUtils
  VCGenXUtils
  GenXDebugInfo
  GenX_IR
  )
