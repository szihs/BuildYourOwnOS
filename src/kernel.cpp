#include <common/types.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gdt.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <hardwarecomm/interrupts.h>
#include <hardwarecomm/pci.h>
#include <memorymanagement.h>
#include <multitasking.h>
#include <drivers/amd_am79c973.h>

#define SCREEN_W 80
#define SCREEN_H 25

using namespace os;
using namespace os::common;
using namespace os::drivers;
using namespace os::hardwarecomm;
using namespace os::gui;

#define GRAPHICS_MODE 0
/*
 #if 1
    VideoMemory[i] = (VideoMemory[i] & 0xFF00)| str[i];
#else
    c = (VideoMemory[i] & 0xFF00) >> 8;
    c = (c & 0xF << 4) | (c & 0xF0 >> 4);
    VideoMemory[i] = (c << 8)| str[i];
#endif
 */
void printf(const char *str) {
  static uint8_t x = 0, y = 0;
  uint16_t *VideoMemory = (uint16_t *)(0xb8000);
  for (uint32_t i = 0; str[i] != '\0'; i++) {

    switch (str[i]) {
    case '\n':
      x = 0;
      y++;
      break;
    default:
      VideoMemory[SCREEN_W * y + x] =
          (VideoMemory[SCREEN_W * y + x] & 0xFF00) | str[i];
      x++;
      break;
    }

    if (x >= SCREEN_W) {
      x = 0;
      y++;
    }
    if (y >= SCREEN_H) {
      for (y = 0; y < SCREEN_H; y++)
        for (x = 0; x < SCREEN_W; x++)
          VideoMemory[SCREEN_W * y + x] =
              (VideoMemory[SCREEN_W * y + x] & 0xFF00) | ' ';

      y = x = 0;
    }
  }
}

void printfHex(uint8_t key) {
  char foo[3] = "00";
  char hex[17] = "0123456789ABCDEF";
  foo[0] = hex[(key >> 4) & 0xF];
  foo[1] = hex[key & 0xF];
  printf(foo);
}

class PrintKeyboardEventHandler : public KeyboardEventHandler {
private:
  void doOnKeyDown(char c) {
    char foo[2] = " ";
    foo[0] = c;
    printf(foo);
  }
};

class MouseConsole : public MouseEventHandler {
private:
  int8_t x{40};
  int8_t y{12};

  void doOnMouseMove(int8_t xoffset, int8_t yoffset) {

    uint16_t *VideoMem = (uint16_t *)0xb8000;

    VideoMem[80 * y + x] = (((VideoMem[80 * y + x] & 0xF000) >> 4) |
                            ((VideoMem[80 * y + x] & 0x0F00) << 4) |
                            (VideoMem[80 * y + x] & 0x00FF));

    x += xoffset;
    if (x < 0)
      x = 0;
    if (x >= 80)
      x = 79;

    y += yoffset;
    if (y < 0)
      y = 0;
    if (y >= 25)
      y = 24;

    VideoMem[80 * y + x] = (((VideoMem[80 * y + x] & 0xF000) >> 4) |
                            ((VideoMem[80 * y + x] & 0x0F00) << 4) |
                            (VideoMem[80 * y + x] & 0x00FF));
  }

public:
  MouseConsole() {
    uint16_t *VideoMem = (uint16_t *)0xb8000;

    VideoMem[80 * y + x] = (((VideoMem[80 * y + x] & 0xF000) >> 4) |
                            ((VideoMem[80 * y + x] & 0x0F00) << 4) |
                            (VideoMem[80 * y + x] & 0x00FF));
  }
};

void taskA() {
  while (true)
    printf("Task A");
}

void taskB() {
  while (true)
    printf("Task B");
}
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors() {
  for (constructor *i = &start_ctors; i != &end_ctors; i++)
    (*i)();
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber) {
  printf("Hello ~ Refactored Custom Kernel\n");
  GlobalDescriptorTable gdt;
  TaskManager taskManager;
  size_t heap = 10 * 1024 * 1024;
  uint32_t *memupper = (uint32_t *)((size_t)multiboot_structure + 8);
  MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

  printf("heap: 0x");
  printfHex((heap >> 24) & 0xFF);
  printfHex((heap >> 16) & 0xFF);
  printfHex((heap >> 8) & 0xFF);
  printfHex((heap)&0xFF);

  void *allocated = memoryManager.malloc(1024);

  printf("\n allocated: 0x");
  printfHex(((size_t)allocated >> 24) & 0xFF);
  printfHex(((size_t)allocated >> 16) & 0xFF);
  printfHex(((size_t)allocated >> 8) & 0xFF);
  printfHex(((size_t)allocated) & 0xFF);
  printf("\n");

  /*
  Task task1(&gdt, taskA);
  Task task2(&gdt, taskB);
  taskManager.AddTask(&task1);
  taskManager.AddTask(&task2);
  */
  InterruptManager interrupts(&gdt, &taskManager);

  DriverManager drvManager;
  printf("Init Stage : 1\n");

#if !GRAPHICS_MODE
  PrintKeyboardEventHandler printKey;
  KeyboardDriver keyboard(&interrupts, &printKey);
#else
  Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
  KeyboardDriver keyboard(&interrupts, &desktop);
#endif
  drvManager.AddDriver(&keyboard);

#if !GRAPHICS_MODE
  MouseConsole mouseConsole;
  MouseDriver mouse(&interrupts, &mouseConsole);
#else
  MouseDriver mouse(&interrupts, &desktop);
#endif
  drvManager.AddDriver(&mouse);

  PeripheralComponentInterconnectController PCIController;
  PCIController.SelectDrivers(&drvManager, &interrupts);
  printf("Init Stage : 2\n");

  drvManager.ActivateAll();
  printf("Init Stage : 3\n");

#if GRAPHICSMODE
  VideoGraphicsArray vga;
  vga.SetMode(320, 200, 8);
  // vga.FillRectangle(0, 0, 320, 200, 0x0, 0x0, 0xA8);

  Window win1(&desktop, 10, 10, 15, 10, 0xA8, 0x0, 0x0);
  desktop.AddChild(&win1);
  Window win2(&desktop, 40, 15, 30, 30, 0x0, 0xA8, 0x0);
  desktop.AddChild(&win2);
  // desktop.MouseEventHandler::OnMouseDown(0x0);
#endif

 amd_am79c973 *eth0 = (amd_am79c973 *)drvManager.getDriver(2);
 eth0->Send((uint8_t *)"Hello Network", 13);

  interrupts.Activate();
  while (1) {
#if GRAPHICSMODE
    desktop.Draw(&vga);
#endif
  }
}
