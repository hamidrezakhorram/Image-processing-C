#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <unzip.h>
#include <zip.h>
int fileExistsZip(unzFile zip, const char *fileCheck) {
    if (unzGoToFirstFile(zip) != UNZ_OK) return 0;  // Move to first file in ZIP
    char filename_in_zip[256];

    do {
        unz_file_info file_info;
        unzGetCurrentFileInfo(zip, &file_info, filename_in_zip, sizeof(filename_in_zip), NULL, 0, NULL, 0);

        if (strcmp(filename_in_zip, fileCheck) == 0) {
            return 1;
        }
    } while (unzGoToNextFile(zip) == UNZ_OK);

    return 0;
}
int copyExistingFiles(zipFile newZip, const char *oldZipFileName, const char *skipFile) {
    int CHUNK = 16384;
    unzFile old_zip = unzOpen(oldZipFileName);
    if (!old_zip) return 0;

    char filename_in_zip[256];
    char buffer[CHUNK];

    if (unzGoToFirstFile(old_zip) != UNZ_OK) return 0;

    do {

        unz_file_info file_info;
        unzGetCurrentFileInfo(old_zip, &file_info, filename_in_zip, sizeof(filename_in_zip), NULL, 0, NULL, 0);


        if (strcmp(filename_in_zip, skipFile) == 0) continue;


        if (unzOpenCurrentFile(old_zip) != UNZ_OK) return 0;


        if (zipOpenNewFileInZip(newZip, filename_in_zip, NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED,
                                Z_DEFAULT_COMPRESSION) != ZIP_OK) {
            unzCloseCurrentFile(old_zip);
            return 0;
        }


        int bytes_read;
        while ((bytes_read = unzReadCurrentFile(old_zip, buffer, CHUNK)) > 0) {
            zipWriteInFileInZip(newZip, buffer, bytes_read);
        }


        unzCloseCurrentFile(old_zip);
        zipCloseFileInZip(newZip);
    } while (unzGoToNextFile(old_zip) == UNZ_OK);

    unzClose(old_zip);
    return 1;
}

int moveXml(const char *zipFileName, const char *xmlFilenName, const char *zipEentryName) {
    int CHUNK = 16384;
    unzFile oldZip = unzOpen(zipFileName);
    int fileExists = 0;

    if (oldZip) {
        fileExists = fileExistsZip(oldZip, zipEentryName);
        unzClose(oldZip);
    }


    zipFile newZip = zipOpen("temp.zip", APPEND_STATUS_CREATE);
    if (!newZip) {
        fprintf(stderr, "Failed to open new ZIP file\n");
        return 1;
    }

    copyExistingFiles(newZip, zipFileName, fileExists ? zipEentryName : NULL);

    FILE *file = fopen(xmlFilenName, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open XML file\n");
        zipClose(newZip, NULL);
        return 1;
    }

    if (zipOpenNewFileInZip(newZip, zipEentryName, NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION) !=
        ZIP_OK) {
        fprintf(stderr, "Failed to add file to ZIP\n");
        fclose(file);
        zipClose(newZip, NULL);
        return 1;
    }

    char buffer[CHUNK];
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, CHUNK, file)) > 0) {
        zipWriteInFileInZip(newZip, buffer, bytes_read);
    }

    fclose(file);
    zipCloseFileInZip(newZip);
    zipClose(newZip, NULL);

    remove(zipFileName);
    rename("temp.zip", zipFileName);

    printf("Successfully %s %s in %s\n", fileExists ? "replaced" : "added", zipEentryName, zipFileName);
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
int main() {
    char *fileName;
    getXmlName(&fileName);
    moveXml(fileName, "sheet.xml", "xl/worksheets/sheet.xml");
    return 0;
}
