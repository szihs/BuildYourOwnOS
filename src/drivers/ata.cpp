#include <drivers/ata.h>
using namespace os::common;
using namespace os::drivers;

void printf(const char *str);
void printfHex(uint8_t key); 
AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(uint16_t portBase,
                                                           bool master)
    : dataPort(portBase), errorPort(portBase + 1), sectorCountPort(portBase),
      lbaLowPort(portBase + 3), lbaMidPort(portBase + 4),
      lbaHiPort(portBase + 5), devicePort(portBase + 6),
      commandPort(portBase + 7), controlPort(portBase + 0x206) {
  bytesPerSector = 512;
  this->master = master;
}
AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment() {}

void AdvancedTechnologyAttachment::Identify() {
    devicePort.Write(master ? 0xA0: 0xB0);
    controlPort.Write(0);

    devicePort.Write(0xA0);//read status of master
    uint8_t status = commandPort.Read();

    if (status == 0xFF) return;//no device on bus

    devicePort.Write(master ? 0xA0 : 0xB0);
    sectorCountPort.Write(0);
    lbaLowPort.Write(0);
    lbaMidPort.Write(0);
    lbaHiPort.Write(0);


    commandPort.Write(0xEC);//Identify cmd
    status = commandPort.Read();

    if (status == 0x00)
    return;//no device

    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
            status = commandPort.Read();

    if (status & 0x01) {
        printf("ERROR\n");
        return;
    }

    for (uint16_t i = 0; i < 256; i++) {
        uint16_t data = dataPort.Read();
        char foo[3];
        foo[1] = ((data >> 8) &  0x00FF);
        foo[0] = data & 0x00FF;
        foo[2]=  '\0';
        printf(foo);
     }
}
void AdvancedTechnologyAttachment::Read28(uint32_t sector) {}
void AdvancedTechnologyAttachment::Write28(uint32_t sector, uint8_t *data,
                                           uint32_t count) {}
void AdvancedTechnologyAttachment::Flush() {}