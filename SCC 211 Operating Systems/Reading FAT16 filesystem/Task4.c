#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
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

//Structure for en entity in the root directory of a FAT16 file
typedef struct __attribute__((__packed__)) {
    uint8_t DIR_Name[ 11 ];     // Non zero terminated string
    uint8_t DIR_Attr;           // File attributes
    uint8_t DIR_NTRes;          // Used by Windows NT, ignore
    uint8_t DIR_CrtTimeTenth;   // Tenths of sec. 0 ... 199
    uint16_t DIR_CrtTime;       // Creation Time in 2s intervals
    uint16_t DIR_CrtDate;       // Date file created
    uint16_t DIR_LstAccDate;    // Date of last read or write
    uint16_t DIR_FstClusHI;     // Top 16 bits file's 1st cluster
    uint16_t DIR_WrtTime;       // Time of last write
    uint16_t DIR_WrtDate;       // Date of last write
    uint16_t DIR_FstClusLO;     // Lower 16 bits file's 1st cluster
    uint32_t DIR_FileSize;      // File size in bytes
}DirectoryEntry;

//Method use to read the boot sector of a file system and make a copy of it in memory 
BootSector *LoadBootSector(int fd){
    BootSector *boot = malloc(sizeof(BootSector));
    lseek(fd, 0, SEEK_SET);
    read(fd, boot,sizeof(BootSector));
    return boot;
}

//Method used to store a copy of the first FAT into memory
uint16_t *loadFirstFAT(int fd, BootSector *boot){
    int size = boot->BPB_FATSz16 * boot->BPB_BytsPerSec;
    uint16_t *FAT = malloc(size);;
    memset(FAT,0,size);
    lseek(fd,boot->BPB_BytsPerSec*boot->BPB_RsvdSecCnt,SEEK_SET);
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
    }
    else{
        printf("Cluster number out of bounds");
    }
}

//Returns the first cluster number from the first 16 bit high and low of an entity
uint32_t getFirstClusterNum(uint16_t upper, uint16_t lower){
    return ((upper<<16)|lower);
}

//Returns a char pointer to a file's last modified time after it's formatted correctly
char* getTime(uint16_t time){
    int hour = (time>>11);
    int minute = (time>>5)&0x3F;
    int sec = time & 0x1F;
    char* timeMod = malloc(10);
    if(hour < 10){
        sprintf(timeMod,"0%d:%d:%d",hour,minute,sec);
    }else{
        sprintf(timeMod,"%d:%d:%d",hour,minute,sec);
    }
    return timeMod;
}

//Returns a char pointer to the place where the formatted date for a file is stored for the last modified date
char* getDate(uint16_t date){
    int year = (date >> 9) + 1980;
    int month = (date >> 5) & 0xf;
    int day = date & 0x1f;
    char* dateMod = malloc(10);
    if(day < 10){
        sprintf(dateMod,"0%d/%d/%d",day,month,year);
    }else{
        sprintf(dateMod,"%d/%d/%d",day,month,year);
    }
    return dateMod;
}

//Returns a char pointer to a file's attributes formatted as ADVHSR
char* getAttributes(uint16_t attributes){
    char * attr = malloc(7);
    attr[6]='\0';
    uint16_t comp = 0x20;
    for(int i = 0; i < 6; i++){
        switch((attributes & comp)){
            case 0x20:
                attr[0] = 'A';
                break;
            case 0x10:
                attr[1] = 'D';
                break;
            case 0x8:
                attr[2] = 'V';
                break;
            case 0x4:
                attr[3] = 'S';
                break;
            case 0x2:
                attr[4] = 'H';
                break;
            case 0x1:
                attr[5] = 'R';
                break;
            default:
                attr[i] = '-';
                break;
        }
        comp = comp >>1;
    }
    return attr;
}

//check file names from root table to see if a file is valid, been deleted or no more valid files
int checkFileName(uint8_t *filename){
    switch(filename[0]){
        case 0x00:
            return 1;
            break;
        case 0xE5:
            return 2;
            break;
        default:
            return 0;
    }
}

//Checks file attributes to see if a file is a long
int checkFile(uint16_t attributes){
    if ((attributes & 0x20)==0 && (attributes & 0x10)==0 && (attributes & 0x7) == 7){
        return 1;
    }
    return 0;
}

//Outputs all the files on the FAT16 image in a table, except for long files
void outputRootFile(DirectoryEntry *root, uint16_t size){
    printf("Starting Cluster:\tModified Time:\tModified Date:\tFile attributes:\tFile size:\tFile Name:\n");
    for(int i = 0;i<size; i++){
        if(checkFileName(root[i].DIR_Name) == 1){
            break;
        }else if(checkFileName(root[i].DIR_Name) == 2){
        }else if(checkFile(root[i].DIR_Attr) == 0){
            printf("%-24d %-16s %-17s %-19s %-15d %s\n", getFirstClusterNum(root[i].DIR_FstClusHI,root[i].DIR_FstClusLO), getTime(root[i].DIR_WrtTime),getDate(root[i].DIR_WrtDate),getAttributes(root[i].DIR_Attr),root[i].DIR_FileSize,root[i].DIR_Name);
        }
    }
}

//Loads root into memory 
void LoadRoot(int fd, BootSector *boot){
    DirectoryEntry root[boot->BPB_RootEntCnt];
    lseek(fd, boot->BPB_BytsPerSec *(boot->BPB_RsvdSecCnt+boot->BPB_FATSz16*boot->BPB_NumFATs),SEEK_SET);
    read(fd,&root,sizeof(root));
    outputRootFile(root,boot->BPB_RootEntCnt);
}

//Output a table of file details stored on the root directory that is not a long file
void main(){
    int fd = open("fat16.img",O_RDONLY);
    if(fd == -1){
        printf("File not found.");
        exit(0);
    }
    BootSector *boot = LoadBootSector(fd);
    uint16_t *FAT = loadFirstFAT(fd,boot);
    LoadRoot(fd,boot);
    close(fd);
}