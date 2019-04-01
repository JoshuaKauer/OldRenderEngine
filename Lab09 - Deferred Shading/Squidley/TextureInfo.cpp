#include "TextureInfo.h"
#include <fstream>

const unsigned long FOURCC_DXT1 = 0x31545844;
const unsigned long FOURCC_DXT3 = 0x33545844;
const unsigned long FOURCC_DXT5 = 0x35545844;

void TextureInfo::loadBMP_custom(const char* imagepath)
{
	//Data read from the header of the BMP file
	unsigned char header[54]; //Each BMP file begins with a 54-byte header
	unsigned int dataPos; //Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize; // = width * height * 3
	//Actual RGB data
	unsigned char* data;

	//Open the file
	FILE* file;// = fopen(imagepath, "rb");
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		printf("Image could not be opened\n"); 
		exit(1);
	}
	if (fread(header, 1, 54, file) != 54)
	{
		//If not 54 bytes read there is a problem
		printf("Not a correct BMP file\n");
		exit(1);
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file\n");
		exit(1);
	}

	//Read ints form the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	//Some BMP files are misformatted, gues the missing information
	if (imageSize == 0)
		imageSize = width * height * 3; //3 : one byte for each Red, Green, and Blue component
	if (dataPos == 0)
		dataPos = 54; //The BMP header is done that way

	//Create a buffer
	data = new unsigned char[imageSize];

	//Read the actual data from the into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	//GLuint textureID;
	glGenTextures(1, &textureID);

	//"Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureInfo::loadNormalMap(const char* imagePath)
{
	glGenTextures(1, &normalMapID);

	unsigned int width, height;
	unsigned char* image = 0;

	readBMP(&width, &height, image, imagePath);

	glBindTexture(GL_TEXTURE_2D, normalMapID);

	//Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureInfo::loadParallaxMap(const char* imagePath)
{
	glGenTextures(1, &parallaxMapID);

	unsigned int width, height;
	unsigned char* image = 0;

	readBMP(&width, &height, image, imagePath);

	glBindTexture(GL_TEXTURE_2D, parallaxMapID);

	//Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureInfo::readBMP(unsigned int * width, unsigned int * height, unsigned char * &image, const char* imagepath)
{
	//Data read from the header of the BMP file
	unsigned char header[54]; //Each BMP file begins with a 54-byte header
	unsigned int dataPos; //Position in the file where the actual data begins
	unsigned int imageSize; // = width * height * 3
							//Actual RGB data

	//Open the file
	FILE* file;// = fopen(imagepath, "rb");
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		printf("Image could not be opened\n");
		exit(1);
	}
	if (fread(header, 1, 54, file) != 54)
	{
		//If not 54 bytes read there is a problem
		printf("Not a correct BMP file\n");
		exit(1);
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file\n");
		exit(1);
	}

	//Read ints form the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);

	//Some BMP files are misformatted, gues the missing information
	if (imageSize == 0)
		imageSize = *width * *height * 3; //3 : one byte for each Red, Green, and Blue component
	if (dataPos == 0)
		dataPos = 54; //The BMP header is done that way

					  //Create a buffer
	image = new unsigned char[imageSize];

	//Read the actual data from the into the buffer
	fread(image, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);
}

void TextureInfo::loadCubeMap(const GLchar* faces[])
{
    glGenTextures(1, &textureID);

    unsigned int width,height;
    unsigned char* image = 0;
	
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint j = 0; j < 6; ++j)
    {
		readBMP(&width, &height, image, faces[j]);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
		image = 0;
		width = height = 0;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

//DXT compression comes from the DirectX world, where the V texture coordinate is inversed compared to OpenGL.So if
//you use compressed textures, you’ll have to use ( coord.u, 1.0-coord.v) to fetch the correct texel. You can do this 
//whenever you want : in your export script, in your loader, in your shader…

void TextureInfo::LoadDDS(const char* imagepath)
{
	unsigned char header[124];

	FILE *fp;

	//try to pen the file
	fopen_s(&fp, imagepath, "rb");
	if (fp == NULL)
	{
		printf("Image could not be opened\n");
		exit(1);
	}

	//verify the type of file
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0)
	{
		fclose(fp);
		printf("Not a correct DDS file\n");
		exit(1);
	}

	fread(&header, 124, 1, fp);

	unsigned int height		= *(unsigned int*) &(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int *) & (header[80]);

	unsigned char* buffer;
	unsigned int bufsize;

	//how big is it going to be including all mipmaps?
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	//close the file pointer
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	components;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		printf("Could not convert flag into opengl understanding");
		exit(1);
	}

	glGenTextures(1, &textureID);

	//"Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// load the mipmaps
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}

	free(buffer);
}