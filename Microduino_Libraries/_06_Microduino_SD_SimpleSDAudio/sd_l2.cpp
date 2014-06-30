
#include "sd_l0.h"
#include "sd_l1.h"
#include "sd_l2.h"

// #include "dbg_uart.h"
#include <stdio.h>

uint8_t  *SD_L2_workBuf;
SD_L2_FAT_t SD_L2_FAT;

// This both will share same buffer -> NextCluster not public availible!
uint8_t  *SD_L2_FatCache;
uint32_t SD_L2_LastFatSect;

uint8_t     SD_L2_NextCluster(uint32_t *cluster);

/**
 * Returns the first sector of a given cluster
 */
uint32_t SD_L2_Cluster2Sector(uint32_t cluster)
{
    return((cluster << SD_L2_FAT.ClusterSizeShift) + SD_L2_FAT.DataStart);
}

/** 
 * Returns next cluster of a given cluster
 *
 * Param Pointer to cluster number (will be swapped against next cluster)
 *
 * return Zero if successful, error code otherwise
 */
uint8_t SD_L2_NextCluster(uint32_t *cluster)
{
    uint32_t sector;
    uint16_t offset;
    uint8_t  retval;
    
    // calculate sector where cluster information can be found
    if(SD_L2_FAT.PartType == SD_L2_PARTTYPE_FAT16) {
        sector = (*cluster >> 8) + SD_L2_FAT.FatStart;
        offset = (uint16_t)(*cluster & 0xff) << 1;
    } else if(SD_L2_FAT.PartType == SD_L2_PARTTYPE_FAT32) {
        sector = (*cluster >> 7) + SD_L2_FAT.FatStart;
        offset = (uint16_t)(*cluster & 0x7f) << 2;
    } else {
        return(SD_L2_ERROR_FAT_NOT_INIT);
    }
    
    // Read FAT sector if not already buffered
    if(sector != SD_L2_LastFatSect) {
        retval = SD_L1_ReadBlock(sector, SD_L2_FatCache);
        if(retval) return(retval);
        SD_L2_LastFatSect = sector;
    }
    
    // Get FAT entry
    if(SD_L2_FAT.PartType == SD_L2_PARTTYPE_FAT32) {
        *cluster = ((uint32_t)SD_L2_FatCache[offset+3] << 24U) | ((uint32_t)SD_L2_FatCache[offset+2] << 16U);
    } else {
        *cluster = 0;
    }
    *cluster += ((uint32_t)SD_L2_FatCache[offset+1] << 8U) | ((uint32_t)SD_L2_FatCache[offset]);
    
    return(0);
}

/**
 * Test if file is completely not fragmented.
 *
 * return Zero if successful (=not fragmented), error code otherwise
 */
uint8_t SD_L2_IsFileFragmented(SD_L2_File_t *fileinfo)
{
    uint32_t startCluster = fileinfo->FirstCluster;
    uint32_t oldclus = startCluster;
    uint32_t bytecount = 0;
    uint32_t bytepercluster = 512 * SD_L2_FAT.SecPerClus;
    uint8_t  retval = 0;
    SD_L2_FatCache = SD_L2_workBuf;
    SD_L2_LastFatSect = 0xffffffffUL;
    while(retval == 0) {
        retval = SD_L2_NextCluster(&startCluster);
        if(retval) return(retval);
        if(oldclus != (startCluster-1)) {
            retval = SD_L2_ERROR_FRAGMET_FOUND;
        }
        oldclus = startCluster;
        bytecount += bytepercluster;
    }
    if(bytecount < fileinfo->Size) return(retval);
    if(startCluster >= SD_L2_FAT.ClusterEndMarker) {
        return(0);
    } else {
        return(SD_L2_ERROR_FAT_ENTRY);
    }
}

/**
 * DeInitialize the file system (for safe power down mode)
 *
 * Get status (initialized or not) by reading SD_L2_FAT.PartType.
 */
void SD_L2_DeInit()
{
    SD_L2_FAT.PartType = SD_L2_PARTTYPE_UNKNOWN;
    SD_L1_DeInit();
}

