#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
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

//Long file entry structure
typedef struct __attribute__((__packed__)) {
    uint8_t LDIR_Ord;           // Order/ position in sequence/ set
    uint16_t LDIR_Name1[ 5 ];   // First 5 UNICODE characters 
    uint8_t LDIR_Attr;          // = ATTR_LONG_NAME (xx001111)
    uint8_t LDIR_Type;          // Should = 0 
    uint8_t LDIR_Chksum;        // Checksum of short name
    uint16_t LDIR_Name2[ 6 ];   // Middle 6 UNICODE characters 
    uint16_t LDIR_FstClusLO;    // MUST be zero
    uint16_t LDIR_Name3[ 2 ];    // Last 2 UNICODE characters
}LongEntry;

//Global variables place here to be seen to the hole program
BootSector *boot;       //Used to store boot sector allow access to bootSector to all methods
uint16_t *FAT;          //Used to store the first FAT
DirectoryEntry *cd;     //Current directory, holds all entities for current directory
int cdSize;             //Number of entities in the Current Directory

//Method use to read the boot sector of a file system and make a copy of it in memory 
BootSector *LoadBootSector(int fd){
    BootSector *boot = malloc(sizeof(BootSector));
    lseek(fd, 0, SEEK_SET);
    read(fd, boot,sizeof(BootSector));
    return boot;
}

