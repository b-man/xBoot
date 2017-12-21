##
# Common variables used for building xBoot
##

##
# Architecture and platform variables (set in bsp/<platform>/bsp.mk)
#
# ARCH			- architecture to compile for
# SUBARCH		- subarchitecture to compile for
# PLATFORM		- platform target for xBoot
# DRAM_BASE		- start of physical memory
# DRAM_SIZE		- size of physical memory
# BOOT_TEXT_BASE	- platform xBoot entry point address
# PLAT_DT_FILE		- platform device tree file

ARCH		:=
SUBARCH		:=
PLATFORM	:=
DRAM_BASE	:=
DRAM_SIZE	:=
BOOT_TEXT_BASE	:=
PLAT_DT_FILE	:=

##
# Common toolchain variables (set in make/tools.mk)
#
# CC		- C compiler to be used for building xBoot
# LD		- linker to be used while building xBoot
# AS		- assembler to be used while building xBoot
# AR		- archiver to be used for pre-linking stage of xBoot build
# OBJDMP	- object dumper to bed used while building xBoot
# OBJCPY	- object copier to be used while building xBoot
# HOST_CC	- compiler to use for building tools that are ran on the host
# CROSS		- prefix for toolchain to be used while building xBoot
# DTC		- device tree compiler to be used for building xBoot
# ARCHIVE	- archival tool for the first stage of linking xBoot
# VERSION	- timestamp tool for labeling the build date of xBoot
# IMAGE3MAKER	- archival tool for generating IMG3 containers

CC		:=
LD		:=
AS		:=
AR		:=
OBJDMP		:=
OBJCPY		:=
HOST_CC		:=
CROSS		:=
DTC		:=
ARCHIVE		:=
VERSION		:=
IMAGE3MAKER	:=

##
# Common compiler, linker, and assembler flags  (set in make/flags.mk)
#
# CFLAGS	- all compiler flags to be passed while building xBoot
# CFLAGS_ARGS	- compiler arguments to be passed while building xBoot
# CFLAGS_DEFS	- compiler define flags to be passed while building xBoot
# CFLAGS_WARNS	- compiler warning flags to be passed while building xBoot
# CFLAGS_INCS	- compiler include flags to be passed while building xBoot
#
# SFLAGS	- all assembler flags to be passed while building xBoot
# SFLAGS_ARGS	- assembler arguments to be passed while building xBoot
# SFLAGS_DEFS	- assembler define flags to be passed while building xBoot
# SFLAGS_WARNS	- assembler warning flags to be passed while building xBoot
# SFLAGS_INCS	- assembler include flags to be passed while building xBoot
#
# LDFLAGS	- all linker flags to be passed while building xBoot

CFLAGS		:=
CFLAGS_ARGS	:=
CFLAGS_DEFS	:=
CFLAGS_WARNS	:=
CFLAGS_INCS	:=
SFLAGS		:=
SFLAGS_ARGS	:=
SFLAGS_DEFS	:=
SFLAGS_WARNS	:=
SFLAGS_INCS	:=
LDFLAGS		:=

##
# Common internal toolchain variables (set in make/tools.mk)
#

_CC		:=
_LD		:=
_AS		:=
_AR		:=
_OBJDMP		:=
_OBJCPY		:=

##
# Common internal variables for build tree (set in make/build.mk)
#
# BUILD_DIR	- common build directory to be used for
#		  containing compiled objects and generated sources
# BUILD_ROOT	- points to BUILD_DIR
# BUILD_OBJS	- directory to contain compiled objects within BUILD_ROOT
# BUILD_GEN	- directory to contain generated source files within BUILD_ROOT

BUILD_DIR	:=
BUILD_ROOT	:=
BUILD_OBJS	:=
BUILD_GEN	:=

##
# Common internal variables for source files and directory tree
# (DIRS set in make/dirs.mk, OBJS set in build.mk, (C|S)_SRCS set in
# various module.mk(s) throughout the source tree)
#
# DIRS		- all directories to be included in xBoot build (source tree)
# C_SRCS	- all C source code to be included in xBoot build
# S_SRCS	- all ASM (.S) source code to be included in xBoot build

DIRS	:=
OBJS	:=
C_SRCS	:=
S_SRCS	:=
