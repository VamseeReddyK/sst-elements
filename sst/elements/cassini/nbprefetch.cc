
#include <vector>

#include "sst_config.h"
#include "sst/core/serialization.h"
#include "sst/core/element.h"

#include "nbprefetch.h"
#include <stdint.h>

using namespace SST;
using namespace SST::Interfaces;
using namespace SST::MemHierarchy;
using namespace SST::Cassini;

NextBlockPrefetcher::NextBlockPrefetcher(Params& params) {
	blockSize = (uint64_t) params.find_integer("prefetcher:blocksize", 64);
	prefetchEventsIssued = 0;
	missEventsProcessed = 0;
	hitEventsProcessed = 0;
}

NextBlockPrefetcher::~NextBlockPrefetcher() {

}

void NextBlockPrefetcher::notifyAccess(NotifyAccessType notifyType, NotifyResultType notifyResType, Addr addr)
{
	if(notifyResType == MISS) {
		missEventsProcessed++;

		Addr nextBlockAddr = (addr - (addr % blockSize)) + blockSize;
		std::vector<Event::HandlerBase*>::iterator callbackItr;
		prefetchEventsIssued++;

		// Cycle over each registered call back and notify them that we want to issue a prefetch request
		for(callbackItr = registeredCallbacks.begin(); callbackItr != registeredCallbacks.end(); callbackItr++) {
			// Create a new read request, we cannot issue a write because the data will get
			// overwritten and corrupt memory (even if we really do want to do a write)
			MemEvent* newEv = new MemEvent(owner, nextBlockAddr, RequestData);
            		newEv->setSize(blockSize);
			(*(*callbackItr))(newEv);
		}
	} else {
		hitEventsProcessed++;
	}
}

void NextBlockPrefetcher::registerResponseCallback(Event::HandlerBase *handler)  
{
	registeredCallbacks.push_back(handler);
}

void NextBlockPrefetcher::setOwningComponent(const SST::Component* own) 
{
	owner = own;
}

void NextBlockPrefetcher::printStats(Output &out) {
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Next Block Prefetch Engine:" << std::endl;
	std::cout << "Cache Miss Events:         " << missEventsProcessed << std::endl;
	std::cout << "Cache Hit Events:          " << hitEventsProcessed << std::endl;
	std::cout << "Cache Miss Rate (%):       " << ((missEventsProcessed / ((double) (missEventsProcessed + hitEventsProcessed))) * 100.0) << std::endl;
	std::cout << "Cache Hit Rate (%):        " << ((hitEventsProcessed / ((double) (missEventsProcessed + hitEventsProcessed))) * 100.0) << std::endl;
	std::cout << "Prefetches Issued:         " << prefetchEventsIssued << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
}
