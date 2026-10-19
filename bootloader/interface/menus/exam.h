#ifndef _BOOTLOADER_INTERFACE_EXAM_H_
#define _BOOTLOADER_INTERFACE_EXAM_H_

#include <bootloader/interface/src/menu.h>

namespace Bootloader {
  class ExamMenu : public Menu {
    public:
      ExamMenu();
    
      void setup() override;
      void postOpen() override {};
  };
}


#endif
