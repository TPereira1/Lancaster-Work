#include <stdio.h>
#include <string.h>
struct sled {
    int  x,y,orientation,totalDistance;
};

void initialise_map(char map[30][30]){
    for(int x = 0; x < 30; x++){
        for(int y = 0; y < 30; y++){
            map[x][y] = '.';
        }
    }
}

void populate_map(FILE *MapFile,char map[30][30]){
    if(MapFile != NULL){
        int x,y;
        char c;
        while(fscanf(MapFile,"%d,%d,%c",&x,&y,&c)!=EOF){
            map[x][y]=c;
        }
    }
    else{
        printf("Map file cannot be found\n.");
    }

}

void print_map(char map[30][30]){
    for(int y = 29; y >= 0; y--){
        for(int x = 0; x < 30; x++){
            printf("%c", map[x][y]);
        }
        printf("\n");
    }
}

int process_command(FILE* CommandFile, char map[30][30], struct sled* sledPointer){
    if(CommandFile != NULL){
        char movement[10];
        int magnitude;
        while(fscanf(CommandFile,"%s %d",movement,&magnitude)!=EOF){
            if(strcmp(movement,"FORWARD")==0){
                sledPointer->totalDistance =sledPointer->totalDistance + magnitude;
                switch(sledPointer->orientation){
                    case 0:
                        sledPointer->y = sledPointer->y + magnitude;
                        break;
                    case 90:
                        sledPointer->x = sledPointer->x + magnitude;
                        break;
                    case 180:
                        sledPointer->y = sledPointer->y - magnitude;
                        break;
                    case 270:
                        sledPointer->x = sledPointer->x - magnitude;
                        break;
                    default:
                        printf("Error with sled orientation.\n");
                }
            }
            else if(strcmp(movement,"TURN")==0){
                if(magnitude == 0 || magnitude == 90 || magnitude == 180 || magnitude == 270){
                    sledPointer->orientation += magnitude;
                    if(sledPointer->orientation >= 360){
                        sledPointer->orientation = (sledPointer->orientation) % 360;
                    }
                }
                else{
                    printf("%d is not an appropriate degree to turn.\n",magnitude);
                }
            }
            else{
                printf("Unknown command: %s\n",movement);
            }
        }
    }
    else{
        printf("Command file not found\n.");
    }
    return 0;
}

void print_sled(struct sled SLED1){
    printf("sled status:\nlocation %d,%d\norientation %d degrees\ndistance travelled %d squares\nend sled status.\n",SLED1.x,SLED1.y,SLED1.orientation,SLED1.totalDistance);
}

void print_symbol(char map[30][30], struct sled SLED1){
    if(SLED1.x < 30 && SLED1.x >=0 && SLED1.y < 30 && SLED1.y >=0){
        printf("sled is flying over symbol %c\n", map[SLED1.x][SLED1.y]);
    }
    else{
        printf("The sled is not on the map.\n");
    }
}

int is_over_symbol (char map[30][30], struct sled SLED1, char Symbol){
    if(map[SLED1.x][SLED1.y] == Symbol){
        return (1);
    }
    else{
        return (0);
    }
}

int extended_process_command(FILE* CommandFile, char map[30][30], struct sled* sledPointer){
    int count = 0;
    if(CommandFile != NULL){
        char movement[10];
        int magnitude;
        while(fscanf(CommandFile,"%s %d",movement,&magnitude)!=EOF){
            count++;
            if(strcmp(movement,"FORWARD")==0){
                sledPointer->totalDistance =sledPointer->totalDistance + magnitude;
                switch(sledPointer->orientation){
                    case 0:
                        sledPointer->y = sledPointer->y + magnitude;
                        break;
                    case 90:
                        sledPointer->x = sledPointer->x + magnitude;
                        break;
                    case 180:
                        sledPointer->y = sledPointer->y - magnitude;
                        break;
                    case 270:
                        sledPointer->x = sledPointer->x - magnitude;
                        break;
                    default:
                        printf("Error with sled orientation.\n");
                }
            }
            else if(strcmp(movement,"BACK")==0){
                sledPointer->totalDistance =sledPointer->totalDistance + magnitude;
                switch(sledPointer->orientation){
                    case 0:
                        sledPointer->y = sledPointer->y - magnitude;
                        break;
                    case 90:
                        sledPointer->x = sledPointer->x - magnitude;
                        break;
                    case 180:
                        sledPointer->y = sledPointer->y + magnitude;
                        break;
                    case 270:
                        sledPointer->x = sledPointer->x + magnitude;
                        break;
                    default:
                        printf("Error with sled orientation.\n");
                }
            }
            else if(strcmp(movement,"TURN")==0){
                if(magnitude == 0 || magnitude == 90 || magnitude == 180 || magnitude == 270){
                    sledPointer->orientation += magnitude;
                    if(sledPointer->orientation >= 360){
                        sledPointer->orientation = (sledPointer->orientation) % 360;
                    }
                }
                else{
                    printf("%d is not an appropriate degree to turn.\n",magnitude);
                }
            }
            else{
                printf("Unknown command: %s\n",movement);
                count--;
            }

            if(map[sledPointer->x][sledPointer->y] == 'T'||map[sledPointer->x][sledPointer->y] == 't'){
                printf("WARNING: This route will fly over a tower. Route aborted.\n");
                count--;
                sledPointer->x=0;
                sledPointer->y=0;
                sledPointer->orientation=0;
                sledPointer->totalDistance=0;
                break;
            }
        }
    }
    else{
        printf("Command file not found\n.");
    }
    return (count);
}

int main(){
    struct sled SLED1= {0,0,0,0};
    FILE *MapFile;
    MapFile = fopen("map.txt", "r");
    FILE *CommandFile;
    CommandFile = fopen("calledsledcommand.txt", "r");
    char map[30][30];
    initialise_map(map);
    populate_map(MapFile,map);
    print_map(map);
    printf("%d commands executed\n", extended_process_command(CommandFile, map, &SLED1));
    print_sled(SLED1);
    print_symbol(map, SLED1);
    printf("%d\n",is_over_symbol (map,SLED1, '.'));
}