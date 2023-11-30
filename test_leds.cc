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

volatile uint32_t* GPFSEL0Register(volatile void* gpio_memory) {
  return static_cast<volatile uint32_t*>(gpio_memory);
}

volatile uint32_t* GPFSEL1Register(volatile void* gpio_memory) {
  return static_cast<volatile uint32_t*>(gpio_memory + 0x04);
}

volatile uint32_t* GPFSEL2Register(volatile void* gpio_memory) {
  return static_cast<volatile uint32_t*>(gpio_memory + 0x08);
}

uint32_t getFSELMask(uint pin) {
  return 0b111 << (pin % 10) * 3;
}

uint32_t setFSELValue(uint pin, uint8_t fsel_value) {
  return (fsel_value & 0b111) << (pin % 10) * 3;
}

// TODO does this need memory barriers, or volatile?
void setAllPinsFSELValue(volatile void* gpio_memory, uint8_t fsel_value) {
  // Setup GPLSEL1.
  const uint32_t gpfsel1_curr = *GPFSEL1Register(gpio_memory);
  const uint32_t gpfsel1_mask = getFSELMask(11) | getFSELMask(17) | getFSELMask(18);
  const uint32_t gpfsel1_update =
      (gpfsel1_curr & ~gpfsel1_mask) |
      setFSELValue(11, fsel_value) | setFSELValue(17, fsel_value) | setFSELValue(18, fsel_value);
  std::cout << "GPFSEL1: " << std::bitset<32>(gpfsel1_curr) << std::endl;
  std::cout << "mask:    " << std::bitset<32>(gpfsel1_mask) << std::endl;
  std::cout << "value:   " << std::bitset<32>(gpfsel1_update) << std::endl;
  
  // Write the configuration.
  *GPFSEL1Register(gpio_memory) = gpfsel1_update;
  const uint32_t gpfsel1_after = *GPFSEL1Register(gpio_memory);
  std::cout << "after:   " << std::bitset<32>(gpfsel1_after) << std::endl;
  
  // Setup GPFSEL2.
  const uint32_t gpfsel2_curr = *GPFSEL2Register(gpio_memory);
  const uint32_t gpfsel2_mask =
      getFSELMask(22) | getFSELMask(23) | getFSELMask(24) | getFSELMask(25) | getFSELMask(27);
  const uint32_t gpfsel2_update =
      (gpfsel2_curr & ~gpfsel2_mask) |
      setFSELValue(22, fsel_value) | setFSELValue(23, fsel_value) | setFSELValue(24, fsel_value) |
      setFSELValue(25, fsel_value) | setFSELValue(27, fsel_value);
  std::cout << "GPFSEL2: " << std::bitset<32>(gpfsel2_curr) << std::endl;
  std::cout << "mask:    " << std::bitset<32>(gpfsel2_mask) << std::endl;
  std::cout << "value:   " << std::bitset<32>(gpfsel2_update) << std::endl;
  
  // Write the configuration.
  *GPFSEL2Register(gpio_memory) = gpfsel2_update;
  const uint32_t gpfsel2_after = *GPFSEL2Register(gpio_memory);
  std::cout << "after:   " << std::bitset<32>(gpfsel2_after) << std::endl;
}

void setAllPinsOutput(volatile void* gpio_memory) {
  setAllPinsFSELValue(gpio_memory, 0b001);
}

void setAllPinsInput(volatile void* gpio_memory) {
  setAllPinsFSELValue(gpio_memory, 0b000);
}

// TODO are the sleep times sufficient, or does this need busywait?
void setNoPinPulls(volatile void* gpio_memory) {
  // All inputs pull up/down disabled.
  const uint32_t gppud_value = 0b00;
  const uint32_t gppud_clk0_value =
      0b1 << 11 | 0b1 << 17 | 0b1 << 18 | 0b1 << 22 |
      0b1 << 23 | 0b1 << 24 | 0b1 << 25 | 0b1 << 27;

  // Set GPPUD.
  *((uint32_t*) (gpio_memory + 0x94)) = gppud_value;
  std::this_thread::sleep_for(150ns);
  // Set GPPUDCLK0.
  *((uint32_t*) (gpio_memory + 0x98)) = gppud_clk0_value;
  std::this_thread::sleep_for(150ns);
  // Clear the clock.
  *((uint32_t*) (gpio_memory + 0x94)) = gppud_value;
  *((uint32_t*) (gpio_memory + 0x98)) = gppud_clk0_value;
}

