EXTRA_CFLAGS += -Iarch/arm/mach-msm -Iarch/arm/tools

# Replace the vee3 board files that differ on the E400. The remaining vee3
# files (regulators, bt, camera, storage, wlan, display) are built from
# ../vee3 because this target also selects MACH_MSM7X25A_V3.
obj-$(CONFIG_MACH_MSM7X25A_E0) += board-msm7x27a_e0.o
obj-$(CONFIG_MACH_MSM7X25A_E0) += board-msm7x27a-sound_e0.o
obj-$(CONFIG_MACH_MSM7X25A_E0) += board-msm7627a-io_e0.o
