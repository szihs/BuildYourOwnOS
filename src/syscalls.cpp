#include <syscalls.h>
using namespace os;
using namespace os::common;
using namespace os::hardwarecomm;
void printf(const char *str);
void printfHex(uint8_t key);

SyscallHandler::SyscallHandler(uint8_t interruptNumber,
                               InterruptManager *interruptManager)
    : InterruptHandler(interruptNumber, interruptManager) {}
SyscallHandler::~SyscallHandler() {}

uint32_t SyscallHandler::doHandleInterrupt(uint32_t esp) {
  CPUState *cpustate = (CPUState *)esp;

  switch (cpustate->eax) {
  case 4:
    printf((const char *)cpustate->ebx);
    break;
  default:
    break;
  }

  return esp;
}
