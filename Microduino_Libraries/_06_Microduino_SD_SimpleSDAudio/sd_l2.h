#ifndef SD_L2_H
#define SD_L2_H

#define SD_ENABLE_DIR_VIEW          1

/** No valid MBR/FAT-BS signature found in sector 0 */
#define SD_L2_ERROR_INVAL_SECT0     0x30
/** Malformed FAT boot sector */
#define SD_L2_ERROR_INVAL_BS        0x31
/** FAT12 is not supported */
#define SD_L2_ERROR_FAT12           0x32
/** FAT not initialized properly */
#define SD_L2_ERROR_FAT_NOT_INIT    0x33
/** End of cluster reached (not a real error, just information) */
#define SD_L2_ERROR_DIR_EOC         0x34
/** File not found after reaching end of directory */
#define SD_L2_ERROR_FILE_NOT_FOUND  0x35
/** Fragmentation found */
#define SD_L2_ERROR_FRAGMET_FOUND   0x36
/** Error in FAT entry */
#define SD_L2_ERROR_FAT_ENTRY       0x37
/** End of file reached */
#define SD_L2_ERROR_EOF             0x38
/** WorkBuf not set */
#define SD_L2_ERROR_WORKBUF         0x39


#define SD_L2_PARTTYPE_UNKNOWN      0
#define SD_L2_PARTTYPE_SUPERFLOPPY  1
#define SD_L2_PARTTYPE_FAT16        2
#define SD_L2_PARTTYPE_FAT32        3

typedef struct  {
    uint8_t     PartType;       // Use this to test whether it is FAT16 or FAT32 or not initialized
	
	// Stuff from FAT boot sector
	uint8_t     SecPerClus;
	uint16_t    RsvdSecCnt;
	uint8_t     NumFATs;
	uint16_t    RootEntryCount;
	uint32_t    TotalSec;
	uint32_t    SecPerFAT;
	uint32_t    RootClus;
	
	// For cluster calculations
	uint8_t     ClusterSizeShift;
	uint32_t    ClusterCount;
	
	// Start addresses (all in blocks / sector addresses)
	uint32_t    BootSectorStart;    // Address of boot sector from FAT
	uint32_t    FatStart;           // First file allocation table starts here
	uint32_t    RootDirStart;       // Root directory starts here
	uint32_t    DataStart;          // Cluster 0 starts here
	
	uint32_t    ClusterEndMarker;   // if Cluster >= this then end of file reached.
	
} SD_L2_FAT_t;

typedef struct {
	uint8_t     Attributes;
	uint32_t    Size;           // in bytes
	uint32_t    FirstCluster;   // First cluster
    
	uint32_t    ActSector;      // 0 to (SD_L2_FAT.SecPerClus - 1)
	uint32_t    ActBytePos;     // 0 to Size
} SD_L2_File_t;

extern SD_L2_FAT_t SD_L2_FAT;

uint8_t     SD_L2_Init(uint8_t *pWorkBuf);
void        SD_L2_DeInit();

uint8_t     SD_L2_SearchFile(uint8_t *filename, const uint32_t cluster, const uint8_t maskSet, const uint8_t maskUnset, SD_L2_File_t *fileinfo);
uint32_t    SD_L2_Cluster2Sector(uint32_t cluster);

uint8_t     SD_L2_IsFileFragmented(SD_L2_File_t *fileinfo);

#if SD_ENABLE_DIR_VIEW 
  uint8_t SD_L2_Dir(const uint32_t cluster, const uint8_t maskSet, const uint8_t maskUnset, void (*callback)(char *entryLine));
#endif  /* SD_ENABLE_DIR_VIEW */

#endif