#include "DLLoader.h"
#include "Debug.h"
#include "FileSystem/getDirectoryList.h"

#include <string>
#include <vector>
#include <filesystem>
#include <system_error>
#include <format>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

namespace TealEngine
{
	namespace fs = std::filesystem;

	namespace
	{
		bool isDynamicLibrary(const std::string& name)
		{
		#ifdef _WIN32
			constexpr const char* extension = ".dll";
		#elif defined(__APPLE__)
			constexpr const char* extension = ".dylib";
		#else
			constexpr const char* extension = ".so";
		#endif

		const std::size_t extLength = std::char_traits<char>::length(extension);
		bool ext2Match = false;

			return name.size() > extLength &&
					name.compare(name.size() - extLength, extLength, extension) == 0 ||
					ext2Match;
		}

		std::string getLibraryError()
		{
		#ifdef _WIN32
			const DWORD errorCode = GetLastError();

			LPSTR errorMessage = nullptr;

			const DWORD length = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				errorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&errorMessage),
				0,
				nullptr
			);

			std::string message = length && errorMessage
				? std::string(errorMessage, length)
				: "Unknown Windows error: " + std::to_string(errorCode);

			if (errorMessage)
				LocalFree(errorMessage);

			return message;
		#else
			const char* error = dlerror();
			return error ? error : "Unknown dynamic library error";
		#endif
		}
	}

	void DLLoader::loadDir(const std::string& path)
	{
		const fs::path directory(path);
		const std::vector<std::string> dlList = getDirectoryList(path);

		for (const std::string& name : dlList)
		{
			if (!isDynamicLibrary(name))
				continue;

			const fs::path libraryPath = directory / name;
			const std::string fullPath = libraryPath.string();
		#ifdef _WIN32
			HMODULE dlhandle = LoadLibraryA(fullPath.c_str());

			if (!dlhandle)
			{
				std::string errorText = getLibraryError();
				throw std::runtime_error(std::format("Failed to load library: {}.", errorText));
				continue;
			}

			handles[fullPath] = reinterpret_cast<void*>(dlhandle);

		#else
			// Clear any previous dynamic loader error.
			dlerror();

			void* dlhandle = dlopen(fullPath.c_str(), RTLD_NOW);

			if (!dlhandle)
			{
				std::string errorText = getLibraryError();
				throw std::runtime_error(std::format("Failed to load library: {}.", errorText));
				continue;
			}

			handles[fullPath] = dlhandle;
		#endif
		}
	}

	void DLLoader::unloadAll()
	{
		for (const auto& handle : handles)
		{
		#ifdef _WIN32
			FreeLibrary(reinterpret_cast<HMODULE>(handle.second));
		#else
			dlclose(handle.second);
		#endif
		}

		handles.clear();
		mtime.clear();
	}

	DLLoader::~DLLoader()
	{
		unloadAll();
	}

	bool DLLoader::checkUpdates()
	{
		for (const auto& handle : handles)
		{
			const std::string& libraryPath = handle.first;

			std::error_code ec;

			const auto currentTime = fs::last_write_time(
				libraryPath,
				ec
			);

			if (ec)
				continue;

			const auto it = mtime.find(libraryPath);

			if (it == mtime.end() || it->second != currentTime.time_since_epoch().count())
				return true;
		}

		return false;
	}
}