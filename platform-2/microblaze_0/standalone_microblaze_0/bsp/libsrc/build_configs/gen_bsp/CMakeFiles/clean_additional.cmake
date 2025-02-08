# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "C:\\Users\\ishan\\internalandddr\\internalandddr.vitis\\platform-2\\microblaze_0\\standalone_microblaze_0\\bsp\\include\\sleep.h"
  "C:\\Users\\ishan\\internalandddr\\internalandddr.vitis\\platform-2\\microblaze_0\\standalone_microblaze_0\\bsp\\include\\xiltimer.h"
  "C:\\Users\\ishan\\internalandddr\\internalandddr.vitis\\platform-2\\microblaze_0\\standalone_microblaze_0\\bsp\\include\\xtimer_config.h"
  "C:\\Users\\ishan\\internalandddr\\internalandddr.vitis\\platform-2\\microblaze_0\\standalone_microblaze_0\\bsp\\lib\\libxiltimer.a"
  )
endif()
