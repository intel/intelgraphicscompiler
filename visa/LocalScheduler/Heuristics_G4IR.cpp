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

#include "LocalScheduler_G4IR.h"
using namespace vISA;

void G4_BB_Schedule::printReadyList(NODE_LIST &ready_list)
{
	DEBUG_MSG("READY : ");
	NODE_LIST_ITER iN(ready_list.begin()), eNode(ready_list.end());
	for( ; iN != eNode; ++iN)
	{
	    DEBUG_EMIT((*iN)->getInstruction());
        DEBUG_MSG("");
	}
}

Node* G4_BB_Schedule::SendHeuristic(NODE_VECT &ready_list)
{
    uint32_t  earlist = 0xFFFFFFFF;
    Node *candidateNode = NULL;
	auto iNode(ready_list.begin()), endNode(ready_list.end());
	for( ; iNode != endNode; ++iNode)
	{
		G4_INST *inst = (*iNode)->getInstruction();
		if(inst->isSend())
		{
		    Node *curNode = (*iNode);
		    if (curNode->earliest < earlist)
		    {
		        earlist = curNode->earliest;
		        candidateNode = curNode;
		    }
		}
	}

	return candidateNode;
}

// Node* G4_BB_Schedule::TornDefUseHeuristic(NODE_VECT &ready_list)
// {

// 	Node *to_schedule = NULL;	/* A node to be scheduled */

// 	// send is preferred
// 	if((to_schedule = SendHeuristic(ready_list)))
// 	{
// 		return to_schedule;
// 	}
// 	// Now we have to choose an instruction that has the maximum distance between
// 	// the current position and the closest definition that is in the schedule.
// 	// NOTE: There can be many instructions that have the same distance => we need a tie breaker,
// 	// which'd be one of the following heuristics (to be added...).
// 	uint16_t maxDUDiff = 0xffff;
// 	auto niter(ready_list.begin()), nend(ready_list.end());
// 	for( ; niter != nend; ++niter)
// 	{
// 		if( (*niter)->getClosestDefPos() < maxDUDiff)
// 		{
// 			maxDUDiff   = (*niter)->getClosestDefPos();
// 			to_schedule = *niter;
// 		}
// 	}

// 	if(to_schedule == NULL)
// 	{
// 		// FIXME : Choosing the first one from the ready list (have to break the tie)
// 		to_schedule = ready_list.front();
// 	}

// 	return to_schedule;
// }

// This one just pops a random node out of the ready list.
Node* G4_BB_Schedule::RandomHeuristic(NODE_LIST &ready_list)
{
	int range_max = ((int) ready_list.size()) - 1;
	int randomNum = (int) (((double) rand() / (double) RAND_MAX) * range_max);

	NODE_LIST_ITER iNode = ready_list.begin();
	for(int i = 0; i < randomNum; i++, ++iNode);

	return *iNode;
}
