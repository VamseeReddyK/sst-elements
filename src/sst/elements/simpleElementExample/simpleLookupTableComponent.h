// Copyright 2009-2017 Sandia Corporation. Under the terms
// of Contract DE-NA0003525 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2017, Sandia Corporation
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef _SIMPLELOOKUPTABLECOMPONENT_H
#define _SIMPLELOOKUPTABLECOMPONENT_H

#include <sst/core/component.h>
#include <sst/core/elementinfo.h>
#include <sst/core/output.h>
#include <sst/core/sharedRegion.h>

namespace SST {
namespace SimpleElementExample {

class simpleLookupTableComponent : public SST::Component
{
public:

    // REGISTER THIS COMPONENT INTO THE ELEMENT LIBRARY
    SST_ELI_REGISTER_COMPONENT(
        simpleLookupTableComponent,
        "simpleElementExample",
        "simpleLookupTableComponent",
        SST_ELI_ELEMENT_VERSION(1,0,0),
        "Demonstrates using a Shared Lookup Table",
        COMPONENT_CATEGORY_UNCATEGORIZED
    )
    
    SST_ELI_DOCUMENT_PARAMS(
        { "filename", "Filename to load as the table", ""},
        {"num_entities", "Number of entities in the sim", "1"},
        {"myid", "ID Number (0 <= myid < num_entities)", "0"}
    )

    // Optional since there is nothing to document
    SST_ELI_DOCUMENT_STATISTICS(
    )

    // Optional since there is nothing to document
    SST_ELI_DOCUMENT_PORTS(
    )

    // Optional since there is nothing to document
    SST_ELI_DOCUMENT_SUBCOMPONENT_SLOTS(
    )

    simpleLookupTableComponent(SST::ComponentId_t id, SST::Params& params);
    ~simpleLookupTableComponent();

    virtual void init(unsigned int phase);
    virtual void setup();
    virtual void finish();

    bool tick(SST::Cycle_t);
private:
    Output out;
    const uint8_t * table;
    size_t tableSize;
    SharedRegion *sregion;
};

}
}

#endif
