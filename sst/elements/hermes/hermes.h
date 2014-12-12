// Copyright 2013-2014 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2013-2014, Sandia Corporation
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef _H_HERMES
#define _H_HERMES

#include <sst/core/module.h>
#include <sst/core/component.h>

#include "functor.h"

using namespace SST;

namespace SST {

namespace Hermes {

class Interface : public Module {
    public:
    virtual void _componentInit(unsigned int phase ) {}
    virtual void _componentSetup( void ) {}
	virtual void printStatus( Output& ) {}
};

}

}

#endif
