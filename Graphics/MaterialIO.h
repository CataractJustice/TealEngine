#include "Kirino/Material.h"
#include <filesystem>

namespace Kirino 
{
	#define MaterialFormatExtension ".mtrl"
	namespace MaterialFormatMagic 
	{
		const unsigned int v1x = 0x3939fade;
	}

	struct MaterialFormatHeaderV1 
	{
		unsigned int magic;
		unsigned int version;
		char name[256];
		char shader[256];
		unsigned int samplerCount;
		unsigned int uniformsCount;
	};

	class MaterialIO 
	{
		public:
		struct MaterialFileInfo 
		{
			std::string name;
			bool valid;
		};
		static void load(Kirino::Material& material, std::string& name, const std::filesystem::path& path);
		static void save(const Kirino::Material& material, const std::string& name, const std::filesystem::path& path);
		static MaterialFileInfo info(const std::filesystem::path& path);
	};

	class MaterialLoaderJson 
	{
		static void load(std::filesystem::path path);
	};

	class MaterialLoaderV1 
	{
		public:
		enum class LoadError 
		{
			Ok,
			FileTooShort,
			InvalidFormatStart,
			InvalidVersion,
			InvalidSamplerCount,
			InvalidUniformCount,
			UnknownShader
		};

		struct LoadStatus 
		{
			bool ok;
			LoadError loadError;
		};
		static LoadStatus load(Kirino::Material& material, std::string& name, std::filesystem::path path);
		static void save(const Kirino::Material& material, const std::string& name, std::filesystem::path path);
		static MaterialIO::MaterialFileInfo info(const std::filesystem::path& path);
	};
}