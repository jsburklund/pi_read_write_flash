#include <bitset>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace std::chrono_literals;

uint32_t getFLSELMask(uint pin) {
  return 0b111 << (pin % 10) * 3;
}

uint32_t setFLSELValue(uint pin, uint8_t flsel_value) {
  return (flsel_value & 0b111) << (pin % 10) * 3;
}

const std::string kGPIOMemoryFilename{"/dev/gpiomem"};
int main() {

  // Open the GPIO specific memory.
  const int gpio_memory_file =
       open(kGPIOMemoryFilename.c_str(), O_RDWR | O_DSYNC | O_SYNC);
  
  if (gpio_memory_file < 0) {
    std::cerr << "Could not open: " << kGPIOMemoryFilename << std::endl;
    return 1;
  }

  void* gpio_memory =
      mmap(/* address */ nullptr, /* length */ 4096,
           /* prot */ PROT_READ | PROT_WRITE,
           /* flags */ MAP_SHARED, /* fd */ gpio_memory_file, /* offset */ 0);

  if (gpio_memory == MAP_FAILED) {
    std::cerr << "Could not memory map the GPIO memory region" << std::endl;
    return 1;
  }

  std::cout << "SUCCESS!!!!" << std::endl;

  // Extract the FSEL values.
  const uint32_t gpflsel0 = *((uint32_t*) gpio_memory);
  const uint32_t gpflsel1 = *((uint32_t*) (gpio_memory + 0x04));
  std::cout << "GPFLSEL0: " << std::bitset<32>(gpflsel0) << std::endl;
  std::cout << "GPFLSEL1: " << std::bitset<32>(gpflsel1) << std::endl;

  {
    const uint32_t gplev0 = *((uint32_t*) (gpio_memory + 0x0034));
    const uint32_t gplev1 = *((uint32_t*) (gpio_memory + 0x0038));
    std::cout << "GPLEV0: " << std::bitset<32>(gplev0) << std::endl;
    std::cout << "GPLEV1: " << std::bitset<32>(gplev1) << std::endl;
  }

  // Set all of the 8 io pins to output mode.
  const uint32_t gpflsel0_mask =
      getFLSELMask(2) | getFLSELMask(3) | getFLSELMask(4) |
      getFLSELMask(7) | getFLSELMask(8) | getFLSELMask(9);
  const uint32_t gpflsel0_update =
      (gpflsel0 & ~gpflsel0_mask) |
      setFLSELValue(2, 0b1) | setFLSELValue(3, 0b1) | setFLSELValue(4, 0b1) |
      setFLSELValue(7, 0b1) | setFLSELValue(8, 0b1) | setFLSELValue(9, 0b1);
  std::cout << "GPFLSEL0: " << std::bitset<32>(gpflsel0) << std::endl;
  std::cout << "mask:     " << std::bitset<32>(gpflsel0_mask) << std::endl;
  std::cout << "value:    " << std::bitset<32>(gpflsel0_update) << std::endl;
  // Write the configuration.
  *((uint32_t*) (gpio_memory)) = gpflsel0_update;
  {
    const uint32_t gpflsel0_new = *((uint32_t*) gpio_memory);
    std::cout << "GPFLSEL0: " << std::bitset<32>(gpflsel0_new) << std::endl;
  }

  const uint32_t gpflsel1_mask = getFLSELMask(10) | getFLSELMask(11);
  const uint32_t gpflsel1_update =
      (gpflsel1 & ~gpflsel1_mask) |
      setFLSELValue(10, 0b1) | setFLSELValue(11, 0b1);
  std::cout << "GPFLSEL1: " << std::bitset<32>(gpflsel1) << std::endl;
  std::cout << "mask:     " << std::bitset<32>(gpflsel1_mask) << std::endl;
  std::cout << "value:    " << std::bitset<32>(gpflsel1_update) << std::endl;
  // Write the configuration.
  *((uint32_t*) (gpio_memory + 0x04)) = gpflsel1_update;
  {
    const uint32_t gpflsel1_new = *((uint32_t*) (gpio_memory + 0x04));
    std::cout << "GPFLSEL1: " << std::bitset<32>(gpflsel1_new) << std::endl;
  }

  // Clear the pullups?
  {

  }

  // Try to blink the leds?
  {
    const uint32_t leds =
      0b1 << 2 | 0b1 << 3 | 0b1 << 4 | 0b1 << 7 | 0b1 << 8 | 0b1 << 9 | 0b1 << 10 | 0b1 << 11;
    for (int i=0; i < 5; ++i) {
      // set
      *((uint32_t*) (gpio_memory + 0x001C)) = leds;
      std::this_thread::sleep_for(400ms);
      // clear
      *((uint32_t*) (gpio_memory + 0x0028)) = leds;
      std::this_thread::sleep_for(400ms);
    }
  }



  const int munmap_result = munmap(gpio_memory, 4096);
  if (munmap_result) {
    std::cerr << "Could not unmap GPIO memory mappped region" << std::endl;
  }

  const int close_result = close(gpio_memory_file);
  if (close_result) {
    std::cerr << "Could not close GPIO memory file" << std::endl;
  }

  return 0;
}