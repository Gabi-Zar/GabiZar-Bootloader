#include "home.h"
#include <bootloader/boot.h>
#include <bootloader/slots/slot.h>
#include <bootloader/interface/menus/about.h>
#include <bootloader/interface/menus/slot_utils.h>
#include <stdlib.h>

Bootloader::AboutMenu * Bootloader::HomeMenu::aboutMenu() {
  static AboutMenu * aboutMenu = new AboutMenu();
  return aboutMenu;
}

Bootloader::InstallerMenu * Bootloader::HomeMenu::installerMenu() {
  static InstallerMenu * installerMenu = new InstallerMenu();
  return installerMenu;
}

Bootloader::ExamMenu * Bootloader::HomeMenu::examMenu() {
  static ExamMenu * examMenu = new ExamMenu();
  return examMenu;
}

Bootloader::HomeMenu::HomeMenu() : Menu(KDColorWhite, KDColorBlack, Messages::homeTitle, Messages::mainTitle) {
  setup();
}

bool slotA_submenu() {
  if (Bootloader::Slot::isFullyValid(Bootloader::Slot::A())) {
    Bootloader::Boot::bootSlot(Bootloader::Slot::A());
    return true;
  }
  return false;
}

bool slotKhi_submenu() {
  if (Bootloader::Slot::isFullyValid(Bootloader::Slot::Khi())) {
    Bootloader::Boot::bootSlot(Bootloader::Slot::Khi());
    return true;
  }
  return false;
}

bool slotB_submenu() {
  if (Bootloader::Slot::isFullyValid(Bootloader::Slot::B())) {
    Bootloader::Boot::bootSlot(Bootloader::Slot::B());
    return true;
  }
  return false;
}

bool installer_submenu() {
  Bootloader::HomeMenu::installerMenu()->open();
  return true;
}

bool about_submenu() {
  Bootloader::HomeMenu::aboutMenu()->open();
  return true;
}

bool exam_submenu() {
  Bootloader::HomeMenu::examMenu()->open();
  return true;
}

void Bootloader::HomeMenu::setup() {
  Slot A = Slot::A();
  Slot Khi = Slot::Khi();
  Slot B = Slot::B();

  m_slotColumns[0] = SlotColumn(getSlotText(A), getKernelText(A), getSlotOsText(A), getVersionText(A), "", Ion::Keyboard::Key::One, k_small_font, 10, false, &slotA_submenu);
  m_slotColumns[1] = SlotColumn(getSlotText(Khi), getKernelText(Khi), getSlotOsText(Khi), getVersionText(Khi), "", Ion::Keyboard::Key::Two, k_small_font, 10, false, &slotKhi_submenu);
  m_slotColumns[2] = SlotColumn(getSlotText(B), getKernelText(B), getSlotOsText(B), getVersionText(B), "", Ion::Keyboard::Key::Three, k_small_font, 10, false, &slotB_submenu);
  m_defaultColumns[0] = Column(Messages::homeInstallerSubmenu, Ion::Keyboard::Key::Four, k_small_font, 10, false, &installer_submenu);
  m_defaultColumns[1] = Column(Messages::homeAboutSubmenu, Ion::Keyboard::Key::Six, k_small_font, 10, false, &about_submenu);
  m_defaultColumns[2] = Column(Messages::homeExamSubmenu, Ion::Keyboard::Key::Five, k_small_font, 10, false, &exam_submenu);

  m_columns[0] = ColumnBinder(&m_slotColumns[0]);
  m_columns[1] = ColumnBinder(&m_slotColumns[1]);
  m_columns[2] = ColumnBinder(&m_slotColumns[2]);
  m_columns[3] = ColumnBinder(&m_defaultColumns[0]);
  m_columns[4] = ColumnBinder(&m_defaultColumns[2]);
  m_columns[5] = ColumnBinder(&m_defaultColumns[1]);
}
