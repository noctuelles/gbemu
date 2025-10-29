//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_IDEBUGGER_HXX
#define GBEMU_IDEBUGGER_HXX

template <typename CPUView>
class IDebugger
{
  public:
    virtual ~IDebugger() = default;

    virtual void onCpuInitialization(CPUView view)      = 0;
    virtual void onCpuInstructionFetched(CPUView view)  = 0;
    virtual void onCpuInstructionExecuted(CPUView view) = 0;
};

#endif  // GBEMU_IDEBUGGER_HXX
