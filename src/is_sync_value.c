#include "../include/xml_synchronize.h"

int is_sync_value(xmlChar* name, unsigned char names[MAX_LINES][MAX_NAME_LENGTH]){
    for(int i=0;i<MAX_LINES;i++){
        if(xmlStrEqual(name, (xmlChar*)names[i]) == 1)
            return i;
    }
    return -1;
}
