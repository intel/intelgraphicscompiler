/*===================== begin_copyright_notice ==================================

Copyright (c) 2017 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


======================= end_copyright_notice ==================================*/

#include "AdaptorCommon/ImplicitArgs.hpp"
#include "Compiler/Optimizer/OpenCLPasses/OpenCLPrintf/OpenCLPrintfAnalysis.hpp"
#include "Compiler/IGCPassSupport.h"

#include "common/LLVMWarningsPush.hpp"
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/ADT/StringRef.h>
#include "common/LLVMWarningsPop.hpp"

using namespace llvm;
using namespace IGC;
using namespace IGC::IGCMD;

// Register pass to igc-opt
#define PASS_FLAG "igc-opencl-printf-analysis"
#define PASS_DESCRIPTION "Analyzes OpenCL printf calls"
#define PASS_CFG_ONLY false
#define PASS_ANALYSIS false
IGC_INITIALIZE_PASS_BEGIN(OpenCLPrintfAnalysis, PASS_FLAG, PASS_DESCRIPTION, PASS_CFG_ONLY, PASS_ANALYSIS)
IGC_INITIALIZE_PASS_DEPENDENCY(MetaDataUtilsWrapper)
IGC_INITIALIZE_PASS_END(OpenCLPrintfAnalysis, PASS_FLAG, PASS_DESCRIPTION, PASS_CFG_ONLY, PASS_ANALYSIS)

char OpenCLPrintfAnalysis::ID = 0;

OpenCLPrintfAnalysis::OpenCLPrintfAnalysis() : ModulePass(ID)
{
    initializeOpenCLPrintfAnalysisPass(*PassRegistry::getPassRegistry());
}

const StringRef OpenCLPrintfAnalysis::OPENCL_PRINTF_FUNCTION_NAME = "printf";

bool OpenCLPrintfAnalysis::runOnModule(Module &M)
{
    m_hasPrintf = false;

    visit(M);

    if (m_hasPrintf)
    {
        for (Function& func : M.getFunctionList())
        {
            if (!func.isDeclaration())
            {
                addPrintfBufferArgs(func);
            }
        }
    }

    return m_hasPrintf;
}

void OpenCLPrintfAnalysis::visitCallInst(llvm::CallInst &callInst)
{
    if (!callInst.getCalledFunction())
    {
        return;
    }

    StringRef  funcName = callInst.getCalledFunction()->getName();
    m_hasPrintf |= (funcName == OpenCLPrintfAnalysis::OPENCL_PRINTF_FUNCTION_NAME);
}

void OpenCLPrintfAnalysis::addPrintfBufferArgs(Function &F)
{
    SmallVector<ImplicitArg::ArgType, 1> implicitArgs;
    MetaDataUtils *pMdUtils = getAnalysis<MetaDataUtilsWrapper>().getMetaDataUtils();
    implicitArgs.push_back(ImplicitArg::PRINTF_BUFFER);
    ImplicitArgs::addImplicitArgs(F, implicitArgs, pMdUtils);
}
