#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>

#include "logo_data.h"

#pragma pack(push, 1)
#define EPSILON 1e-6
struct bitMapFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};
struct bitMapInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

#pragma pack(pop)

int readImage(unsigned char ***logo, int logoHeight, int logoWidth, char *inputFilenamee);

unsigned char ***readLogo(int *height, int *width);

int getData(double *rightAngel, double *leftAngel, const char *inputFilename) {

    FILE *fp = fopen(inputFilename, "rb");
    if (!fp) {
        printf("Error: Unable to open file %s\n", inputFilename);
        return -1;
    }
    struct bitMapFileHeader fileHeader;
    fread(&fileHeader, sizeof(struct bitMapFileHeader), 1, fp);
    struct bitMapInfoHeader infoHeader;
    fread(&infoHeader, sizeof(struct bitMapInfoHeader), 1, fp);
    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight);
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char ***img = (unsigned char ***) malloc(height * sizeof(unsigned char **));
    for (int i = 0; i < height; i++) {
        img[i] = (unsigned char **) malloc(width * sizeof(unsigned char *));
        for (int j = 0; j < width; j++) {
            img[i][j] = (unsigned char *) malloc(3 * sizeof(unsigned char));
        }
    }
    fseek(fp, fileHeader.bfOffBits, SEEK_SET);
    unsigned char *rowData = (unsigned char *) malloc(row_padded * sizeof(unsigned char));
    for (int i = 0; i < height; i++) {
        fread(rowData, sizeof(unsigned char), row_padded, fp);
        for (int j = 0; j < width; j++) {
            int index = j * 3;
            img[i][j][0] = rowData[index];
            img[i][j][1] = rowData[index + 1];
            img[i][j][2] = rowData[index + 2];
            if (img[i][j][2] != 255) {
                img[i][j][0] = 255;
                img[i][j][1] = 255;
                img[i][j][2] = 255;
            }
        }
    }
    free(rowData);
    fclose(fp);
    for (int i = 0; i < height / 2; i++) {
        unsigned char **temp = img[i];
        img[i] = img[height - 1 - i];
        img[height - 1 - i] = temp;
    }
    *leftAngel = 0;
    *rightAngel = 0;
    int tempNum, dot = 1;
    int h = 60;
    int w = 148;
    int l = 0;
    for (; w < 500; w += 13) {
        if (img[h + 15][w + 7][0] != 255 && img[h + 3][w + 3][0] != 255) {
            tempNum = 1;
        } else if (img[h + 5][w + 10][0] != 255 && img[h + 11][w + 6][0] != 255 && img[h + 17][w + 2][0] != 255) {
            tempNum = 2;
        } else if (img[h + 6][w + 9][0] != 255 && img[h + 2][w + 2][0] != 255) {
            tempNum = 3;
        } else if (img[h][w + 9][0] != 255 && img[h + 3][w + 6][0] != 255) {
            tempNum = 4;
        } else if (img[h + 7][w + 3][0] != 255 && img[h + 8][w + 3][0] != 255) {
            tempNum = 5;
        } else if (img[h][w + 10][0] != 255 && img[h + 2][w + 4][0] != 255) {
            tempNum = 6;
        } else if (img[h + 2][w + 11][0] != 255 && img[h][w + 2][0] != 255) {
            tempNum = 7;
        } else if (img[h + 1][w + 3][0] != 255 && img[h + 3][w + 2][0] != 255 && img[h + 1][w + 10][0] != 255) {
            tempNum = 8;
        } else if (img[h + 1][w + 3][0] != 255 && img[h + 10][w + 4][0] != 255) {
            tempNum = 9;
        } else if (img[h + 10][w + 2][0] != 255 && img[h][w + 5][0] != 255 && img[h + 6][w + 2][0] != 255) {
            tempNum = 0;
        } else if (img[h + 15][w + 3][0] != 255 && img[h + 16][w + 3][0] != 255 && img[h + 15][w + 4][0] != 255 &&
                   img[h + 17][w + 3][0] != 255) {
            w -= 6;
            dot = 0;
            continue;
        } else {
            break;
        }
        if (dot)
            *leftAngel = (*leftAngel) * 10 + tempNum;
        else
            *leftAngel += tempNum * pow(10, --l);

    }
    l = 0;
    dot = 1;
    h = 89;
    w = 162;
    for (; w < 600; w += 13) {
        if (img[h + 15][w + 7][0] != 255 && img[h + 3][w + 3][0] != 255) {
            tempNum = 1;
        } else if (img[h + 5][w + 10][0] != 255 && img[h + 11][w + 6][0] != 255 && img[h + 17][w + 2][0] != 255) {
            tempNum = 2;
        } else if (img[h + 6][w + 9][0] != 255 && img[h + 2][w + 2][0] != 255) {
            tempNum = 3;
        } else if (img[h][w + 9][0] != 255 && img[h + 3][w + 6][0] != 255) {
            tempNum = 4;
        } else if (img[h + 7][w + 3][0] != 255 && img[h + 8][w + 3][0] != 255) {
            tempNum = 5;
        } else if (img[h][w + 10][0] != 255 && img[h + 2][w + 4][0] != 255) {
            tempNum = 6;
        } else if (img[h + 2][w + 11][0] != 255 && img[h][w + 2][0] != 255) {
            tempNum = 7;
        } else if (img[h + 1][w + 3][0] != 255 && img[h + 3][w + 2][0] != 255 && img[h + 1][w + 10][0] != 255) {
            tempNum = 8;
        } else if (img[h + 1][w + 3][0] != 255 && img[h + 10][w + 4][0] != 255) {
            tempNum = 9;
        } else if (img[h + 10][w + 2][0] != 255 && img[h][w + 5][0] != 255 && img[h + 6][w + 2][0] != 255) {
            tempNum = 0;
        } else if (img[h + 15][w + 3][0] != 255 && img[h + 16][w + 3][0] != 255 && img[h + 15][w + 4][0] != 255 &&
                   img[h + 17][w + 3][0] != 255) {
            w -= 6;
            dot = 0;
            continue;
        } else {
            break;
        }
        if (dot)
            *rightAngel = (*rightAngel) * 10 + tempNum;
        else
            *rightAngel += tempNum * pow(10, --l);
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            free(img[i][j]);
        }
        free(img[i]);
    }
    free(img);
    return 0;
}

