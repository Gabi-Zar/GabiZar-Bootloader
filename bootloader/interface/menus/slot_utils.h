#ifndef _BOOTLOADER_INTERFACE_MENUS_SLOT_UTILS_H_
#define _BOOTLOADER_INTERFACE_MENUS_SLOT_UTILS_H_

#include <bootloader/slots/slot.h>

namespace Bootloader {
  const char * getSlotOsText(Slot slot);
  const char * getSlotText(Slot slot);
  const char * getKernelText(Slot slot);
  const char * getVersionText(Slot slot);
  const char * getExamState(Slot slot);
}

#endif
