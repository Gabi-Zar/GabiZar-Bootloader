#include "slot_utils.h"
#include <bootloader/boot.h>
#include <bootloader/interface/static/messages.h>
#include <bootloader/slots/slot_exam_mode.h>

namespace Bootloader {

static const char* GetSlotLetter(uint32_t address) {
  switch (address) {
    case 0x90000000: return "A";
    case 0x90400000: return "B";
    case 0x90180000: return "Khi";
    default:         return "Unknown";
  }
}


const char * getSlotOsText(Slot slot) {
  if (Slot::isFullyValid(slot)) {
    if (slot.userlandHeader()->isOmega() && slot.userlandHeader()->isUpsilon()) {
      return Messages::upsilonSlot;
    } else if (slot.userlandHeader()->isOmega() && slot.kernelHeader()->patchLevel()[0] != '\0') {
      return Messages::omegaSlot;
    } else if (slot.userlandHeader()->isOmega()) {
      return Messages::khiSlot;
    } else {
      return Messages::epsilonSlot;
    }
  }
  return nullptr;
}

const char * getSlotText(Slot slot) {
  if (Slot::isFullyValid(slot)) {
    if (slot.address() == Slot::A().address()) {
      return Messages::homeSlotASubmenu;
    } else if (slot.address() == Slot::Khi().address()) {
      return Messages::homeSlotKhiSubmenu;
    } else if (slot.address() == Slot::B().address()) {
      return Messages::homeSlotBSubmenu;
    }
  }
  return Messages::invalidSlot;
}

const char * getKernelText(Slot slot) {
  return Slot::isFullyValid(slot) ? slot.kernelHeader()->patchLevel() : nullptr;
}

const char * getVersionText(Slot slot) {
  return Slot::isFullyValid(slot) ? slot.userlandHeader()->isOmega() && slot.userlandHeader()->isUpsilon() ? slot.userlandHeader()->upsilonVersion() : slot.userlandHeader()->isOmega() ? slot.userlandHeader()->omegaVersion() : slot.kernelHeader()->version() : nullptr;
}

const char * getExamState(Slot slot) {
  return Slot::isFullyValid(slot) && (Bootloader::ExamMode::ExamMode)Bootloader::ExamMode::SlotsExamMode::FetchSlotExamMode(slot.userlandHeader()->version(), GetSlotLetter(slot.address())) != Bootloader::ExamMode::ExamMode::Off && (Bootloader::ExamMode::ExamMode)Bootloader::ExamMode::SlotsExamMode::FetchSlotExamMode(slot.userlandHeader()->version(), GetSlotLetter(slot.address())) != Bootloader::ExamMode::ExamMode::Unknown  ? "Exam: On" : "Exam: Off";
}

}
