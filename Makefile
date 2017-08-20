VULKAN_SDK_PATH = ../VulkanSDK/1.0.57.0/x86_64

CFLAGS = -std=c++14 -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
SOURCES = $(wildcard *.cpp)
EXECUTABLE = Vulkan

VulkanTest: ${SOURCES}
	g++ $(CFLAGS) -o ${EXECUTABLE} ${SOURCES} $(LDFLAGS)

.PHONY: test clean

test: ${EXECUTABLE}
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/explicit_layer.d ./${EXECUTABLE}

clean:
	rm -f ${EXECUTABLE}