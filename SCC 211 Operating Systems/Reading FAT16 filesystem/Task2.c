#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//Structure for BootSector of a FAT16 file
typedef struct __attribute__((__packed__)) {
    uint8_t BS_jmpBoot[ 3 ];    // x86 jump instr. to boot code
    uint8_t BS_OEMName[ 8 ];    // What created the filesystem
    uint16_t BPB_BytsPerSec;    // Bytes per Sector
    uint8_t BPB_SecPerClus;     // Sectors per Cluster
    uint16_t BPB_RsvdSecCnt;    // Reserved Sector Count
    uint8_t BPB_NumFATs;        // Number of copies of FAT
    uint16_t BPB_RootEntCnt;    // FAT12/FAT16: size of root DIR
    uint16_t BPB_TotSec16;      // Sectors, may be 0, see below
    uint8_t BPB_Media;          // Media type, e.g. fixed
    uint16_t BPB_FATSz16;       // Sectors in FAT (FAT12 or FAT16)
    uint16_t BPB_SecPerTrk;     // Sectors per Track
    uint16_t BPB_NumHeads;      // Number of heads in disk 
    uint32_t BPB_HiddSec;       // Hidden Sector count
    int32_t BPB_TotSec32;       // Sectors if BPB_TotSec16 == 0
    uint8_t BS_DrvNum;          // 0 = floppy, 0x80 = hard disk
    uint8_t BS_Reserved1;       //
    uint8_t BS_BootSig;         // Should = 0x29
    uint32_t BS_VolID;          // 'Unique' ID for volume
    uint8_t BS_VolLab[ 11 ];    // Non zero terminated string
    uint8_t BS_FilSysType[ 8 ]; // e.g. 'FAT16 ' (Not 0 term.
} BootSector;

//Reads FAT16 BootSector and the output some of the details of the file
void main() {
  int fd = open("fat16.img", O_RDONLY);
  lseek(fd, 0, SEEK_SET);
  BootSector a;
  read(fd, &a,sizeof(a));
  printf("Bytes per Sector: %d\n", a.BPB_BytsPerSec);
  printf("Sectors per Cluster: %d\n", a.BPB_SecPerClus);
  printf("Reserved Sector Count: %d\n", a.BPB_RsvdSecCnt);
  printf("Number of copies of FAT: %d\n", a.BPB_NumFATs);
  printf("FAT12/FAT16: size of root DIR: %d\n", a.BPB_RootEntCnt);
  printf("Sectors, may be 0, see below: %d\n", a.BPB_TotSec16);
  printf("Sectors in FAT (FAT12 or FAT16): %d\n", a.BPB_FATSz16);
  printf("Sectors if BPB_TotSec16 == 0: %d\n", a.BPB_TotSec32);
  printf("Non zero terminated string: %s\n", a.BS_VolLab);
  close(fd);
}