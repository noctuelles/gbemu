//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_IDEBUGGER_HXX
#define GBEMU_IDEBUGGER_HXX

class IDebugger
{
  public:
    virtual ~IDebugger() = default;

    virtual void onInstructionFetched()  = 0;
    virtual void onInstructionExecuted() = 0;
};

#endif  // GBEMU_IDEBUGGER_HXX
