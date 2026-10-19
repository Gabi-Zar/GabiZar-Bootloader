#include "exam.h"
#include <bootloader/interface/static/messages.h>
#include <bootloader/slots/slot.h>
#include <bootloader/slots/slot_exam_mode.h>
#include <bootloader/interface/menus/slot_utils.h>

#include <ion/src/device/shared/drivers/external_flash.h>
#include <stddef.h>
#include <stdint.h>

Bootloader::ExamMenu::ExamMenu() : Menu(KDColorWhite, KDColorBlack, Messages::examMenuTitle, Messages::mainTitle) {
  setup();
}

/* Écrit des zéros dans [start, end), en adresses memory-mapped
 * (0x90000000 -> 0x90800000). Retourne false si la plage est invalide. */
bool zeroExternalFlash(uint32_t start, uint32_t end) {
  constexpr uint32_t k_base = 0x90000000;
  constexpr size_t k_chunk = 4096;
  if (start < k_base || end > k_base + Ion::Device::ExternalFlash::FlashAddressSpaceSize || start >= end) {
    return false;
  }

  // La source doit être en RAM (ou en flash interne), jamais en flash externe.
  static uint8_t zeros[k_chunk];
  for (size_t i = 0; i < k_chunk; i++) {
    zeros[i] = 0;
  }

  uint32_t address = start;
  while (address < end) {
    uint32_t remaining = end - address;
    size_t length = remaining < k_chunk ? remaining : k_chunk;
    Ion::Device::ExternalFlash::WriteMemory(reinterpret_cast<uint8_t *>(address), zeros, length);
    address += length;
  }
  return true;
}

bool slotA_exam_submenu() {
  if (Bootloader::Slot::isFullyValid(Bootloader::Slot::A())) {
    const char * version = Bootloader::Slot::A().userlandHeader()->version();
    uint32_t start = 0;
    uint32_t end = 0;
    
    if (version[0] < '1' || (version[0] == '1' && version[1] < '6')) {
      start = Bootloader::ExamMode::SlotsExamMode::getSlotAStartExamAddress(0);
      end = Bootloader::ExamMode::SlotsExamMode::getSlotAEndExamAddress(0);
    }
    // Else versions before 22
    else if ((version[0] == '2' && version[1] < '2') || (version[0] == '1')) {
      start = Bootloader::ExamMode::SlotsExamMode::getSlotAStartExamAddress(1);
      end = Bootloader::ExamMode::SlotsExamMode::getSlotAEndExamAddress(1);
    }
    // Else Epsilon 22 +
    else {
      start = Bootloader::ExamMode::SlotsExamMode::getSlotAStartExamAddress(2);
      end = Bootloader::ExamMode::SlotsExamMode::getSlotAEndExamAddress(2);
    }
    
    zeroExternalFlash(start, end);
    return true;
  }
  return false;
}

bool slotKhi_exam_submenu() {
  if (Bootloader::Slot::isFullyValid(Bootloader::Slot::Khi())) {
    uint32_t start = Bootloader::ExamMode::SlotsExamMode::getSlotKhiStartExamAddress();
    uint32_t end = Bootloader::ExamMode::SlotsExamMode::getSlotKhiEndExamAddress();

    zeroExternalFlash(start, end);
    return true;
  }
  return false;
}

bool slotB_exam_submenu() {
  if (Bootloader::Slot::isFullyValid(Bootloader::Slot::B())) {
    const char * version = Bootloader::Slot::B().userlandHeader()->version();
    uint32_t start = 0;
    uint32_t end = 0;

    if (version[0] < '1' || (version[0] == '1' && version[1] < '6')) {
      start = Bootloader::ExamMode::SlotsExamMode::getSlotBStartExamAddress(0);
      end = Bootloader::ExamMode::SlotsExamMode::getSlotBEndExamAddress(0);
    }
    // Else versions before 22
    else if ((version[0] == '2' && version[1] < '2') || (version[0] == '1')) {
      start = Bootloader::ExamMode::SlotsExamMode::getSlotBStartExamAddress(1);
      end = Bootloader::ExamMode::SlotsExamMode::getSlotBEndExamAddress(1);
    }
    // Else Epsilon 22 +
    else {
      start = Bootloader::ExamMode::SlotsExamMode::getSlotBStartExamAddress(2);
      end = Bootloader::ExamMode::SlotsExamMode::getSlotBEndExamAddress(2);
    }

    zeroExternalFlash(start, end);
    return true;
  }
  return false;
}

void Bootloader::ExamMenu::setup() {
  Slot A = Slot::A();
  Slot Khi = Slot::Khi();
  Slot B = Slot::B();

  m_slotColumns[0] = SlotColumn(getSlotText(A), "", getSlotOsText(A), "", getExamState(A),  Ion::Keyboard::Key::One, k_small_font, 10, false, &slotA_exam_submenu);
  m_slotColumns[1] = SlotColumn(getSlotText(Khi), "", getSlotOsText(Khi), "", getExamState(Khi), Ion::Keyboard::Key::Two, k_small_font, 10, false, &slotKhi_exam_submenu);
  m_slotColumns[2] = SlotColumn(getSlotText(B), "", getSlotOsText(B), "", getExamState(B), Ion::Keyboard::Key::Three, k_small_font, 10, false, &slotB_exam_submenu);
  
  m_columns[0] = ColumnBinder(&m_slotColumns[0]);
  m_columns[1] = ColumnBinder(&m_slotColumns[1]);
  m_columns[2] = ColumnBinder(&m_slotColumns[2]);
}
