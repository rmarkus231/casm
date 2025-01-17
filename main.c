#include "bfi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* read_file_to_string(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Error seeking in file");
        fclose(file);
        return NULL;
    }
    
    long file_size = ftell(file);
    if (file_size < 0) {
        perror("Error telling file size");
        fclose(file);
        return NULL;
    }
    rewind(file);
    
    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }
    
    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read < file_size) {
        if (ferror(file)) {
            perror("Error reading file");
            free(buffer);
            fclose(file);
            return NULL;
        }
    }
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

int main(int argc, char* argv[]){
	if(argc >= 2){
		// uses file provided as brainfuck code
		const char* filename = argv[1];
		char* prog = read_file_to_string(filename);
		if(argc == 3){
			if(!strcmp(argv[2],"asm")){
				get_asm(prog);
			}else{
				printf("what are you even trying to do here");
				return 1;
			}
		}else{
			interpret2(prog);
		}
	}else{
		printf("unsuitable combination of arguments");
		return 1;
	}
	return 0;
}