int readFolder(char **bmpFiles, int maxFiles) {
    char folderPath[50];
    strcpy(folderPath, "./");

    struct dirent *entry;
    DIR *dir = opendir(folderPath);
    int count = 0;

    if (dir == NULL) {
        printf("Could not open directory: %s\n", folderPath);
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".bmp") != NULL) {
            if (count < maxFiles) {
                bmpFiles[count] = strdup(entry->d_name);
                count++;
            } else {
                break;
            }
        }
    }

    closedir(dir);
    return count;
}

void readFile(double *angel1, double *angel2, char **bmpFile, int bmpCount) {
    for (int i = 0; i < bmpCount; ++i) {
        double leftAngel;
        double rightAngel;
        getData(&rightAngel, &leftAngel, bmpFile[i]);
        angel1[i] = leftAngel;
        angel2[i] = rightAngel;
    }
}

void sort(double *angel1, double *angel2, char **photoName, int *angelCount) {
    double difference[10000];

    for (int i = 0; i < *angelCount; ++i) {
        difference[i] = fabs(angel1[i] - angel2[i]);
    }


    for (int i = 0; i < *angelCount - 1; i++) {
        for (int j = 0; j < *angelCount - i - 1; j++) {
            if (difference[j] > difference[j + 1]) {
                double tempDiff = difference[j];
                difference[j] = difference[j + 1];
                difference[j + 1] = tempDiff;

                double tempAngel1 = angel1[j];
                angel1[j] = angel1[j + 1];
                angel1[j + 1] = tempAngel1;

                double tempAngel2 = angel2[j];
                angel2[j] = angel2[j + 1];
                angel2[j + 1] = tempAngel2;

                char *tempPhoto = photoName[j];
                photoName[j] = photoName[j + 1];
                photoName[j + 1] = tempPhoto;
            }
        }
    }
    int newCount = 0;
    for (int i = 0; i < *angelCount; i++) {
        if (i == 0 || (angel1[i] != angel1[i - 1] || angel2[i] != angel2[i - 1])) {

            angel1[newCount] = angel1[i];
            angel2[newCount] = angel2[i];
            photoName[newCount] = photoName[i];
            newCount++;
        }
    }

    *angelCount = newCount;
}



