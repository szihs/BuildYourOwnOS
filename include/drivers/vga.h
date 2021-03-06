#pragma once
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecomm/port.h>
namespace os {
namespace drivers {
class VideoGraphicsArray {
private:
  hardwarecomm::Port8Bit miscPort;
  hardwarecomm::Port8Bit crtcIndexPort;
  hardwarecomm::Port8Bit crtcDataPort;
  hardwarecomm::Port8Bit sequencerIndexPort;
  hardwarecomm::Port8Bit sequencerDataPort;
  hardwarecomm::Port8Bit graphicsControllerIndexPort;
  hardwarecomm::Port8Bit graphicsControllerDataPort;
  hardwarecomm::Port8Bit attributeControllerIndexPort;
  hardwarecomm::Port8Bit attributeControllerReadPort;
  hardwarecomm::Port8Bit attributeControllerWritePort;
  hardwarecomm::Port8Bit attributeControllerResetPort;
  common::uint8_t *fbSegmentAddr{nullptr};
  void WriteRegisters(common::uint8_t *registers);

  common::uint8_t *GetFrameBufferSegment();
  common::uint8_t GetColorIndex(common::uint8_t r, common::uint8_t g,
                                common::uint8_t b);
  bool SupportsMode(common::uint32_t width, common::uint32_t height,
                    common::uint32_t colordepth);
  bool PutPixel(common::uint32_t x, common::uint32_t y,
                common::uint8_t colorIndex);

public:
  VideoGraphicsArray();
  ~VideoGraphicsArray();

  bool SetMode(common::uint32_t width, common::uint32_t height,
               common::uint32_t colordepth);

  void PutPixel(common::uint32_t x, common::uint32_t y, common::uint8_t r,
                common::uint8_t g, common::uint8_t b);
  void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w,
                     common::uint32_t h, common::uint8_t r, common::uint8_t g,
                     common::uint8_t b);
};
} // namespace drivers
} // namespace os