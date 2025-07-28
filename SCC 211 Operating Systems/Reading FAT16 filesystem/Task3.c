#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

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

//Method use to read the boot sector of a file system and make a copy of it in memory 
uint16_t *loadFirstFAT(int fd, BootSector a){
    int size = a.BPB_FATSz16 * a.BPB_BytsPerSec;
    uint16_t *FAT = malloc(size);;
    memset(FAT,0,size);
    lseek(fd,a.BPB_BytsPerSec*a.BPB_RsvdSecCnt,SEEK_SET);
    read(fd, FAT,size);
    return FAT;
}

//Method to get the next clusters in a file given a starting cluster till it reaches the EOF
void getFileCulsters(uint16_t *FAT, int ClusterNum){
    if(ClusterNum < 0xfff8 && ClusterNum > 1){
        printf("The cluster that make up this file are as follows: %d", ClusterNum); 
        uint16_t next_cluster = FAT[ClusterNum];
        while (next_cluster < 0xfff8){
            printf(", %d", next_cluster);
            next_cluster = FAT[next_cluster];
        }
        printf("\n");
    }
    else{
        printf("Cluster number out of bounds\n");
    }
}

//Ask user to enter a clusternumber and then outputs the following clusters that make up a file from it.
void main() {
  int fd = open("fat16.img", O_RDONLY);
  if(fd == -1){
    printf("File not found.");
    exit(0);
  }
  lseek(fd, 0, SEEK_SET);
  BootSector a;
  read(fd, &a,sizeof(a));
  uint16_t *FAT = loadFirstFAT(fd,a);
  printf("Please enter the number of the first cluster of the file:");
  int startCluster = 0xfff8;
  scanf("%d",&startCluster);
  getFileCulsters(FAT, startCluster);
  close(fd);
}