int getName(const char *time) {
    int hours, minutes, seconds;
    if (sscanf(time, "%*d/%*d/%*d %d:%d:%d", &hours, &minutes, &seconds) == 3) {
        return hours * 10000 + minutes * 100 + seconds;
    }

    return -1;
}

int checkStart(const char *str) {
    char *endptr;
    double val = strtod(str, &endptr);

    return (endptr != str && *endptr == '\0');
}

void readXml(int *check, int name, char **nameXml, int *nameCount , double angel1 , double angel2 , double angel1List[] , double angel2List[], int *lastSavedName) {
    int checkCount = 0;
    int rowCount = 0;
    char line[1000000];
    char photoName[50];

    FILE *fptr = fopen("sheet.xml", "r");
    if (!fptr) {
        printf("File error, can't read!\n");
        return;
    }


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
                } else if (checkStart(start) == 0 && rowCount == 4) {

                    int value = getName(start);
                    if (name == value) {

                        if ((fabs(angel1 - angel1List[checkCount]) < EPSILON) &&fabs(angel2 - angel2List[checkCount]) < EPSILON && (*lastSavedName)!=name){
                            *lastSavedName= name;
                            nameXml[*nameCount] = malloc(strlen(start) + 1);
                            if (!nameXml[*nameCount]) {
                                printf("Memory allocation error!\n");
                                fclose(fptr);
                                return;
                            }
                            strcpy(nameXml[*nameCount], start);
                            (*nameCount)++;
                            check[checkCount] = 0;
                        }
                    }
                    checkCount++;
                } else if (strtod(start, NULL) && rowCount == 7) {

                    int value = getName(start);
                    if (name == value) {
                        if ((fabs(angel1 - angel1List[checkCount]) < EPSILON) &&fabs(angel2 - angel2List[checkCount]) < EPSILON  && (*lastSavedName)!=name){
                            *lastSavedName = name;
                            nameXml[*nameCount] = malloc(strlen(start) + 1);
                            if (!nameXml[*nameCount]) {
                                printf("Memory allocation error!\n");
                                fclose(fptr);
                                return;
                            }
                            strcpy(nameXml[*nameCount], start);
                            (*nameCount)++;
                            check[checkCount] = 0;
                        }

                    }
                    checkCount++;
                }
            }
            start = strstr(end + 1, "<x:v>");
        }
    }



    fclose(fptr);

}

void readXmlAngel(double *angel1List , double *angel2List ){
    int rowCount =0;
    char line[1000000];
    FILE *fptr;
    int angelCount =0;
    fptr = fopen("sheet.xml", "r");
    if (fptr == NULL) printf("File error, can't read! \n");
    else {
        printf("Printing file content:\n");

        while (fgets(line, sizeof(line), fptr)) {
            char *start = strstr(line, "<x:v>");
            while (start) {
                start += 5;
                char *end = strstr(start, "</x:v>");
                if (end) {
                    *end = '\0';
                    rowCount++;
                    double  value =strtod(start, NULL);
                    if (strtod(start, NULL) == 0.0f) {
                        rowCount = 0;

                    } else if (angel1List[0]==0.0f && value>10 ){
                        angel1List[angelCount]=value;

                    }else if (angel2List[0]==0.0f && value>10){
                        angel2List[angelCount]=value;
                        angelCount++;


                    }
                    else if (strtod(start, NULL) && rowCount == 4) {
                        angel1List[angelCount] =value;



                    } else if (strtod(start, NULL) && rowCount == 5) {
                        angel2List[angelCount] = value;
                        angelCount++;


                    }


                }
                start = (end) ? strstr(end + 1, "<x:v>") : NULL;
            }
        }

        fclose(fptr);
    }
    printf(" the number of element  is %i\n", angelCount);
}

