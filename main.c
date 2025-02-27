#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <unzip.h>
#include <zip.h>
#include "logo_data.h"

#pragma pack(push, 1)

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
    //const char *inputFilename = nameFile;
    FILE *fp = fopen(inputFilename, "rb");
    if (!fp) {
        printf("Error: Unable to open file %s\n", inputFilename);
        return -1; // Error code
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

void sort(double angel1[], double angel2[], char **photoName, const int angelCount) {
    double difference[400];
    for (int i = 0; i < angelCount; ++i) {
        difference[i] = fabs(angel1[i] - angel2[i]);
    }
    for (int i = 0; i < angelCount - 1; i++) {
        for (int j = 0; j < angelCount - i - 1; j++) {
            if (difference[j] > difference[j + 1]) {

                double tempDiff = difference[j];
                difference[j] = difference[j + 1];
                difference[j + 1] = tempDiff;


                char *tempPhoto = photoName[j];
                photoName[j] = photoName[j + 1];
                photoName[j + 1] = tempPhoto;
            }
        }
    }

}

void extractZip(const char *fileName, const char *fileAddress) {
    unzFile zipFile = unzOpen(fileName);
    if (!zipFile) {
        printf("Could not open ZIP file: %s\n", fileName);
        return;
    } else if (unzLocateFile(zipFile, fileAddress, 0) != UNZ_OK) {
        printf("File %s not found in ZIP archive\n", fileAddress);
        return;

    } else if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
        printf("Could not open file inside ZIP\n");
        unzClose(zipFile);
        return;
    }
    FILE *fptr;

    fptr = fopen("sheet.xml", "wb");


    char buffer[4096];
    int bytes_read;
    while ((bytes_read = unzReadCurrentFile(zipFile, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytes_read, fptr);
    }
    fclose(fptr);
    printf("File extracted successfully \n");
    unzCloseCurrentFile(zipFile);
    unzClose(zipFile);
}

int getName(const char *time) {
    int hours, minutes, seconds;
    if (sscanf(time, "%*d/%*d/%*d %d:%d:%d", &hours, &minutes, &seconds) == 3) {
        return hours * 10000 + minutes * 100 + seconds;
    }

    return -1;
}

void readXml(int *check, int name, char **nameXml, int *nameCount) {
    int checkCount = 0;
    int rowCount = 0;
    char line[4096];
    char photoName[50];

    FILE *fptr;
    fptr = fopen("sheet.xml", "r");
    if (fptr == NULL) printf("File error, can't read! n");
    else {


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


                    } else if (strtod(start, NULL) && rowCount == 4) {
                        sprintf(photoName, "%d", getName(start));

                        int value = getName(start);
                        if (name == value) {
                            check[checkCount] = 0;

                            nameXml[*nameCount] = malloc(strlen(start) + 1);
                            if (!nameXml[*nameCount]) {
                                printf("Memory allocation error!\n");
                                fclose(fptr);
                                return;
                            }
                            strcpy(nameXml[*nameCount], start);
                            (*nameCount)++;
                            (checkCount)++;
                            return;
                        } else {

                            (checkCount)++;
                        }

                    } else if (strtod(start, NULL) && rowCount == 7) {
                        sprintf(photoName, "%d", getName(start));

                        int value = getName(start);
                        if (name == value) {

                            nameXml[*nameCount] = malloc(strlen(start) + 1);
                            if (!nameXml[*nameCount]) {
                                printf("Memory allocation error!\n");
                                fclose(fptr);
                                return;
                            }
                            strcpy(nameXml[*nameCount], start);

                            (*nameCount)++;
                            check[checkCount] = 0;
                            (checkCount)++;
                            return;

                        } else {

                            (checkCount)++;
                        }

                    }
                }
                start = (end) ? strstr(end + 1, "<x:v>") : NULL;
            }


        }


        fclose(fptr);
    }
    printf("The photo row doesn't exist on the XML\n");

}

void findRows(char **bmpfile, int transforNumber, int *check, char **namexml) {
    int nameCount = 0;
    for (int i = 0; i < transforNumber; ++i) {
        int name = strtol(bmpfile[i], NULL, 10);
        readXml(check, name, namexml, &nameCount);
    }


}

