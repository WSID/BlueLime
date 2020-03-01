# Appendix

# Outline of this file.
#
# 1. TDlib build.
#    Instructions are generally taken from https://tdlib.github.io/td/build.html, for C++, Linux build.
.PHONY : td_native_build td_build

$(OBJS) : td_build

OUT_PATH = $(PROJ_PATH)/$(BUILD_CONFIG)

VCPKG_PATH = $(OUT_PATH)/vcpkg

TD_SRC_PATH = $(PROJ_PATH)/td
TD_STAGE_PATH = $(OUT_PATH)/td_stage
TD_BUILD_PATH = $(OUT_PATH)/td
TD_NATIVE_BUILD_PATH = $(OUT_PATH)/td_native

TD_DEST_PATH = $(OUTPUT_DIR)/tdlib
TD_CMAKE_ENVS = \
  CC=$(CC) \
  CFLAGS='$(C_OPT)' \
  CXX=$(CXX) \
  CXXFLAGS='$(CPP_OPT)'

TD_CMAKE_OPTS = \
  -DTIZEN_ARCH=$(ARCH) \
  -DTIZEN_SYSROOT=$(SBI_SYSROOT) \
  -DTIZEN_SDK_TOOLPATH=$(SDK_TOOLPATH) \
  -DTIZEN_TD_STAGE_PATH=$(TD_STAGE_PATH) \
  -DCMAKE_TOOLCHAIN_FILE="$(BUILD_ROOT)/crosscompile.cmake" \
  -DCMAKE_AR=$(AR) \
  -DCMAKE_MAKE_PROGRAM="$(MAKE)" \
  -DCMAKE_BUILD_TYPE=$(BUILD_CONFIG) \
  -DCMAKE_INSTALL_PREFIX=$(TD_DEST_PATH) \
  -DTD_ENABLE_LTO=ON

# Overall:
# Cross-compilation of tdlib requires, certain target built on native
# version of tdlib. ("prepare_cross_compiling" target).
#
# This will build tdlib twice.
# One for native version. (Only for prepare_cross_compiling target)
# The other for tizen runtime.

# Windows Section
ifeq ($(OS), Windows_NT)

TD_CMAKE_OPTS += \
  -DCMAKE_GENERATOR="MSYS Makefiles"

$(VCPKG_PATH)/vcpkg.exe :
	@echo ======================================================
	@echo Setting up vcpkg 
	-cd $(OUT_PATH) ; git clone https://github.com/Microsoft/vcpkg.git
	cd $(VCPKG_PATH) ; ./bootstrap-vcpkg.bat

$(VCPKG_PATH)/packages/openssl_x64-windows/ : $(VCPKG_PATH)/vcpkg.exe
	@echo ======================================================
	@echo Setting up openSSL in vcpkg
	cd $(VCPKG_PATH) ; ./vcpkg.exe install openssl:x64-windows

$(VCPKG_PATH)/packages/zlib_x64-windows/ : $(VCPKG_PATH)/vcpkg.exe
	@echo ======================================================
	@echo Setting up openSSL in vcpkg
	cd $(VCPKG_PATH) ; ./vcpkg.exe install zlib:x64-windows

td_native_build : $(VCPKG_PATH)/packages/openssl_x64-windows/ $(VCPKG_PATH)/packages/zlib_x64-windows/
	@echo ======================================================
	@echo Building TDLib native build on $(TD_NATIVE_BUILD_PATH)
	$(MKDIR) $(MKDIR_OP) $(TD_NATIVE_BUILD_PATH)
	cd $(TD_NATIVE_BUILD_PATH) ; cmake -A x64 -DCMAKE_TOOLCHAIN_FILE:FILEPATH=../vcpkg/scripts/buildsystems/vcpkg.cmake $(TD_SRC_PATH)
	cd $(TD_NATIVE_BUILD_PATH) ; cmake --build . --target prepare_cross_compiling --config $(BUILD_CONFIG)

endif # Windows Section


# Linux Section
ifeq ($(shell uname -s), Linux)

td_native_build:
	@echo ======================================================
	@echo Building TDLib native build on $(TD_NATIVE_BUILD_PATH)
	$(MKDIR) $(MKDIR_OP) $(TD_NATIVE_BUILD_PATH)
	cd $(TD_NATIVE_BUILD_PATH) ; cmake $(TD_SRC_PATH)
	cd $(TD_NATIVE_BUILD_PATH) ; cmake --build . --target prepare_cross_compiling --config $(BUILD_CONFIG)

endif # Linux Section


# TODO: Let it built on Mac.


td_build : td_native_build
	@echo ==========================
	@echo Making tdlib staging prefix on $(TD_STAGE_PATH)
	$(MKDIR) $(MKDIR_OP) $(TD_STAGE_PATH)
	
	@echo Building tdlib on $(TD_BUILD_PATH)
	$(MKDIR) $(MKDIR_OP) $(TD_BUILD_PATH)
	cd $(TD_BUILD_PATH) ; $(TD_CMAKE_ENVS) cmake $(TD_CMAKE_OPTS) $(TD_SRC_PATH)
	cd $(TD_BUILD_PATH) ; cmake --build . --target install