/**
 * Initialize the file system .
 *
 * Does the lower level initialization and
 * tries to find the boot sector of the first FAT16
 * or FAT32 partition and parse it.
 * Workbuf must hold at least 512 bytes.
 * Workbuf will be used later also for following functions:
 * - SD_L2_SearchFile
 * - SD_L2_Dir
 *
 * \return Zero if successful, error code otherwise
 */
uint8_t SD_L2_Init(uint8_t *pWorkBuf)
{
    uint8_t  retval;
    uint8_t  PartType;
    uint16_t temp16;
    uint32_t temp32;
    
    SD_L2_FAT.PartType = SD_L2_PARTTYPE_UNKNOWN;
    SD_L2_LastFatSect = 0;
    
    if(pWorkBuf == NULL) return(SD_L2_ERROR_WORKBUF);
    SD_L2_workBuf = pWorkBuf;
    
    // Try init SD-Card
    retval = SD_L1_Init();
    if(retval) return(retval);
    
    // ==== MBR (partition table) access here =====
    
    // Read sector 0 
    retval = SD_L1_ReadBlock(0, SD_L2_workBuf);
    if(retval) return(retval);

    // Test for signature (valid not only for MBR, but FAT Boot Sector as well!)
    if((SD_L2_workBuf[0x1fe] != 0x55) || (SD_L2_workBuf[0x1ff] != 0xaa)) return(SD_L2_ERROR_INVAL_SECT0);
    
    // Store most important MBR values for first partition
    PartType = SD_L2_workBuf[0x1be + 0x04];
    SD_L2_FAT.BootSectorStart =  (uint32_t)SD_L2_workBuf[0x1be + 0x08] 
                             | ((uint32_t)SD_L2_workBuf[0x1be + 0x09] << 8UL)
                             | ((uint32_t)SD_L2_workBuf[0x1be + 0x0a] << 16UL)
                             | ((uint32_t)SD_L2_workBuf[0x1be + 0x0b] << 24UL);
    
    // Check MBR values for plausibility
    if(  ((SD_L2_workBuf[0x1be] & 0x7f) == 0)
      && ((PartType == 0x04) || (PartType == 0x06) || (PartType == 0x0B) 
           || (PartType == 0x0C) || (PartType == 0x0E)) )  
    {
        // MBR seems to contain valid FAT16/FAT32 partition entry
        SD_L2_FAT.PartType = ((PartType == 0x0B) || (PartType == 0x0C)) ? SD_L2_PARTTYPE_FAT32 : SD_L2_PARTTYPE_FAT16;
    }
    else
    {
        // MBR seems to contain not an valid entry, so try for super-floppy now
        SD_L2_FAT.BootSectorStart = 0UL;
        SD_L2_FAT.PartType = SD_L2_PARTTYPE_SUPERFLOPPY;
    }
    
    // ====== FAT access here ======
    
    // Read Boot-Sector and test for signature
    retval = SD_L1_ReadBlock(SD_L2_FAT.BootSectorStart, SD_L2_workBuf);
    if(retval) return(retval);  

    // Test for signature (valid not only for MBR, but FAT Boot Sector as well!)
    if((SD_L2_workBuf[0x1fe] != 0x55) || (SD_L2_workBuf[0x1ff] != 0xaa)) return(SD_L2_ERROR_INVAL_BS);
    
    // Plausibility checks for FAT
    if((SD_L2_workBuf[0x0b] != 0x00) || (SD_L2_workBuf[0x0c] != 0x02) || (SD_L2_workBuf[0x15] != 0xf8)) return(SD_L2_ERROR_INVAL_BS);

    // Read fields that are same for FAT16 and FAT32
    SD_L2_FAT.SecPerClus = SD_L2_workBuf[0x0d];
    SD_L2_FAT.RsvdSecCnt = (uint16_t)SD_L2_workBuf[0x0e] | ((uint16_t)SD_L2_workBuf[0x0f]<<8U);
    if((SD_L2_FAT.SecPerClus == 0) || (SD_L2_FAT.RsvdSecCnt == 0)) return(SD_L2_ERROR_INVAL_BS);
    SD_L2_FAT.NumFATs = SD_L2_workBuf[0x10];
    SD_L2_FAT.RootEntryCount = (uint16_t)SD_L2_workBuf[0x11] | ((uint16_t)SD_L2_workBuf[0x12]<<8U);
    
    temp16 = (uint16_t)SD_L2_workBuf[0x13] | ((uint16_t)SD_L2_workBuf[0x14]<<8U);
    temp32 = (uint32_t)SD_L2_workBuf[0x20] | ((uint32_t)SD_L2_workBuf[0x21]<<8U) | ((uint32_t)SD_L2_workBuf[0x22]<<16U) | ((uint32_t)SD_L2_workBuf[0x23]<<24U);
    SD_L2_FAT.TotalSec  = temp16 ? temp16 : temp32;
    
    temp16 = (uint16_t)SD_L2_workBuf[0x16] | ((uint16_t)SD_L2_workBuf[0x17]<<8U);
    temp32 = (uint32_t)SD_L2_workBuf[0x24] | ((uint32_t)SD_L2_workBuf[0x25]<<8U) | ((uint32_t)SD_L2_workBuf[0x26]<<16U) | ((uint32_t)SD_L2_workBuf[0x27]<<24U);
    SD_L2_FAT.SecPerFAT  = temp16 ? temp16 : temp32;
    
    // Calculate start sectors
    SD_L2_FAT.FatStart = SD_L2_FAT.BootSectorStart + (uint32_t)SD_L2_FAT.RsvdSecCnt;
    SD_L2_FAT.RootDirStart = SD_L2_FAT.FatStart + SD_L2_FAT.NumFATs * (uint32_t)SD_L2_FAT.SecPerFAT;
    
    // Data area starts at cluster #2
    SD_L2_FAT.DataStart = SD_L2_FAT.RootDirStart+ ((32 * (uint32_t)SD_L2_FAT.RootEntryCount + 511)/512) - (2 * SD_L2_FAT.SecPerClus);
    
    // determine shift that is same as multiply by SD_L2_FAT.SecPerClus
    SD_L2_FAT.ClusterSizeShift = 0;
    while (SD_L2_FAT.SecPerClus != (1 << SD_L2_FAT.ClusterSizeShift)) {
      // error if not power of 2
      if (SD_L2_FAT.ClusterSizeShift++ > 7) return(SD_L2_ERROR_INVAL_BS);
    }	
    
     // Calculate number and shifting of clusters
    // total data blocks
    SD_L2_FAT.ClusterCount = SD_L2_FAT.TotalSec - (SD_L2_FAT.DataStart - SD_L2_FAT.BootSectorStart);
    // divide by cluster size to get cluster count
    SD_L2_FAT.ClusterCount >>= SD_L2_FAT.ClusterSizeShift;	
    
    // determine if FAT16 or FAT32 (only by cluster count as done by M$)
    if (SD_L2_FAT.ClusterCount < 4085) {
        // this would be FAT12, which is not supported
        SD_L2_FAT.PartType = SD_L2_PARTTYPE_UNKNOWN;
        return(SD_L2_ERROR_FAT12);
    } else if (SD_L2_FAT.ClusterCount < 65525) {
        SD_L2_FAT.PartType = SD_L2_PARTTYPE_FAT16;
        SD_L2_FAT.ClusterEndMarker = 0xfff8UL;
    } else {
        temp32 = (uint32_t)SD_L2_workBuf[0x2c] | ((uint32_t)SD_L2_workBuf[0x2d]<<8U) | ((uint32_t)SD_L2_workBuf[0x2e]<<16U) | ((uint32_t)SD_L2_workBuf[0x2f]<<24U);
        SD_L2_FAT.RootDirStart = SD_L2_Cluster2Sector(temp32);
        SD_L2_FAT.PartType = SD_L2_PARTTYPE_FAT32;
        SD_L2_FAT.ClusterEndMarker = 0xffffff8UL;
    }
      
    return 0;
}

