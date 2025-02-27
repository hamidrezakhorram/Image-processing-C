#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_START "<x:row>"
#define ROW_END   "</x:row>"

int getName(const char *time) {
    int hours, minutes, seconds;
    if (sscanf(time, "%*d/%*d/%*d %d:%d:%d", &hours, &minutes, &seconds) == 3) {
        return hours * 10000 + minutes * 100 + seconds;
    }

    return -1;
}

void readName(int *check, int *checkCount) {
    int rowCount = 0;
    char line[4096];
    char photoName[50];
    char *fileName = "125712";
    int name =125712;
    FILE *fptr;
    fptr = fopen("file.xml", "r");
    if (fptr == NULL) printf("File error, can't read! n");
    else {
        printf("Printing file content:n");

        while (fgets(line, sizeof(line), fptr)) {
            char *start = strstr(line, "<x:v>");
            while (start) {
                start += 5;
                char *end = strstr(start, "</x:v>");
                if (end) {
                    *end = '\0';
                    rowCount++;

                    if (strtod(start, NULL) == 0.0f) {
                        rowCount = 0;


                    } else if (photoName[0] == 0 && rowCount == 4) {
                        sprintf(photoName, "%d", getName(start));
                       // strcat(photoName, ".bmp");
                        if (name== getName(start)) {
                            check[*checkCount] = 0;
                            (*checkCount)++;
                        }else {
                            //check[*checkCount] = 1;
                            (*checkCount)++;
                        }

                    } else if (strtod(start, NULL) && rowCount == 7) {
                        sprintf(photoName, "%d", getName(start));
                        //strcat(photoName, ".bmp");
                       // printf("the get time is%d\n", getName(start));
                       int value = getName(start);
                        if (name== value) {
                            printf("the line found %s\n", photoName);
                            check[*checkCount] = 0;
                            (*checkCount)++;
                        }else {
                            //check[*checkCount] = 1;
                            (*checkCount)++;
                        }

                    }
                }
                start = (end) ? strstr(end + 1, "<x:v>") : NULL;
            }
        }

        fclose(fptr);
    }
    printf("XML file read successfully\n");

}


int main(int argc, char *argv[]) {

    int check[3000];
    for (int i = 0; i < 3000; i++) {
        check[i] = 1;
    }
    int chekCount = 0;
    int count =0;
    int rowCount=0;
    readName(check, &chekCount);
    const char *filename = "file.xml";
    for (int i = 0; i < chekCount; ++i) {
        if(check[i]==0){
            printf("yes\n");
        }
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }


    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);


    char *buffer = malloc(filesize + 1);
    if (!buffer) {
        perror("Memory allocation error");
        fclose(fp);
        return 1;
    }
    fread(buffer, 1, filesize, fp);
    buffer[filesize] = '\0';
    fclose(fp);


    char *output = malloc(filesize + 1);
    if (!output) {
        perror("Memory allocation error");
        free(buffer);
        return 1;
    }
    output[0] = '\0';


    char *pos = buffer;
    char *row_start, *row_end;
    while ((row_start = strstr(pos, ROW_START)) != NULL) {
        strncat(output, pos, row_start - pos);

        row_end = strstr(row_start, ROW_END);

        if (!row_end) {
            break;
        }
        row_end += strlen(ROW_END);
        if(rowCount>2){
            if(check[count]==0   ){
                strncat(output, row_start, row_end - row_start);
                count++;

            }else {
                printf("Removed row containing target time: \n");
                count++;
            }
        }

         rowCount++;
        pos = row_end;
    }


    strcat(output, pos);


    fp = fopen("output.xml", "wb");
    if (!fp) {
        perror("Error opening file for writing");
        free(buffer);
        free(output);
        return 1;
    }
    fwrite(output, 1, strlen(output), fp);
    fclose(fp);

    free(buffer);
    free(output);

    return 0;
}