void findRows(char **bmpfile, int transforNumber, int *check, char **namexml, int *namexmlCount , double angel1[] ,double angel2[]) {
    double angel1List[10000]={0.0};
    double angel2List[10000]={0.0};
    int lastSavedName = -1;
    readXmlAngel(angel1List , angel2List);

    for (int i = 0; i < transforNumber; ++i) {
        int name = strtol(bmpfile[i], NULL, 10);
        readXml(check, name, namexml, namexmlCount , angel1[i], angel2[i],angel1List , angel2List ,&lastSavedName);
    }


}

void extractDateTime(char *row, char *dateTime) {
    char *start = strstr(row, "<x:v>");
    while (start) {
        start += 5;
        char *end = strstr(start, "</x:v>");
        if (!end) break;

        size_t len = end - start;
        if (len < 100) {
            strncpy(dateTime, start, len);
            dateTime[len] = '\0';
        }

        if (strchr(dateTime, '/') && strchr(dateTime, ':')) {
            return;
        }

        start = strstr(end + 1, "<x:v>");
    }
}


int removeLineFromXML(char **bmpFile, int *check, int transformNumber , double angel1[] , double angel2[]) {
    char rowStart[] = "<x:row>";
    char rowEnd[] = "</x:row>";
    int namexmlCount = 0;
    // Initialize check array
    for (int i = 0; i < 10000; i++) {
        check[i] = 1;
    }

    char *nameXml[10000] = {NULL};
    findRows(bmpFile, transformNumber, check, nameXml, &namexmlCount , angel1 ,angel2);


    const char *filename = "sheet.xml";
    const char *newFilename = "sheet_modified.xml";

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    FILE *output = fopen(newFilename, "wb");
    if (!output) {
        perror("Error creating output file");
        fclose(fp);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    char *buffer = malloc(filesize + 1);
    if (!buffer) {
        perror("Memory allocation error");
        fclose(fp);
        fclose(output);
        return 1;
    }

    fread(buffer, 1, filesize, fp);
    buffer[filesize] = '\0';
    fclose(fp);

    char *pos = buffer;
    char *row_start, *row_end;
    int rowCount = 0;
    while ((row_start = strstr(pos, rowStart)) != NULL) {
        row_end = strstr(row_start, rowEnd);
        if (!row_end) break;
        row_end += strlen(rowEnd);

        char datetime[100] = "";


        char rowBuffer[5000];
        snprintf(rowBuffer, sizeof(rowBuffer), "%.*s", (int) (row_end - row_start), row_start);

        extractDateTime(rowBuffer, datetime);

        if (rowCount >=3) {
            int row_should_be_kept = 0;

            if(check[rowCount-3]==0){
                row_should_be_kept=1;
                printf(">>> Match Found: %d\n",rowCount);


            }
            if (row_should_be_kept) {
                fwrite(pos, 1, row_start - pos, output);
                fwrite(row_start, 1, row_end - row_start, output);
            }

        } else {
            fwrite(pos, 1, row_start - pos, output);
            fwrite(row_start, 1, row_end - row_start, output);
        }


        pos = row_end;
        rowCount++;
    }


    fwrite(pos, 1, strlen(pos), output);
    fclose(output);
    free(buffer);

    remove(filename);
    rename(newFilename, filename);

    printf("File modified successfully: %s\n", filename);
    return 0;
}


void changePhotoName(double angel1, double angel2, char *name) {
    if (name == NULL) {

        return;
    }
    int len = strlen(name);
    for (int i = len - 4; i < len; ++i) {
        name[i] = '\0';
    }
    char angel1Char[50];
    char angel2Char[50];
    sprintf(angel1Char, "_%.2f", angel1);
    sprintf(angel2Char, "_%.2f", angel2);
    strcat(name, angel1Char);
    strcat(name, angel2Char);
    strcat(name, ".bmp");


}


void transferPhoto(char **photoName, int number, unsigned char ***logo, int logoHeight, int logoWidth, double angel1[],
                   double angel2[], int bmpCount) {
    mkdir("results");
    for (int i = 0; i < number; ++i) {
        char name[50];
        sprintf(name, "%s", photoName[i]);

        readImage(logo, logoHeight, logoWidth, name);
    }

    char source[50], destination[50];
    for (int i = 0; i < number; ++i) {


        sprintf(source, "%s", photoName[i]);
        changePhotoName(angel1[i], angel2[i], photoName[i]);
        sprintf(destination, "results/%s", photoName[i]);
        if (access(source, F_OK) != -1) {

            if (rename(source, destination) == 0) {
                printf("Moved: %s -> %s\n", source, destination);
            } else {
                perror("Error moving file");
            }
        } else {
            printf("Source file %s does not exist.\n", source);

        }
    }


}

unsigned char ***readLogo(int *height, int *width) {

    struct bitMapFileHeader fileHeader;
    struct bitMapInfoHeader infoHeader;


    memcpy(&fileHeader, logo_data, sizeof(struct bitMapFileHeader));
    memcpy(&infoHeader, logo_data + sizeof(struct bitMapFileHeader), sizeof(struct bitMapInfoHeader));

    *width = infoHeader.biWidth;
    *height = abs(infoHeader.biHeight);
    int row_padded = ((*width) * 3 + 3) & (~3);
    unsigned char ***logo = (unsigned char ***) malloc((*height) * sizeof(unsigned char **));
    if (!logo) return NULL;

    for (int i = 0; i < *height; i++) {
        logo[i] = (unsigned char **) malloc((*width) * sizeof(unsigned char *));
        if (!logo[i]) return NULL;

        for (int j = 0; j < *width; j++) {
            logo[i][j] = (unsigned char *) malloc(3 * sizeof(unsigned char));
            if (!logo[i][j]) return NULL;
        }
    }


    unsigned char *rowData = (unsigned char *) malloc(row_padded);
    if (!rowData) return NULL;

    int offset = fileHeader.bfOffBits;


    for (int i = 0; i < *height; i++) {
        int reversedIndex = *height - 1 - i;
        memcpy(rowData, logo_data + offset + (reversedIndex * row_padded), row_padded);
        for (int j = 0; j < *width; j++) {
            int index = j * 3;
            logo[i][j][0] = rowData[index];
            logo[i][j][1] = rowData[index + 1];
            logo[i][j][2] = rowData[index + 2];
        }
    }

    free(rowData);
    return logo;
}


int readImage(unsigned char ***logo, int logoHeight, int logoWidth, char *inputFilename) {
    char *outputFilename = inputFilename;

    FILE *fp = fopen(inputFilename, "rb");
    if (fp == NULL) {
        printf("such image is not exict\n");
        return 1;
    }
    struct bitMapFileHeader fileHeader;
    fread(&fileHeader, sizeof(struct bitMapFileHeader), 1, fp);
    struct bitMapInfoHeader infoHeader;
    fread(&infoHeader, sizeof(struct bitMapInfoHeader), 1, fp);
    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight);
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char ***img = (unsigned char ***) malloc(height * sizeof(unsigned char **));
    for (int i = 0; i < height; i++) {
        img[i] = (unsigned char **) malloc(width * sizeof(unsigned char *));
        for (int j = 0; j < width; j++) {
            img[i][j] = (unsigned char *) malloc(3 * sizeof(unsigned char));
        }
    }
    fseek(fp, fileHeader.bfOffBits, SEEK_SET);
    unsigned char *rowData = (unsigned char *) malloc(row_padded * sizeof(unsigned char));
    for (int i = 0; i < height; i++) {
        fread(rowData, sizeof(unsigned char), row_padded, fp);
        for (int j = 0; j < width; j++) {
            int index = j * 3;
            img[i][j][0] = rowData[index];
            img[i][j][1] = rowData[index + 1];
            img[i][j][2] = rowData[index + 2];
        }
    }
    free(rowData);
    fclose(fp);
    for (int i = 0; i < height / 2; i++) {
        unsigned char **temp = img[i];
        img[i] = img[height - 1 - i];
        img[height - 1 - i] = temp;
    }
    int startX = 640, startY = 574;


    if (startX + logoWidth > width || startY + logoHeight > height) {
        printf("Error: Logo does not fit within the image boundaries.\n");
        return 1;
    }


    for (int y = 0; y < logoHeight; y++) {
        for (int x = 0; x < logoWidth; x++) {

            img[startY + y][startX + x][0] = logo[y][x][0];
            img[startY + y][startX + x][1] = logo[y][x][1];
            img[startY + y][startX + x][2] = logo[y][x][2];

        }
    }
    infoHeader.biHeight = height;
    fileHeader.bfSize = fileHeader.bfOffBits + row_padded * height;
    FILE *fp_out = fopen(outputFilename, "wb");
    if (!fp_out) return 1;
    fwrite(&fileHeader, sizeof(struct bitMapFileHeader), 1, fp_out);
    fwrite(&infoHeader, sizeof(struct bitMapInfoHeader), 1, fp_out);
    rowData = (unsigned char *) malloc(row_padded * sizeof(unsigned char));
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            int index = j * 3;
            rowData[index] = img[i][j][0];
            rowData[index + 1] = img[i][j][1];
            rowData[index + 2] = img[i][j][2];
        }
        for (int k = width * 3; k < row_padded; k++) {
            rowData[k] = 0;
        }
        fwrite(rowData, sizeof(unsigned char), row_padded, fp_out);
    }
    free(rowData);
    fclose(fp_out);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            free(img[i][j]);
        }
        free(img[i]);
    }
    free(img);
    printf("the logo successfully added to the %s\n", inputFilename);
    return 0;
}




