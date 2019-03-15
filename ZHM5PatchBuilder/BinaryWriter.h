#pragma once
#include <fstream>
#include <filesystem>

class BinaryWriter {
	std::ofstream ofs;

public:
	BinaryWriter(const std::filesystem::path& file) {
		try {
			ofs.open(file, std::ios::binary | std::ios::out);
		}
		catch (std::ofstream::failure e) {
			printf("Failed to open %s", file.generic_string().c_str());
			exit(0);
		}
	};
	~BinaryWriter() {
		ofs.close();
	};

	template<typename T>
	void write(T& val) {
		ofs.write((const char*)&val, sizeof(T));
	};

	template<typename T>
	void write(T* arr, unsigned long long len) {
		ofs.write(reinterpret_cast<const char*>(arr), len);
	}

};