/**
 * Search a file in the directory.
 * Filename must be 8.3 format, terminated by \0 
 * (can not access ".." now...)
 *
 * Works only over one cluster of directory information. If 
 * SD_L2_ERROR_DIR_EOC is returned call function again with next
 * cluster number. 
 *
 * Set cluster to 0 to access root directory.
 *
 * Deleted files and long name entries are not shown generally.
 *
 * Only files are printed that has their attributes set/unset regarding maskSet/maskUnset.
 * Examples for maskSet, maskUnset:
 *  Ouput everything:           0x00, 0x00
 *  Shortname files only:       0x00, 0x18
 *  Shortname files and dirs:   0x00, 0x08
 *  Shortname dirs:             0x10, 0x08
 *  Volume name:                0x08, 0x10
 *
 * Mask bits: B7 = 0, B6 = 0, B5 = archive, B4 = directory, 
 *            B3 = volume name, B2 = system, B1 = hidden, B0 = read only
 * 
 * If file is found, fileinfo gets filled with file attributes, file size in bytes and first cluster.
 *
 *  return Zero if successfully, error code otherwise
 */
uint8_t SD_L2_SearchFile(uint8_t *filename, const uint32_t cluster, const uint8_t maskSet, const uint8_t maskUnset, SD_L2_File_t *fileinfo)
{
    uint16_t maxsect = SD_L2_FAT.SecPerClus;
    uint32_t startsect = SD_L2_Cluster2Sector(cluster);
    char fnentry[12];
    
    if((SD_L2_FAT.PartType != SD_L2_PARTTYPE_FAT16) && (SD_L2_FAT.PartType != SD_L2_PARTTYPE_FAT32)) return(SD_L2_ERROR_FAT_NOT_INIT);
    if(cluster == 0) {
        // Set root dir sector
        startsect = SD_L2_FAT.RootDirStart;
        if(SD_L2_FAT.PartType == SD_L2_PARTTYPE_FAT16) maxsect = (uint16_t)((32 * (uint32_t)SD_L2_FAT.RootEntryCount + 511)/512);
    }
    
    // convert filename to space-filled uppercase format
    for(uint8_t i = 0; i < 11; i++) fnentry[i] = ' ';
    for(uint8_t i = 0; i < 9; i++) {
        uint8_t c = *filename++;
        if((c < 0x20) || (c == '.')) break;
        if((c>='a') && (c<='z')) c -= 0x20;  // to upper case
        fnentry[i] = c;
    }
    for(uint8_t i = 8; i < 11; i++) {
        uint8_t c = *filename++;
        if(c < 0x20) break;
        if((c>='a') && (c<='z')) c -= 0x20;  // to upper case
        fnentry[i] = c;
    }
    fnentry[11] = 0;
    //Serial.println(fnentry);
    
    // go through sectors
    for(uint16_t i = 0; i<maxsect; i++) {
        uint8_t retval = SD_L1_ReadBlock(startsect + i, SD_L2_workBuf);
        if(retval) return(retval);
        
        for(uint16_t j = 0; j<512; j+=32) {
            uint8_t attrib;
            if(SD_L2_workBuf[j] == 0) return(SD_L2_ERROR_FILE_NOT_FOUND);    // Last entry when first character of filename == 0
            if(SD_L2_workBuf[j] == 0xe5) continue;  // Skip deleted files
            if(SD_L2_workBuf[j] == 0x05) SD_L2_workBuf[j] = 0xE5;
            
            attrib = SD_L2_workBuf[j+0x0b];
            
            // Test masks (skip long file name entries also)
            if(((attrib & maskSet) == maskSet) && ((attrib & maskUnset) == 0) && (attrib != 0x0f)) {
                uint16_t k;
                
                // compare filename
                for(k = 0; k < 11; k++) if(SD_L2_workBuf[j+k] != fnentry[k]) break;
                if(k >= 11) {
                    // found it
                    fileinfo->Attributes = attrib;
                    fileinfo->Size = (uint32_t)SD_L2_workBuf[j+0x1c] | (((uint32_t)SD_L2_workBuf[j+0x1d])<<8) 
                                    | (((uint32_t)SD_L2_workBuf[j+0x1e])<<16) | (((uint32_t)SD_L2_workBuf[j+0x1f])<<24);
                    if(SD_L2_FAT.PartType == SD_L2_PARTTYPE_FAT16) {
                        fileinfo->FirstCluster = (uint32_t)SD_L2_workBuf[j+0x1a] | (((uint32_t)SD_L2_workBuf[j+0x1b])<<8);
                    } else {
                        fileinfo->FirstCluster = (uint32_t)SD_L2_workBuf[j+0x1a] | (((uint32_t)SD_L2_workBuf[j+0x1b])<<8) 
                             | (((uint32_t)SD_L2_workBuf[j+0x14])<<16) | (((uint32_t)SD_L2_workBuf[j+0x15])<<24);
                    }	
                    
                    // Initialize some things
                    fileinfo->ActSector = SD_L2_Cluster2Sector(fileinfo->FirstCluster);
                    fileinfo->ActBytePos = 0;
                    return(0);
                }

            }
            
        }
    }
    if(SD_L2_FAT.PartType == SD_L2_PARTTYPE_FAT16) return(SD_L2_ERROR_FILE_NOT_FOUND);
    return(SD_L2_ERROR_DIR_EOC);
}