void getXmlName(char **zipName) {
    char folderPath[50];
    strcpy(folderPath, "./");

    struct dirent *entry;
    DIR *dir = opendir(folderPath);
    int count = 0;

    if (dir == NULL) {
        printf("Could not open directory: %s\n", folderPath);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if ((strstr(entry->d_name, ".xls") != NULL) || (strstr(entry->d_name, ".xlsx") != NULL)) {

            *zipName = strdup(entry->d_name);
            printf("The zip file %s founded\n", *zipName);
            break;
        }
    }

    closedir(dir);

}


void zipToXls(char *zipName) {
    int BUFFER_SIZE = 1024;
    FILE *sourceFile, *destFile;
    unsigned char buffer[BUFFER_SIZE];
    size_t bytesRead;

    sourceFile = fopen(zipName, "rb");
    if (sourceFile == NULL) {
        perror("Error opening source file");
        return;
    }

    char xlsName[300] = "results/";
    char baseName[256];

    const char *filePart = strrchr(zipName, '/');
    if (filePart) {
        filePart++;
    } else {
        filePart = zipName;
    }

    strncpy(baseName, filePart, sizeof(baseName) - 1);
    baseName[sizeof(baseName) - 1] = '\0';
    char *ext = strrchr(baseName, '.');
    if (ext != NULL) {
        strcpy(ext, ".xlsx");
    } else {
        strcat(baseName, ".xlsx");
    }

    strncat(xlsName, baseName, sizeof(xlsName) - strlen(xlsName) - 1);

    destFile = fopen(xlsName, "wb");
    if (destFile == NULL) {
        perror("Error opening destination file");
        fclose(sourceFile);
        return;
    }

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);

    printf("File extracted successfully as: %s\n", xlsName);
}

