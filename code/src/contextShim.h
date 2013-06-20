#ifndef __CONTEXT_SHIM_H__
#define __CONTEXT_SHIM_H__

#include "contextHandler.h"
#include "contextSummarySerializer.h"

class ContextShim {
    ContextHandler* h;
    ContextSummarySerializer* s;
public:
    ContextShim()
    {
        s = new ContextSummarySerializer();
        h = ContextHandler::getInstance();
    }
    
    ~ContextShim()
    {
        delete s;
    }
    
    ContextHandler* getContextHandlerPtr() {return h;}
    
    /* from the summaries from handler
     * get the serialized data stored in result buffer
     */
    vector<unsigned char> getContextBytes()
    {
        vector<ContextSummary*> summaries = h->getSummariesToSend();
        s->clearBuffer();
        vector<unsigned char> result = s->writeSummaries(summaries);
        return result;
    }
    
    /* assume that the buffer is filled, 
     * it transforms the buffer content into summaries
     */
    vector<unique_ptr<ContextSummary>> processContextBytes() // const vector<unsigned char>& buffer)
    {
        vector<unique_ptr<ContextSummary>> summaries = s->readSummaries();
        return summaries;
    }
};

    // def processContextBytes(self, buffer):
    //     """
    //     When it processes ContextBytes, it uses handleIncomingSummaries,
    //     it causes to increment hops by 1
    //     
    //     Traceback (most recent call last):
    //       method test_setprocessContextBytes in testContextShim.py at line 61
    //         summaries = self.c.processContextBytes(res)
    //       method processContextBytes in testContextShim.py at line 20
    //         self.contextHandler.handleIncomingSummaries(summaries)
    //       method handleIncomingSummaries in testContextShim.py at line 150
    //         summary.incrementHops()
    //       method incrementHops in testContextShim.py at line 64
    //         raise Exception("WHY???")
    //     """
    //     summaries = self.serializer.readSummaries(buffer)
    //     self.contextHandler.handleIncomingSummaries(summaries)
    //     return summaries
#endif