uint32_t getMaskValue(uint8_t value, uint8_t bit, uint8_t pin) {
  return ((value >> bit) & 0b1) << pin;
}

volatile uint32_t* GPSET0Register(volatile void* gpio_memory) {
  return static_cast<volatile uint32_t*>(gpio_memory + 0x1c);
}

volatile uint32_t* GPCLR0Register(volatile void* gpio_memory) {
  return static_cast<volatile uint32_t*>(gpio_memory + 0x28);
}

void setPinsToValue(volatile void* gpio_memory, uint8_t value) {
  const uint32_t set_mask =
    getMaskValue(value, 0, 11) | getMaskValue(value, 1, 17) |
    getMaskValue(value, 2, 18) | getMaskValue(value, 3, 22) |
    getMaskValue(value, 4, 23) | getMaskValue(value, 5, 24) |
    getMaskValue(value, 6, 25) | getMaskValue(value, 7, 27);
  const uint32_t clear_mask =
    getMaskValue(~value, 0, 11) | getMaskValue(~value, 1, 17) |
    getMaskValue(~value, 2, 18) | getMaskValue(~value, 3, 22) |
    getMaskValue(~value, 4, 23) | getMaskValue(~value, 5, 24) |
    getMaskValue(~value, 6, 25) | getMaskValue(~value, 7, 27);
  *GPSET0Register(gpio_memory) = set_mask;
  *GPCLR0Register(gpio_memory) = clear_mask;
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

  volatile void* gpio_memory =
      mmap(/* address */ nullptr, /* length */ 4096,
           /* prot */ PROT_READ | PROT_WRITE,
           /* flags */ MAP_SHARED, /* fd */ gpio_memory_file, /* offset */ 0);

  if (gpio_memory == MAP_FAILED) {
    std::cerr << "Could not memory map the GPIO memory region" << std::endl;
    return 1;
  }

  std::cout << "SUCCESS!!!!" << std::endl;
  {
    const uint32_t gplev0 = *((uint32_t*) (gpio_memory + 0x0034));
    const uint32_t gplev1 = *((uint32_t*) (gpio_memory + 0x0038));
    std::cout << "GPLEV0: " << std::bitset<32>(gplev0) << std::endl;
    std::cout << "GPLEV1: " << std::bitset<32>(gplev1) << std::endl;
  }

  // Try to blink the leds?
  setAllPinsOutput(gpio_memory);

  setPinsToValue(gpio_memory, 0x00);
  std::this_thread::sleep_for(2s);

  setPinsToValue(gpio_memory, 0x55);
  std::this_thread::sleep_for(2s);
  setPinsToValue(gpio_memory, 0xAA);
  std::this_thread::sleep_for(2s);

  setPinsToValue(gpio_memory, 0x0f);
  std::this_thread::sleep_for(2s);
  setPinsToValue(gpio_memory, 0xf0);
  std::this_thread::sleep_for(2s);

  setPinsToValue(gpio_memory, 0x11);
  std::this_thread::sleep_for(1s);
  setPinsToValue(gpio_memory, 0x22);
  std::this_thread::sleep_for(1s);
  setPinsToValue(gpio_memory, 0x44);
  std::this_thread::sleep_for(1s);
  setPinsToValue(gpio_memory, 0x88);
  std::this_thread::sleep_for(1s);

  for (int i=0; i <= 255; ++i) {
    setPinsToValue(gpio_memory, static_cast<uint8_t>(i));
    std::this_thread::sleep_for(200ms);
  }
  setPinsToValue(gpio_memory, 0x00);

  // Set all pins back to input?
  setAllPinsInput(gpio_memory);

  std::this_thread::sleep_for(2s);
  setNoPinPulls(gpio_memory);


  const int munmap_result = munmap(const_cast<void*>(gpio_memory), 4096);
  if (munmap_result) {
    std::cerr << "Could not unmap GPIO memory mappped region" << std::endl;
  }

  const int close_result = close(gpio_memory_file);
  if (close_result) {
    std::cerr << "Could not close GPIO memory file" << std::endl;
  }

  return 0;
}