void copyFile(const char *source, const char *destination) {
    FILE *src, *dest;
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    src = fopen(source, "rb");
    if (src == NULL) {
        perror("Error opening source file");
        return;
    }

    dest = fopen(destination, "wb");
    if (dest == NULL) {
        perror("Error creating destination file");
        fclose(src);
        return;
    }


    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(src);
    fclose(dest);
}

void backupPhotos(char **image_files, int count, const char *xlsx_file) {
    mkdir("backup");
    for (int i = 0; i < count; i++) {
        char destination[512];
        snprintf(destination, sizeof(destination), "%s/%s", "backup", image_files[i]);
        copyFile(image_files[i], destination);
    }
    if (xlsx_file != NULL) {
        char xlsx_destination[512];
        snprintf(xlsx_destination, sizeof(xlsx_destination), "backup/%s", xlsx_file);
        copyFile(xlsx_file, xlsx_destination);
    }

    printf("Backup action completed successfully\n");

}

void restoreImages(char **image_files, int count, const char *xlsx_file) {
    for (int i = 0; i < count; i++) {
        char backup_path[512], original_path[512];

        snprintf(backup_path, sizeof(backup_path), "%s/%s", "backup", image_files[i]);
        snprintf(original_path, sizeof(original_path), "%s", image_files[i]);
        copyFile(backup_path, original_path);
    }

    if (xlsx_file != NULL) {
        char xlsx_backup_path[512], xlsx_original_path[512];

        snprintf(xlsx_backup_path, sizeof(xlsx_backup_path), "backup/%s", xlsx_file);
        snprintf(xlsx_original_path, sizeof(xlsx_original_path), "%s", xlsx_file);

        copyFile(xlsx_backup_path, xlsx_original_path);
    }

    printf("Image files and XLSX file restored successfully\n");


}