int removeLineFromXML(char **bmpFile, int *check, int transformNumber) {
    char rowStart[20] = "<x:row>";
    char rowEnd[20] = "</x:row>";
    for (int i = 0; i < 3000; i++) {
        check[i] = 1;
    }
    char *nameXml[400] = {NULL};
    int count = 0;
    int rowCount = 0;
    findRows(bmpFile, transformNumber, check, nameXml);
    const char *filename = "file.xml";

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


    char *output = malloc(2 * filesize + 1);
    if (!output) {
        perror("Memory allocation error");
        free(buffer);
        return 1;
    }
    output[0] = '\0';


    char *pos = buffer;
    char *row_start, *row_end;
    while ((row_start = strstr(pos, rowStart)) != NULL) {
        strncat(output, pos, row_start - pos);

        row_end = strstr(row_start, rowEnd);

        if (!row_end) {
            break;
        }
        row_end += strlen(rowEnd);

        int row_removed = 1;


        for (int i = 0; i < transformNumber; ++i) {
            if (nameXml[i] != NULL && strstr(row_start, nameXml[i]) != NULL) {
                row_removed = 0;
                break;
            }
        }


        if (!row_removed) {
            strncat(output, row_start, row_end - row_start);
            printf("Row added to XML\n");
        }
        rowCount++;
        pos = row_end;

    }


    strcat(output, pos);


    fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening XML file for writing");
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


void transferPhoto(char **photoName, int number, unsigned char ***logo, int logoHeight, int logoWidth) {
    mkdir("photos");
    for (int i = 0; i < number; ++i) {
        char name[50];
        sprintf(name, "%s", photoName[i]);

        readImage(logo, logoHeight, logoWidth, name);
    }
    char source[50], destination[50];
    for (int i = 0; i < number; ++i) {
        sprintf(source, "%s", photoName[i]);
        sprintf(destination, "photos/%s", photoName[i]);
        if (access(source, F_OK) != -1) {
            // Move file
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
    int row_padded = ((*width) * 3 + 3) & (~3); // BMP row alignment
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
int copy_existing_files(zipFile new_zip, const char *old_zip_filename) {
    int CHUNK = 16384;
    unzFile old_zip = unzOpen(old_zip_filename);
    if (!old_zip) return 0; // If file doesn't exist, no need to copy

    char filename_in_zip[256];
    char buffer[CHUNK];

    if (unzGoToFirstFile(old_zip) != UNZ_OK) return 0;

    do {
        // Get file info
        unz_file_info file_info;
        unzGetCurrentFileInfo(old_zip, &file_info, filename_in_zip, sizeof(filename_in_zip), NULL, 0, NULL, 0);

        // Open file inside old ZIP
        if (unzOpenCurrentFile(old_zip) != UNZ_OK) return 0;

        // Add file to new ZIP
        if (zipOpenNewFileInZip(new_zip, filename_in_zip, NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION) != ZIP_OK) {
            unzCloseCurrentFile(old_zip);
            return 0;
        }

        // Copy file contents
        int bytes_read;
        while ((bytes_read = unzReadCurrentFile(old_zip, buffer, CHUNK)) > 0) {
            zipWriteInFileInZip(new_zip, buffer, bytes_read);
        }

        // Close current file
        unzCloseCurrentFile(old_zip);
        zipCloseFileInZip(new_zip);
    } while (unzGoToNextFile(old_zip) == UNZ_OK);

    unzClose(old_zip);
    return 1;
}

int moveXml(const char *zip_filename, const char *xml_filename ,const char *zip_entry_name) {
    int CHUNK = 16384;
    zipFile new_zip = zipOpen("temp.zip", APPEND_STATUS_CREATE);
    if (!new_zip) {
        fprintf(stderr, "Failed to open new ZIP file\n");
        return 1;
    }

    // Copy existing files if ZIP exists
    copy_existing_files(new_zip, zip_filename);

    // Open XML file
    FILE *file = fopen(xml_filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open XML file\n");
        zipClose(new_zip, NULL);
        return 1;
    }

    // Add new XML file
    if (zipOpenNewFileInZip(new_zip, zip_entry_name, NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION) != ZIP_OK) {
        fprintf(stderr, "Failed to add file to ZIP\n");
        fclose(file);
        zipClose(new_zip, NULL);
        return 1;
    }

    char buffer[CHUNK];
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, CHUNK, file)) > 0) {
        zipWriteInFileInZip(new_zip, buffer, bytes_read);
    }

    fclose(file);
    zipCloseFileInZip(new_zip);
    zipClose(new_zip, NULL);

    // Replace old ZIP with new ZIP
    remove(zip_filename);
    rename("temp.zip", zip_filename);

    printf("Successfully added %s as %s in %s\n", xml_filename, zip_entry_name, zip_filename);
    return 0;

}

int main() {

    int checkRun = system("cd png2bmp &&  png2bmp.exe");

    if (checkRun == 0) {
        printf("png images converted to bmp\n");
    } else {
        printf("faild to convert png to bmp");
    }


    char fileName[50];
    printf("Please enter the name of your ZIP file:\n");
    scanf("%s", fileName);
    const char *fileAddress = "xl/worksheets/sheet.xml";
    extractZip(fileName, fileAddress);
    char *bmpFiles[3000];
    int bmpCount = readFolder(bmpFiles, 3000);

    if (bmpCount == 0) {
        printf("No BMP files found in the folder.\n");
    }

    double angel1[400] = {0};
    double angel2[400] = {0};
    int check[3000];
    for (int i = 0; i < 3000; ++i) {
        check[i] = 1;
    }

    int angelCount = 0;
    int transferNumber = 10;


    int logoHigh;
    int logoWidth;

    readFile(angel1, angel2, bmpFiles, bmpCount);
    sort(angel1, angel2, bmpFiles, bmpCount);

    removeLineFromXML(bmpFiles, check, transferNumber);

    unsigned char ***logo = readLogo(&logoHigh, &logoWidth);

    if (logo != NULL) {
        printf("the logo read succesfully\n");
    } else {
        printf("the logo cant read properly\n");
    }
    transferPhoto(bmpFiles, transferNumber, logo, logoHigh, logoWidth);

    for (int i = 0; i < logoHigh; i++) {
        for (int j = 0; j < logoWidth; j++) {
            free(logo[i][j]);
        }
        free(logo[i]);
    }
    free(logo);
    moveXml(fileName ,"sheet.xml" , "xl/worksheets/sheet.xml");
    getchar();
    return 0;
}

