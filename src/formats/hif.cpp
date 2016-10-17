#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

struct hifHeader{
    unsigned char magic[3]; //Magic number
    unsigned char version_major;
    unsigned char version_minor;
    
    //Image info
    unsigned short width;  //Should be power of 2
    unsigned short height; //Should be power of 2
    unsigned int flags;
    unsigned char colorMode;
};

struct hifSize{
    unsigned short width;  //Should be power of 2
    unsigned short height; //Should be power of 2
};

enum colorModes{
    pal = 0,
    g8,
    ga88,
    ga81,
    ga44,
    a8,
    a1,
    rgb888,
    rgba8888,
    rgba8881,
    rgb565,
    rgba5651,
    rgba4444
};

enum flags{
    nearest = 0, //Default is linear
    mips, //Only works with power of 2 images
    fullbright, //Should ignore lighting
    ignore_z, //ALWAYS render on top of every other polygon.
    compressed //If set, each mipmap is compressed with glib.
};

class hif{
    int initialize();
    public:
        ~hif();
        unsigned char *data;
        unsigned char format = 255;
        unsigned int options = 0;
        unsigned char version_major = 0;
        unsigned char version_minor = 0;
        unsigned short width = 0;
        unsigned short height = 0;
        
        int read(const char*data);
        int readFile(const char*imagepath);
};

hif::~hif(){
    free(data);
}

int hif::initialize(){
    hifHeader *header = (hifHeader*)data;
    printf("HIF loaded: Version=%i.%i, width=%hu, height=%hu\n", header->version_major, header->version_minor, header->width, header->height);
    return 0;
}

int hif::read(const char *data){
    if(data[0]!='H' || data[1]!='I' || data[2]!='F'){
        printf("ERROR: Input is not a HIF file!\n");
        return 1;
    }
    
    initialize();
    return 0;
}

int hif::readFile(const char *imagepath){
    printf("Reading %s\n", imagepath);
    FILE *file = fopen(imagepath, "rb");
    if(!file){
        printf("ERROR: %s could not be opened!\n", imagepath);
        return 1;
    }
    
    unsigned char header[3];
    
    if(fread(header, 1, 3, file)!=3){
        printf("ERROR: %s is a invalid HIF file!\n", imagepath);
        fclose(file);
        return 2;
    }
    
    if(header[0]!='H' || header[1]!='I' || header[2]!='F'){
        printf("ERROR: %s is not a HIF file!\n", imagepath);
        fclose(file);
        return 3;
    }
    
    fseek(file, 0, SEEK_END);
    long lSize = ftell(file);
    rewind(file);
    
    printf("Size %i\n", lSize);
    data = (unsigned char*)malloc(sizeof(char)*lSize);
    if(data == NULL){
        printf("ERROR: Failed to allocate memory for %s!\n", imagepath);
        fclose(file);
        return 4;
    }
    
    size_t result = fread(data, 1, lSize, file);
    if(result != lSize){
        printf("ERROR: Failed to fully read %s!\n", imagepath);
        fclose(file);
        return 5;
    }
    
    fclose(file);
    initialize();
    return 0;
}
