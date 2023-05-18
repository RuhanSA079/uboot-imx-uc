/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2019 NXP
 * Copyright 2020-2022 Variscite Ltd.
 */

#ifndef __IMX8MP_VAR_DART_H
#define __IMX8MP_VAR_DART_H

#include <linux/sizes.h>
#include <linux/stringify.h>
#include <asm/arch/imx-regs.h>

#include "imx_env.h"

#define CONFIG_SYS_BOOTM_LEN		(64 * SZ_1M)

#define CONFIG_SPL_MAX_SIZE				(176 * 1024)
#define CONFIG_SYS_MONITOR_LEN				(512 * 1024)
#define CONFIG_SYS_UBOOT_BASE				(QSPI0_AMBA_BASE + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_STACK				0x96dff0
#define CONFIG_SPL_BSS_START_ADDR			0x96e000
#define CONFIG_SPL_BSS_MAX_SIZE				SZ_8K   /* 8 KB */

#define CONFIG_SYS_SPL_MALLOC_START			0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE			SZ_512K	/* 512 KB */

/* For RAW image gives a error info not panic */
#define CONFIG_SPL_ABORT_ON_RAW_IMAGE

#define CONFIG_POWER_PCA9450

#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_SPEED		100000
#endif

#define CONFIG_CMD_READ
#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV		0

#define CONFIG_REMAKE_ELF

/* ENET Config */
#if defined(CONFIG_CMD_NET)
#define CONFIG_ETHPRIME			"eth0" /* Set eqos to primary since we use its MDIO */
#define CONFIG_FEC_XCV_TYPE		RGMII
#define PHY_ANEG_TIMEOUT		20000
#endif

#include <config_distro_bootcmd.h>
/* redefine BOOTENV_EFI_SET_FDTFILE_FALLBACK to use Variscite function to load fdt */
#undef BOOTENV_EFI_SET_FDTFILE_FALLBACK
#define BOOTENV_EFI_SET_FDTFILE_FALLBACK \
	"setenv efi_dtb_prefixes; " \
	"run loadfdt; "
#else
#define BOOTENV
#endif

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x43800000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	"emmc_dev=2\0"\
	"sd_dev=1\0" \

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"bootcmd=run load_uc\0" \
	"fitloadaddr=0x45000000\0" \
	"loadfiles=load ${devtype} ${mmcdev}:${kernel_bootpart} ${fitloadaddr} ${kernel_prefix}/${kernel_filename}\0" \
	"kernel_filename=kernel.img\0" \
	"initrd_filename=initrd.img\0" \
	"core_state=/uboot/ubuntu/boot.sel\0" \
	"kernel_vars=snap_kernel snap_try_kernel kernel_status\0" \
	"recovery_vars=snapd_recovery_mode snapd_recovery_system snapd_recovery_kernel\0" \
	"snapd_recovery_mode=install\0" \
	"snapd_standard_params=panic=-1 systemd.gpt_auto=0 rd.systemd.unit=basic.target\0" \
	"boot_uc=run load_uc; bootm ${fitloadaddr}#conf-0\0" \
	"mmc_seed_part=1\0" \
	"mmc_boot_part=2\0" \
	"devtype=mmc\0" \
	"console=ttymxc0,115200\0" \
	"mmcdev=1\0" \
	"mmcpart=1\0" \
	"load_uc=" \
      		"setenv kernel_bootpart ${mmc_seed_part};"\
      		"load ${devtype} ${mmcdev}:${kernel_bootpart} 0x40480000 ${core_state};" \
      		"env import -v -c 0x40480000 ${filesize} ${recovery_vars};" \
      		"if test \"${snapd_recovery_mode}\" = \"run\"; then " \
		  "setenv bootargs \"fde_helper=enabled console=${console} snapd_recovery_mode=${snapd_recovery_mode} ${snapd_standard_params}\";" \
		  "setenv kernel_bootpart ${mmc_boot_part}; " \
		  "load ${devtype} ${mmcdev}:${kernel_bootpart} 0x40480000 ${core_state}; " \
		  "env import -v -c 0x40480000 ${filesize} ${kernel_vars}; " \
		  "setenv kernel_name ${snap_kernel}; " \
		  "if test -n \"${kernel_status}\"; then " \
		    "if test \"${kernel_status}\" = \"try\"; then " \
		      "if test -n \"${snap_try_kernel}\"; then " \
		        "setenv kernel_status trying; " \
			"setenv kernel_name \"${snap_try_kernel}\"; " \
            	      "fi; " \
                    "elif test \"${kernel_status}\" = \"trying\"; then " \
                      "setenv kernel_status \"\"; " \
                    "fi;" \
          	    "env export -c 0x40480000 ${kernel_vars}; " \
		    "save ${devtype} ${mmcdev}:${kernel_bootpart} 0x40480000 ${core_state} ${filesize}; " \
		  "fi; " \
		  "setenv kernel_prefix \"/uboot/ubuntu/${kernel_name}/\"; " \
		"else " \
		  "setenv bootargs \"fde_helper=enabled console=${console} snapd_recovery_mode=${snapd_recovery_mode} snapd_recovery_system=${snapd_recovery_system} ${snapd_standard_params}\";" \
		  "setenv kernel_prefix \"/systems/${snapd_recovery_system}/kernel/\"; " \
		"fi; " \
		"run loadfiles\0" \
	
	
/* Link Definitions */

#define CONFIG_SYS_INIT_RAM_ADDR	0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE	0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* DDR configs */
#define CONFIG_SYS_SDRAM_BASE		0x40000000
#define PHYS_SDRAM			0x40000000
#define DEFAULT_SDRAM_SIZE		(512 * SZ_1M)

#define CONFIG_MXC_UART_BASE		UART1_BASE_ADDR

/* Monitor Command Prompt */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_IMX_BOOTAUX

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR	0

/* USB configs */

#define CONFIG_USB_MAX_CONTROLLER_COUNT		2
#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_VBUS_DRAW		2

#ifdef CONFIG_DM_VIDEO
#define CONFIG_VIDEO_LOGO
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_BMP_24BPP
#define CONFIG_BMP_32BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#endif

/* EEPROM configs */
#define VAR_EEPROM_DRAM_START	(PHYS_SDRAM + (DEFAULT_SDRAM_SIZE >> 1))

#if defined(CONFIG_ANDROID_SUPPORT)
#include "imx8mp_var_dart_android.h"
#endif

#endif
