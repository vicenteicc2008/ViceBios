/** @file
  This BIOS utility bases on LvglLib.

  Copyright (c) 2025, Yang Gang. All rights reserved.<BR>
  Copyright (c) 2025, viZPilot. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __VIZ_BIOS_COMMON_H__
#define __VIZ_BIOS_COMMON_H__

#include <Uefi.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/PlatformLogo.h>
#include <Protocol/Smbios.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>

#include <Library/LvglLib.h>


VOID
UefiBasicInfo (
  lv_obj_t * parent
  );

VOID
UefiBootOptions (
  lv_obj_t * parent
  );

VOID
UefiTime (
  lv_obj_t * parent
  );

#endif