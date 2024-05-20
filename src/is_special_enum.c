#include "../include/xml_synchronize.h"
#include "../include/hash_map.h"
#include <libxml2/libxml/tree.h>
#include <stdio.h>

bool is_special_enum(const xmlChar* name, const char* special_enum){
    FILE* file = fopen(special_enum, "r");
    if (file == NULL) {
        perror("Error opening special_enum file");
        return false;
    }
    char line[MAX_LENGTH];
    while (fscanf(file, "%s", line) != EOF) {
        if (xmlStrcmp(name, (xmlChar*)line) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}
