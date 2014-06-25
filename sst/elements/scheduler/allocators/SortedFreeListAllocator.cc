// Copyright 2009-2014 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2014, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

/**
 * Allocator that assigns the first available processors (according to
 * order specified when allocator is created).
 */
#include "sst_config.h"
#include "SortedFreeListAllocator.h"

#include <vector>
#include <string>
#include <stdio.h>

#include "AllocInfo.h"
#include "Job.h"
#include "LinearAllocator.h"
#include "Machine.h"
#include "MeshMachine.h"
#include "MeshAllocInfo.h"
#include "output.h"

using namespace SST::Scheduler;

SortedFreeListAllocator::SortedFreeListAllocator(std::vector<std::string>* params, Machine* mach) : LinearAllocator(params, mach)
{
    schedout.init("", 8, ~0, Output::STDOUT);
    //if (DEBUG) {
    //    printf("Constructing SortedFreeListAllocator\n");
    //}
    schedout.debug(CALL_INFO, 1, 0, "Constructing SortedFreeListAllocator\n");
    if (NULL == dynamic_cast<MeshMachine*>(mach)) {
        schedout.fatal(CALL_INFO, 1, "Linear allocators require a mesh");
    }
}

std::string SortedFreeListAllocator::getSetupInfo(bool comment)
{
    std::string com;
    if (comment)  {
        com = "# ";
    } else  {
        com = "";
    }
    return com + "Linear Allocator (Sorted Free List)";
}

//allocates j if possible
//returns information on the allocation or NULL if it wasn't possible
//(doesn't make allocation; merely returns info on possible allocation)
AllocInfo* SortedFreeListAllocator::allocate(Job* job) 
{
    MeshMachine* mMachine = static_cast<MeshMachine*>(machine);

    schedout.debug(CALL_INFO, 7, 0, "Allocating %s \n", job -> toString().c_str());

    if (!canAllocate(*job)) {
        return NULL;
    }

    std::vector<MeshLocation*>* freeprocs = ((MeshMachine*)machine) -> freeProcessors();
    stable_sort(freeprocs -> begin(), freeprocs -> end(), *ordering);

    int num = job -> getProcsNeeded();  //number of processors for job

    MeshAllocInfo* retVal = new MeshAllocInfo(job);
    for (int i = 0; i < (int)freeprocs -> size(); i++) {
        if (i < num) {
            retVal -> processors -> at(i) = freeprocs->at(i);
            retVal -> nodeIndices[i] = freeprocs -> at(i) -> toInt(*mMachine);
        } else {
            delete freeprocs -> at(i);
        }
    }
    delete freeprocs;
    return retVal;
}