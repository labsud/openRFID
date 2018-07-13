//
// Created by elektron on 22/06/18.
//
// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <driver/sdmmc_types.h>
#include "vfs_api.h"

extern "C" {
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "sdmmc_cmd.h"
}
#include "ff.h"
#include "SD_MMC.h"

using namespace fs;
/*
*/

SDMMCFS::SDMMCFS(FSImplPtr impl)
        : FS(impl), _card(NULL)
{}

bool SDMMCFS::begin(const char * mountpoint, bool mode1bit)
{
    if(_card) {
        return true;
    }
    //mount
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    //host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
    host.max_freq_khz = SDMMC_FREQ_PROBING;
#ifdef BOARD_HAS_1BIT_SDMMC
    mode1bit = true;
#endif
    if(mode1bit) {
        host.flags = SDMMC_HOST_FLAG_1BIT; //use 1-line SD mode
    }

    host.flags = SDMMC_HOST_FLAG_1BIT;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = false,
            .max_files = 5
    };

    esp_err_t ret = esp_vfs_fat_sdmmc_mount(mountpoint, &host, &slot_config, &mount_config, &_card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            log_e("Failed to mount filesystem. If you want the card to be formatted, set format_if_mount_failed = true.");
        } else if (ret == ESP_ERR_INVALID_STATE) {
            _impl->mountpoint(mountpoint);
            log_w("SD Already mounted");
            return true;
        } else {
            log_e("Failed to initialize the card (%d). Make sure SD card lines have pull-up resistors in place.", ret);
        }
        _card = NULL;
        return false;
    }
    _impl->mountpoint(mountpoint);
    return true;
}

void SDMMCFS::end()
{
    if(_card) {
        esp_vfs_fat_sdmmc_unmount();
        _impl->mountpoint(NULL);
        _card = NULL;
    }
}

sdcard_type_t SDMMCFS::cardType()
{
    if(!_card) {
        return CARD_NONE;
    }
    return (_card->ocr & SD_OCR_SDHC_CAP)?CARD_SDHC:CARD_SD;
}

uint64_t SDMMCFS::cardSize()
{
    if(!_card) {
        return 0;
    }
    return (uint64_t)_card->csd.capacity * _card->csd.sector_size;
}

uint64_t SDMMCFS::totalBytes()
{
    FATFS* fsinfo;
    DWORD fre_clust;
    if(f_getfree("0:",&fre_clust,&fsinfo)!= 0) return 0;
    uint64_t size = ((uint64_t)(fsinfo->csize))*(fsinfo->n_fatent - 2)
                    #if _MAX_SS != 512
                    *(fsinfo->ssize);
#else
    *512;
#endif
    return size;
}

uint64_t SDMMCFS::usedBytes()
{
    FATFS* fsinfo;
    DWORD fre_clust;
    if(f_getfree("0:",&fre_clust,&fsinfo)!= 0) return 0;
    uint64_t size = ((uint64_t)(fsinfo->csize))*((fsinfo->n_fatent - 2) - (fsinfo->free_clst))
                    #if _MAX_SS != 512
                    *(fsinfo->ssize);
#else
    *512;
#endif
    return size;
}

SDMMCFS SD_MMC = SDMMCFS(FSImplPtr(new VFSImpl()));