#if SD_ENABLE_DIR_VIEW
/**
 * Prints a directory.
 * Callback must point to a function that prints one string. 
 *
 * Works only over one cluster of directory information. If 
 * SD_L2_ERROR_DIR_EOC is returned call function again with next
 * cluster number. 
 *
 * Set cluster to 0 to access root directory.
 *
 * Deleted files and long name entries are not shown generally.
 *
 * Only files are printed that has their attributes set/unset regarding maskSet/maskUnset.
 * Examples for maskSet, maskUnset:
 *  Ouput everything:           0x00, 0x00
 *  Shortname files only:       0x00, 0x18
 *  Shortname files and dirs:   0x00, 0x08
 *  Shortname dirs:             0x10, 0x08
 *  Volume name:                0x08, 0x10
 *
 * Mask bits: B7 = 0, B6 = 0, B5 = archive, B4 = directory, 
 *            B3 = volume name, B2 = system, B1 = hidden, B0 = read only
 */
uint8_t SD_L2_Dir(const uint32_t cluster, const uint8_t maskSet, const uint8_t maskUnset, void (*callback)(char *entryLine))
{
    uint16_t maxsect = SD_L2_FAT.SecPerClus;
    uint32_t startsect = SD_L2_Cluster2Sector(cluster);
    char buf[16];
    
    if((SD_L2_FAT.PartType != SD_L2_PARTTYPE_FAT16) && (SD_L2_FAT.PartType != SD_L2_PARTTYPE_FAT32)) return(SD_L2_ERROR_FAT_NOT_INIT);
    if(cluster == 0) {
        // Set root dir sector
        startsect = SD_L2_FAT.RootDirStart;
        if(SD_L2_FAT.PartType == SD_L2_PARTTYPE_FAT16) maxsect = (uint16_t)((32 * (uint32_t)SD_L2_FAT.RootEntryCount + 511)/512);
    }
    
    for(uint16_t i = 0; i<maxsect; i++) {
        uint8_t retval = SD_L1_ReadBlock(startsect + i, SD_L2_workBuf);
        if(retval) return(retval);
        
        for(uint16_t j = 0; j<512; j+=32) {
            uint8_t attrib;
            if(SD_L2_workBuf[j] == 0) return(0);    // Last entry when first character of filename == 0
            if(SD_L2_workBuf[j] == 0xe5) continue;  // Skip deleted files
            if(SD_L2_workBuf[j] == 0x05) SD_L2_workBuf[j] = 0xE5;
            
            attrib = SD_L2_workBuf[j+0x0b];
            
            // Test masks (skip long file name entries also)
            if(((attrib & maskSet) == maskSet) && ((attrib & maskUnset) == 0) && (attrib != 0x0f)) {
                uint8_t z=0;
                // Prepare output
                for(uint16_t k = 0; k < 16; k++) buf[k] = ' ';
                for(uint16_t k = 0; k < 8; k++) {
                  buf[z] = SD_L2_workBuf[j+k];
                  if(buf[z] > ' ') z++;     // Remove space
                }
                buf[z++] = '.';
                for(uint16_t k = 0; k < 3; k++) buf[z++] = SD_L2_workBuf[j+k+0x08];
                //buf[z++] = ' ';
                buf[z++] = 0;

                callback(buf);
            }
            
        }
    }
    if(SD_L2_FAT.PartType == SD_L2_PARTTYPE_FAT16) return(0);
    return(SD_L2_ERROR_DIR_EOC);
}
#endif  /* SD_ENABLE_DIR_VIEW */