//Method used to store a copy of the first FAT into memory
uint16_t *loadFirstFAT(int fd){
    int size = boot->BPB_FATSz16 * boot->BPB_BytsPerSec;
    uint16_t *FAT = malloc(size);
    memset(FAT,0,size);
    lseek(fd,boot->BPB_BytsPerSec*boot->BPB_RsvdSecCnt,SEEK_SET);
    read(fd, FAT,size);
    return FAT;
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

//Checks file attributes to see if a file is a long, not a readable file
int checkFile(uint16_t attributes){
    if ((attributes & 0x20)==0 && (attributes & 0x10)==0 && (attributes & 0x7) == 7){
        return 1;
    }else if((attributes & 0X18) != 0){
        return 2;
    }
    return 0;
}

//Decodes long entry file for 13 character name 
void getLongName(LongEntry *entry, char *name){
    int index = strlen(name);
    for(int i = 0; i < 5;i++){
        name[index] = (char)entry->LDIR_Name1[i];
        index++;
    }
    for(int i = 0; i < 6;i++){
        name[index] = (char)entry->LDIR_Name2[i];
        index++;
    }
    for(int i = 0; i < 2;i++){
        name[index] = (char)entry->LDIR_Name3[i];
        index++;
    }
    name[index]='\0';
}

//Outputs all the files on the FAT16 image in a table
void outputDirectory(DirectoryEntry *root, uint16_t size){
    printf("Starting Cluster:\tModified Time:\tModified Date:\tFile attributes:\tFile size:\tFile Name:\n");
    for(int i = 0;i<size; i++){
        if(checkFile(root[i].DIR_Attr) != 1 && checkFileName(root[i].DIR_Name) == 0){
            char *filename = malloc(14);
            filename[0] = '\0';
            int x = i - 1;
            int count = 1;
            while(x > 0 && checkFile(root[x].DIR_Attr) == 1){
                filename = realloc(filename,count*28);
                LongEntry *entryLong = (LongEntry*)&root[x];
                getLongName(entryLong,filename);
                x--;
                count++;
            }
            char *time = getTime(root[i].DIR_WrtTime);
            char* date = getDate(root[i].DIR_WrtDate);
            char* attributes = getAttributes(root[i].DIR_Attr);
            if(strlen(filename) == 0){
                printf("%-24d %-16s %-17s %-19s %-15d %s\n", getFirstClusterNum(root[i].DIR_FstClusHI,root[i].DIR_FstClusLO), time, date,attributes,root[i].DIR_FileSize,root[i].DIR_Name);
            }else{
                printf("%-24d %-16s %-17s %-19s %-15d %-16s -%s\n", getFirstClusterNum(root[i].DIR_FstClusHI,root[i].DIR_FstClusLO), time, date,attributes,root[i].DIR_FileSize,root[i].DIR_Name,filename);
            }
            free(time);
            free(date);
            free(attributes);
        }else if(checkFileName(root[i].DIR_Name) == 1){
            break;
        }
    }
}

//Loads root into memory 
DirectoryEntry *LoadRoot(int fd){
    DirectoryEntry *root = malloc(boot->BPB_RootEntCnt *sizeof(DirectoryEntry));
    lseek(fd, boot->BPB_BytsPerSec *(boot->BPB_RsvdSecCnt+boot->BPB_FATSz16*boot->BPB_NumFATs),SEEK_SET);
    read(fd,root,boot->BPB_RootEntCnt *sizeof(DirectoryEntry));
    outputDirectory(root,boot->BPB_RootEntCnt);
    cdSize = boot->BPB_RootEntCnt;
    return root;
}

//Compare file name from the root directory to the name being searched for, since strcmp doesnt seem to work
int compare(char *Searchfile,char *entryfile){
    if(strlen(Searchfile)==strlen(entryfile)){
        int count = 0;
        for(int i = 0; i < strlen(entryfile); i++){
            if(Searchfile[i] == entryfile[i]){
                count++;
            }
        }
        if (count == strlen(entryfile)){
            return 0;
        }
    }
    return 1;
}

//Format root file name so can be compared to entered text
void formatName(char *name){
    int x = 0;
    for(int i = 0; name[i]!='\0'; i++){
        if(!isspace(name[i])&&(isdigit(name[i])||isalpha(name[i])||name[i]=='.'||name[i]=='~')){
            name[x] = name[i];
            x++;
        }
    }
    name[x]='\0';
}

//Returns a list of cluster that make up a file, excludes the EOF
int *getFileCulsters(int ClusterNum){
    int *FileClusters = malloc(sizeof(int)*20);
    int count = 0;
    uint16_t next_cluster = ClusterNum;
    while (next_cluster < 0xfff8){
        FileClusters[count] = next_cluster;
        count++;
        next_cluster = FAT[next_cluster];
    }
    FileClusters[count] = -1;
    return FileClusters;
}

//Reads the file, jumps to data then cluster to read
void readFile(int fd, uint16_t firstCluster, uint16_t fileSize){
    uint16_t TotalSizeToRead = fileSize;
    int *FileClus = getFileCulsters(firstCluster);
    int i = 0;
    while(FileClus[i]>-1 && TotalSizeToRead > 0){
        if(TotalSizeToRead > boot->BPB_BytsPerSec*boot->BPB_SecPerClus){
            fileSize = boot->BPB_BytsPerSec*boot->BPB_SecPerClus;
        }
        lseek(fd,(FileClus[i]-2)*boot->BPB_SecPerClus*boot->BPB_BytsPerSec+(boot->BPB_BytsPerSec*(boot->BPB_FATSz16*boot->BPB_NumFATs+boot->BPB_RsvdSecCnt)+boot->BPB_RootEntCnt*sizeof(DirectoryEntry)),SEEK_SET);
        char storage[fileSize+1];
        memset(storage,' ',fileSize);
        read(fd, storage, fileSize);
        storage[fileSize] ='\0';
        printf("%s",storage);
        TotalSizeToRead -= fileSize;
        i++;
    }
    printf("\n");
    free(FileClus);
}

//Updates the current directory, loads all files in cluster chain as directory entities
void getDirectory(int fd,int firstCluster){
    if(firstCluster <= 0){
        cd = LoadRoot(fd);
    }else{
        int *DirectoryClus = getFileCulsters(firstCluster);
        int count = 0;
        while(DirectoryClus[count] > -1){
            cd = realloc(cd, boot->BPB_SecPerClus*boot->BPB_BytsPerSec *(count+1));
            lseek(fd,(DirectoryClus[count]-2)*boot->BPB_SecPerClus*boot->BPB_BytsPerSec+(boot->BPB_BytsPerSec*(boot->BPB_FATSz16*boot->BPB_NumFATs+boot->BPB_RsvdSecCnt)+boot->BPB_RootEntCnt*sizeof(DirectoryEntry)),SEEK_SET);
            read(fd,&cd[(boot->BPB_SecPerClus*boot->BPB_BytsPerSec)/sizeof(DirectoryEntry)*count],boot->BPB_SecPerClus*boot->BPB_BytsPerSec);
            count++;
        }
        cdSize = (boot->BPB_SecPerClus*boot->BPB_BytsPerSec)/sizeof(DirectoryEntry)*(count+1);
        outputDirectory(cd,(boot->BPB_SecPerClus*boot->BPB_BytsPerSec)/sizeof(DirectoryEntry)*(count+1));
        free(DirectoryClus);
    }
}

//Gets the important information for a regular file that being requested to be read
void openFile(int fd, char *text){
    int fileFound = 1;
    char name[12];
    uint32_t fileSize;
    uint32_t firstCluster;
    for(int i = 0; i<cdSize;i++){
        snprintf(name, sizeof(name), "%s", cd[i].DIR_Name);
        formatName(name);
        if(compare(name,text)==0 && checkFile(cd[i].DIR_Attr) != 2){
            fileSize = cd[i].DIR_FileSize;
            firstCluster = getFirstClusterNum(cd[i].DIR_FstClusHI,cd[i].DIR_FstClusLO);
            fileFound = 0;
            break;
        }else if(compare(name,text)==0){
            firstCluster = getFirstClusterNum(cd[i].DIR_FstClusHI,cd[i].DIR_FstClusLO);
            fileFound = 2;
            break;
        }
    }
    if(fileFound == 0){
        printf("File Found: %s\n",name);
        readFile(fd, firstCluster, fileSize);
    }else if(fileFound == 2){
        printf("Opening dirrectory: %s\n",name);
        getDirectory(fd, firstCluster);
    }else{
        printf("File Not Found\n");
    }
}

//FAT16 image opened here, allows users to enter short file names to open files or folder in the current directory
int main(){
    int quit = 1;
    int fd = open("fat16.img",O_RDONLY);
    if(fd == -1){
        printf("File not found.\n");
        exit(0);
    }
    boot = LoadBootSector(fd);
    FAT = loadFirstFAT(fd);
    DirectoryEntry *root =LoadRoot(fd);
    cd = root;
    while (quit == 1){
        char text[12];
        printf("Please enter the file you want to read:");
        scanf("%s",text);
        if(strcmp(text,"quit") == 0){
            quit = 0;
        }else{
            openFile(fd, text);
        }
    }
    close(fd);
    free(boot);
    free(FAT);
    free(root);
}