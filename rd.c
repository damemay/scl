#include <stdio.h>
#include <string.h>
#include "scl.h"

char* scl_rd(const char* filepath, int nul_terminate, size_t* size) {
    FILE* file = fopen(filepath, "rb");
    if(!file) return NULL;
    fseek(file, 0L, SEEK_END);
    long f_size = ftell(file);
    *size = f_size;
    rewind(file);
    long m_size = f_size;
    if(nul_terminate) ++m_size;
    char* content = malloc(m_size);
    if(!content) {
        fclose(file);
	return NULL;
    }
    if(fread(content, f_size, 1, file) != 1) {
        fclose(file);
	free(content);
	return NULL;
    }
    fclose(file);
    if(nul_terminate) content[f_size] = '\0';
    return content;
}

char** scl_rdl(const char* filepath, size_t* len) {
    size_t size = 0;
    char* content = scl_rd(filepath, 1, &size);
    if(!content) return NULL;
    char** array = malloc(size+1);
    if(!array) {
	free(content);
	return NULL;
    }
    char* line = strtok(content, "\r\n");
    size_t i = 0;
    for(; line; i++) {
	size_t l = strlen(line)+1;
	array[i] = malloc(l);
	memcpy(array[i], line, l);
        line = strtok(NULL, "\r\n");
    }
    *len = i;
    return array;
}
