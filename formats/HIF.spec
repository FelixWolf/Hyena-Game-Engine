/*
    This format is considered public domain, without any patent, with
    the following restriction:
    Use of the format is permitted as is, but if you alter the format,
    you must change the header from HIF to something else, as to prevent
    format incompatiblities.
*/

//Always little endian

//ONLY G8, GA88, RGB8888, and RGBA8888 support non-power of 2 values.
enum colorModes{
    pal = 0,
    g8,
    ga88,
    ga81,
    ga44,
    a8,
    a1
    rgb888,
    rgba8888,
    rgba8881,
    rgb565,
    rgba5651,
    rgba4444
}

enum flags{
    nearest = 0, //Default is linear
    mips, //Only works with power of 2 images
    fullbright, //Should ignore lighting
    ignore_z, //ALWAYS render on top of every other polygon.
    compressed //If set, each mipmap is compressed with glib.
}

{
    //Basics
    char[3] "HIF" //Magic number
    /*
        If version_major is changed, then a new loader version is
            required, due to something like format change/rearrange.
            Loaders SHOULD NOT attempt to load files if it is not supported.
        If version_minor is changed, it is likely to backwards compatable
            with the exception that it likely has a new color mode, or flag.
            Loaders MAY attempt to load, but must throw an error if unsupported colour mode.
    */
    unsigned short version_major
    unsigned short version_minor
    
    //Image info
    unsigned short width  //Should be power of 2
    unsigned short height //Should be power of 2
    unsigned int flags
    unsigned char colorMode
    
    if pal:
        for(i = 0; i < 255; i++)
            unsigned char color_for_palette
    
    if flags has mips set: //If not set, mips is 1.
        unsigned char mips
    
    //Starting with largest image first to smallest
    mip[mips]{
        unsigned short width
        unsigned short height
        unsigned int data_length
        if pal:
            for(i = 0, l = width*height; i < l; i++)
                unsigned char color_from_palette
                
        if g8:
            for(i = 0, l = width*height; i < l; i++)
                unsigned char grey
                
        if g88:
            for(i = 0, l = (width*height)*2; i < l; i = i + 2)
                unsigned char grey
                unsigned char alpha
                
        if g81:
            for(i = 0, l = width*height; i < l; i++)
                unsigned char grey
                
            for(i = 0, l = (width*height)/8; i < l; i++)
                unsigned char alpha(bits)
                
        if g44:
            for(i = 0, l = width*height; i < l; i++)
                unsigned char greyalpha(0xF0 = grey, 0x0F = alpha)
                
        if a8:
            for(i = 0, l = width*height; i < l; i++)
                unsigned char alpha
                
        if a1:
            for(i = 0, l = (width*height)/8; i < l; i++)
                unsigned char alpha(bits)
                
        if rgb888:
            for(i = 0, l = (width*height)*3; i < l; i = i + 3)
                unsigned char red
                unsigned char green
                unsigned char blue
                
        if rgb8888:
            for(i = 0, l = (width*height)*4; i < l; i = i + 4)
                unsigned char red
                unsigned char green
                unsigned char blue
                unsigned char alpha
                
        if rgb8881:
            for(i = 0, l = (width*height)*3; i < l; i = i + 3)
                unsigned char red
                unsigned char green
                unsigned char blue
                
            for(i = 0, l = (width*height)/8; i < l; i++)
                unsigned char alpha(bits)
                
        if rgb565:
            for(i = 0, l = (width*height)*3; i < l; i = i + 3)
                unsigned short color
                red   = (color & 0x001F) << 3
                green = (color & 0x07E0) >> 3
                blue  = (color & 0xF800) >> 8
                
        if rgb5651:
            for(i = 0, l = (width*height)*3; i < l; i = i + 3)
                unsigned short color
                red   = (color & 0x001F) << 3
                green = (color & 0x07E0) >> 3
                blue  = (color & 0xF800) >> 8
                
            for(i = 0, l = (width*height)/8; i < l; i++)
                unsigned char alpha(bits)
                
        if rgb4444:
            for(i = 0, l = (width*height)*2; i < l; i = i + 2)
                unsigned short color
                red   = (a >> 12) & 0xF
                green = (a >> 8 ) & 0xF
                blue  = (a >> 4 ) & 0xF
                alpha = color     & 0xF
    }
}
