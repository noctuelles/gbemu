//
// Created by plouvel on 20/11/2025.
//

#ifndef GBEMU_EMULATIONSTATE_HXX
#define GBEMU_EMULATIONSTATE_HXX

/**
 * @struct EmulationState
 * @brief Represents the state of the emulation, particularly to track operations such as OAM DMA activity.
 *
 * The structure contains information about the emulation's active state and is used across
 * various components such as the CPU (SM83) and the memory bus (Bus) to ensure correct behavior
 * during specific emulation operations.
 *
 * isInOamDma is primarily used to indicate whether an OAM DMA transfer is currently in progress.
 * This state affects how components interact with memory during the emulation process.
 */
struct EmulationState
{
    bool isInOamDma;
};

#endif  // GBEMU_EMULATIONSTATE_HXX
