
#include "ObjParser.h"
#include "BinaryOutput.h"
#include "../Squidley/Logger.h"
#include "ConfigReader.h"

int main()
{
	Logger::initialize("..\\Data\\ObjConverterLog.html");
	ConfigReader::initialize();
	ObjParser parse;
	BinaryOutput writer;
	parse.vertexFormat = (short)ConfigReader::getIntFromString(ConfigReader::findNeededValueForKey("SceneOutputFormat"));
	parse.colorOverride = (char*)ConfigReader::findValueForKey("OutputColor");
	parse.textureOverride = (char*)ConfigReader::findValueForKey("OutputTexture");
	parse.normalOverride = (char*)ConfigReader::findValueForKey("OutputNormal");
	parse.readTextFile(ConfigReader::findValueForKey("CloneInput"));
	writer.writeCustomBinaryFile(ConfigReader::findValueForKey("CloneOutput"), parse.vertexFormat, parse.indCount, parse.indCount, parse.finalVerts, parse.finalColors, parse.finalTextures, parse.finalNormals, parse.finalIndices);
	Logger::shutDown();
}