#pragma once
#include "BinaryWriter.h"
#include <cstdint>
#include <filesystem>

#pragma pack(push)  
#pragma pack(1)

struct EntryInfo {
	uint64_t runtimeID;
	uint64_t offset;
	const uint32_t zsize; //compressed size

	EntryInfo(const uint64_t& runtimeID);
	void serialize(BinaryWriter& bw) const;
};

struct EntryDescriptor {
	char type[4];
	const uint32_t chunk_info_size;
	const uint32_t chunk_size;
	const uint32_t size;
	const uint32_t mem_size;
	const uint32_t video_mem_size;

	EntryDescriptor(const char* type, const uint32_t& size);
	void serialize(BinaryWriter& bw) const;
};

struct RpkgHeader {
	const char magic[4];
	const uint32_t file_count;
	const uint32_t info_offset;
	const uint32_t info_size;
	const uint32_t override_hash_table_size;

	RpkgHeader(uint32_t file_count);
	void serialize(BinaryWriter& bw) const;
};

class PkgFile {
	std::unique_ptr<EntryInfo> entry_info;
	std::unique_ptr<EntryDescriptor> entry_descriptor;
	std::unique_ptr<char[]> data;

public:
	PkgFile(std::filesystem::path file_path);
	PkgFile(PkgFile&& pf);
	~PkgFile();

	const EntryInfo& getEntryInfo() const;
	const EntryDescriptor& getEntryDescriptor() const;
	void serializeData(BinaryWriter& bw) const;
	void setOffset(uint32_t off);
};

class Rpkg {
	std::unique_ptr<RpkgHeader> header;
	std::vector<PkgFile> files;

public:
	Rpkg(std::filesystem::path directory_path);

	size_t getFileCount() const;
	void printFileList() const;
	void writeToFile(std::filesystem::path dst_path) const;
};

#pragma pack(pop)

