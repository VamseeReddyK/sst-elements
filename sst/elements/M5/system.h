#ifndef _system_h
#define _system_h

#include <sim/system.hh>

namespace SST {
namespace M5 {

static inline System* create_System( std::string name, 
                    PhysicalMemory* physmem,
                    Enums::MemoryMode mem_mode) 
{
    
    System::Params& params = *new System::Params;
    
    params.name = name;
    params.physmem = physmem;
    params.mem_mode = mem_mode;
    params.memories.resize(1);
    params.memories[0] = physmem;

    return new System( &params );
}

}
}

#endif
