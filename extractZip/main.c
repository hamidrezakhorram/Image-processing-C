#include <stdio.h>
#include <unzip.h>
#include <zip.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
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
int main() {
    char *fileName;
    getXmlName(&fileName);
    const char *fileAddress = "xl/worksheets/sheet.xml";
    extractZip(fileName, fileAddress);
    return 0;
}
