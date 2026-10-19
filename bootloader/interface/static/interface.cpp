#include <assert.h>
#include <ion.h>
#include <ion/timing.h>
#include <bootloader/interface/static/interface.h>
#include <bootloader/interface/static/messages.h>

#include <bootloader/frame_1.h>
#include <bootloader/frame_2.h>
#include <bootloader/frame_3.h>


namespace Bootloader {

void drawImage(const Image * image, KDContext * ctx, int offset) {
  const uint8_t * data;
  size_t size;
  size_t pixelBufferSize;

  if (image != nullptr) {
    data = image->compressedPixelData();
    size = image->compressedPixelDataSize();
    pixelBufferSize = image->width() * image->height();
  } else {
    return;
  }

  KDColor pixelBuffer[2500];
  assert(pixelBufferSize <= 2500);
  assert(Ion::stackSafe()); // That's a VERY big buffer we're allocating on the stack

  Ion::decompress(
    data,
    reinterpret_cast<uint8_t *>(pixelBuffer),
    size,
    pixelBufferSize * sizeof(KDColor)
  );

  KDRect bounds((320 - image->width()) / 2, offset, image->width(), image->height());

  ctx->fillRectWithPixels(bounds, pixelBuffer, nullptr);
}

void Interface::drawComputer(KDContext * ctx, int offset) {
  const Image * frames[] = {
    ImageStore::Frame1,
    ImageStore::Frame2,
    ImageStore::Frame3,
  };

  for (uint8_t i = 0; i < 3; i++) {
    drawImage(frames[i], ctx, offset);
    Ion::Timing::msleep(50);
  }
}

KDCoordinate Interface::computerHeight() {
  return ImageStore::Frame1->height();
}

void Interface::drawFlasher() {
  KDContext * ctx = KDIonContext::sharedContext();

  ctx->fillRect(KDRect(0, 0, 320, 240), KDColorBlack);
  drawComputer(ctx, 25);

  KDSize fontSize = KDFont::LargeFont->glyphSize();
  int initPos = (320 - fontSize.width() * strlen(Messages::mainTitle)) / 2;
  ctx->drawString(Messages::mainTitle, KDPoint(initPos, ImageStore::Frame1->height() + 25 + 10), KDFont::LargeFont, KDColorWhite, KDColorBlack);

  int y = ImageStore::Frame1->height() + 25 + 10 + (KDFont::SmallFont->glyphSize().height() + 10);
  initPos = (320 - KDFont::SmallFont->glyphSize().width() * strlen(Messages::dfuSlotsUpdate)) / 2;
  ctx->drawString(Messages::dfuSlotsUpdate, KDPoint(initPos, y), KDFont::SmallFont, KDColorWhite, KDColorBlack);
}

void Interface::drawLoading() {
  const Image * frames[] = {
    ImageStore::Frame1,
    ImageStore::Frame2,
    ImageStore::Frame3,
  };
  KDContext * ctx = KDIonContext::sharedContext();
  ctx->fillRect(KDRect(0, 0, 320, 240), KDColorBlack);
  //drawComputer(ctx, 25);
  Ion::Timing::msleep(100);
  KDSize fontSize = KDFont::LargeFont->glyphSize();
  int initPos = (320 - fontSize.width() * strlen(Messages::mainTitle)) / 2;

  for (uint8_t i = 0; i < strlen(Messages::mainTitle); i++) {
    drawImage(frames[i % 3], ctx, 25);
    char tmp[2] = {Messages::mainTitle[i], '\0'};
    ctx->drawString(tmp, KDPoint(initPos + i * (fontSize.width()), ImageStore::Frame1->height() + 25 + 10), KDFont::LargeFont, KDColorWhite, KDColorBlack);

    int t = 50;
    Ion::Events::Event e = Ion::Events::getEvent(&t);
    if (e.isKeyboardEvent()){
      return;
    }
  }
  Ion::Timing::msleep(200);
}

}