void deleteBackupFolder() {
    char backup[100] = "backup";
    DIR *dir = opendir(backup);
    struct dirent *entry;
    char file_path[512];

    if (dir == NULL) {
        perror("Error opening backup directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(file_path, sizeof(file_path), "%s/%s", backup, entry->d_name);
        if (remove(file_path) == 0) {

        } else {
            perror("Error deleting file");
        }
    }

    closedir(dir);


    if (rmdir(backup) == 0) {
        printf("Backup folder deleted successfully.\n");
    } else {
        perror("Error deleting backup folder");
    }
}


int main() {
    char *bmpFiles[10000];
    char *backupBmpFiles[10000];
    int bmpCount = readFolder(bmpFiles, 10000);
    int backupBmpCount = bmpCount;
    char *fileName;
    getXmlName(&fileName);
    backupPhotos(bmpFiles, backupBmpCount, fileName);
    int checkRun = system("cd png2bmp &&  png2bmp.exe");

    if (checkRun == 0) {
        printf("png images converted to bmp\n");
    } else {
        printf("faild to convert png to bmp");
    }

    int checkExtracZip = system("extractZip.exe");
    if (checkExtracZip == 0) {
        printf("XML extracted\n");
    } else {
        printf("XML Not extracted");
    }



    if (bmpCount == 0) {
        printf("No BMP files found in the folder.\n");
    }

    double angel1[10000] = {0};
    double angel2[10000] = {0};
    int check[10000];
    for (int i = 0; i < 10000; ++i) {
        check[i] = 1;
    }
    for (int i = 0; i < bmpCount; ++i) {
        backupBmpFiles[i] = strdup(bmpFiles[i]);
    }


    int angelCount = 0;
    int transferNumber = 10;


    int logoHigh;
    int logoWidth;

    readFile(angel1, angel2, bmpFiles, bmpCount);

    sort(angel1, angel2, bmpFiles, &bmpCount);



    unsigned char ***logo = readLogo(&logoHigh, &logoWidth);

    if (logo != NULL) {
        printf("the logo read succesfully\n");
    } else {
        printf("the logo cant read properly\n");
    }
    transferPhoto(bmpFiles, transferNumber, logo, logoHigh, logoWidth, angel1, angel2, bmpCount);
    removeLineFromXML(bmpFiles, check, transferNumber,angel1 , angel2);

    for (int i = 0; i < logoHigh; i++) {
        for (int j = 0; j < logoWidth; j++) {
            free(logo[i][j]);
        }
        free(logo[i]);
    }
    free(logo);
    //moveXml(fileName, "sheet.xml", "xl/worksheets/sheet.xml");

    int checkXmlToZip = system("xmlToZip.exe");
    if (checkXmlToZip == 0) {
        printf("XML converted to Zip\n");
    } else {
        printf("XML Not converted to Zip");
    }
    zipToXls(fileName);
    restoreImages(backupBmpFiles, backupBmpCount, fileName);
    deleteBackupFolder();
    int checkConvert = system("cd png2bmp &&  bmp2png.exe");

    if (checkConvert == 0) {
        printf("bmp images converted to png\n");
    } else {
        printf("faild to convert bmp to png\n");
    }
    char end;
    scanf("%s", &end);
    return 0;
}

