//Always big endian
//specification of types:

//Vectors
mvector{ //Mini-vector, 3 bytes
    minifloat x
    minifloat y
    minifloat z
}
hvector{ //Half-vector, 6 bytes
    half_float x
    half_float y
    half_float z
}
vector vector{ //Regular vector, 12 bytes
    float x
    float y
    float z
}
dvector vector{ //Double vector, 24 bytes
    double x
    double y
    double z
}
hvector vector{ //Huge vector, 32 bytes
    long double x
    long double y
    long double z
}

//Strings
small_string{ //Will be 1+length bytes
    unsigned byte length
    ascii[length]
}
string{ //Will be 2+length bytes
    ushort length
    ascii[length]
}

//Bitfields
bitfield{} //Regular bitfield, occupies 1 byte
dbitfield{} //Double bitfield, occupies 2 bytes

//Time
date{
    11111111 11111111 11111111
    YYYYYYYY YYYYYYYM MMMDDDDD
}

time{
    11111111 11111111
    HHHHHMMM MMMTTTTT
}
times{
    11111111 1111111 11111111
    HHHHHMMM MMMSSSS SSUUUUUU
}

//File format: v0.1
HEADER {
    char[3] "HMF" //This is the magic "string" or number
    ushort version //Specification version, formatted as float realversion = version/10
    bitfield options{
        1: has physics
        2: is rigged
        4: has extra data (Probably can be deprecated and reused, loader doesn't use this and will load it if there is data after the mesh)
        8: Is signed by compiler(Debugging information)
    }
    IF this.options.signed_by_compiler == 1:
        small_string compiler_name_plus_version
    unsigned byte BONE_count
    unsigned byte MESH_count //Each mesh is a LOD
    unsigned byte FACE_count //All meshes MUST have the same amount of faces
}

IF HEADER.BONE_count > 0:
    BONES {
        BONE[header.BONE_count]{ 
            small_string
            vector start_position
            vector end_position
        }
    }


//NOTICE: If the object has physics(HEADER->OPTIONS[bit(1)] is set), then the first object WILL be the physics shape
MESH {
    bitfield precision{ //Definition of vector_precision
        1 - 2: Vector precision {
            00: 8-bit precision (mini-vector)
            01: 16-bit precision (half-vector)
            10: 32-bit precision (vector)
            11: 64-bit precision (double vector)
        }
        3 - 4: UV precision {
            00: 8-bit precision (mini-vector)
            01: 16-bit precision (half-vector)
            10: 32-bit precision (vector)
            11: 64-bit precision (double vector)
        }
        5 - 6: Normal precision {
            00: 8-bit precision (mini-vector)
            01: 16-bit precision (half-vector)
            10: 32-bit precision (vector)
            11: 64-bit precision (double vector)
        }
    }
    FACE_DATA[header.FACE_count]{
        byte bitfield options{
            1: is duel sided
            2: is shaderless
            3: is smooth
            4: disable shadows
            5: UV is empty(UV values are not present and are ignored)
            6: Normals is empty(Normal values are not present and are generated on file load)
        }
        unsigned int vertices
        vertices[this.face_data.vertices]{
			vector_precision x, y, z
            if not this.face_data.normalsempty:
				vector_precision normals
        }
        unsigned int polygons
        polygons[this.face_data.polygons]{
            unsigned int vertice
            unsigned int vertice
            unsigned int vertice
            if not this.face_data.uvempty:
				vector_precision(2D) uv
        }
        if rigged:
			weights[this.face_data.vertices]{
				//This area is special. If we are given:
				//0x00 0xFF 0xFF, that means this vertice is fully
				//weighted to bone 0.
				//The last bone should always be 0xFF unless 4 weights
				//are specified, for example:
				//0x00 0x16 0x02 0x20 0x03 0xFF 0x04 0xFF
				//If there are less than 4 weights, then one bone will
				//be bone #255, or null_bone, which is essentially EOF
				while bone_id is not 0xFF:
					unsigned char bone_id
					unsigned char weight
			}
        small_string texture_file //Either a texture file, or the name of the face
    }
}

EXTRAS{
    unsigned byte extras_count
    EXTRA{
        unsigned byte extra_type{
            //Officially assigned extra types:
            0: Text/Script, etc: Highly recommended to NOT use for storing data such as author information
                {
                    small_string name
                    string data
                }
            1: Physics data: //There can ONLY be one of these
                {
                    bitfield options{
                        1: concave physics
                        2: scale mass
                        3: has mass offset
                    }
                    unsigned byte material_type
                    float mass
                    if this.options.has_mass_offset == 1:
                        vector mass_offset
                    double density
                    float drag
                }
            2: Animation: 
                {
                    unsigned int data_length
                    ANIMATION_DATA //See mesh_animation.fmt for specification
                }
            3: Texture data:
                {
                    small_string texture_name
                    char[4] texture_format //(EG: tga, png, jpg, vtf/vmf), use /x00 as padding
                    longword data_size
                    DATA texture_information
                }
            4: Particles: 
                {
                    bitfield options{
                        1: is attached to bone
                        2: is stopped
                        3: is visible to scripts
                        4: is accessable to scripts
                    }
                    if this.options.attached_to_bone == 1:
                        unsigned byte attached_bone_id
                    vector offset
                    vector rotation
                    unsigned int data_length
                    PARTICLE_DATA //See particle.fmt for specification
                }
            5: Sprite: 
                {
                    bitfield options{
                        1: is attached to bone
                        2: Ignore X axis
                        3: Ignore Y axis
                        4: Ignore Z axis
                        5: is shaderless
                        6: is visible to scripts
                        7: is accessable to scripts
                    }
                    if this.options.attached_to_bone == 1:
                        unsigned byte attached_bone_id
                    vector offset
                    float width
                    float height
                    float U
                    float V
                    float vis_distance
                    small_string texture_file //Either a texture file, or name
                }
            6: Rope: 
                {
                    bitfield options{
                        1: is attached to bone
                        2: Ignore X axis
                        3: Ignore Y axis
                        4: Try physics //Completely optional depending on engine, DO NOT RELY ON THIS
                        5: is shaderless
                        6: is visible to scripts
                        7: is accessable to scripts
                    }
                    if this.options.attached_to_bone == 1:
                        unsigned byte start_attached_bone_id
                        unsigned byte end_attached_bone_id
                    vector start_offset
                    vector end_offset
                    float width
                    float jellyness
                    float slack
                    small_string texture_file //Either a texture file, or name
                }
            7: Light
                {
                    bitfield options{
                        1: is attached to bone
                        2: is prelit
                        3: is visible to scripts
                        4: is accessable to scripts
                    }
                    if this.options.attached_to_bone == 1:
                        unsigned byte attached_bone_id
                    vector offset
                    vector rotation
                    mvector colour
                    hfloat distance
                    hfloat intensity
                    float radius //360.0 == fully surrounded
                }
            8: Entity: Stuff thats already scripted, this tells it to attach.   
                //IMPORTANT ABOUT ENTITY TYPE:
                //DO NOT ALLOW MODELS TO BE LOADED FROM A MODEL
                //THIS MAY RESULT IN A BILLION LAUGHS EXPLOIT
                //SEE: https://en.wikipedia.org/wiki/Billion_laughs
                //THIS INCLUDES ENTITIES WHERE MODELS CAN BE SET
                {
                    bitfield options{
                        1: is attached to bone
                    }
                    small_string entity_name
                    if this.options.attached_to_bone == 1:
                        unsigned byte attached_bone_id
                    vector offset
                    vector rotation
                    string(BINARY) entity_data
                }
            9-64: Reserved
            //Unofficial
            67-255: User defined
        }
        variable extra_data
    }
}
