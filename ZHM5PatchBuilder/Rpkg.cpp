#pragma once
#include "stdafx.h"
#include "Rpkg.h"
#include "BinaryWriter.h"
#include <cstdint>
#include <filesystem>

EntryInfo::EntryInfo(const uint64_t& runtimeID) : runtimeID(runtimeID), offset(0), zsize(0) {

}

void EntryInfo::serialize(BinaryWriter& bw) const {
	bw.write(runtimeID);
	bw.write(offset);
	bw.write(zsize);
}

EntryDescriptor::EntryDescriptor(const char* type, const uint32_t& size) :
	chunk_info_size(0),
	chunk_size(0),
	size(size),
	mem_size(size),
	video_mem_size(0) {
	memcpy_s(this->type, sizeof(this->type), type + 1, sizeof(this->type)); // +1 because filesystem::extension() returns extension with "." 
};

void EntryDescriptor::serialize(BinaryWriter& bw) const {
	//type string is little endian.
	bw.write(&type[3], 1);
	bw.write(&type[2], 1);
	bw.write(&type[1], 1);
	bw.write(&type[0], 1);

	bw.write(chunk_info_size);
	bw.write(chunk_size);
	bw.write(size);
	bw.write(mem_size);
	bw.write(video_mem_size);
}

PkgFile::PkgFile(std::filesystem::path file_path) {
	std::ifstream ifs(file_path, std::ios::in | std::ios::binary | std::ios::ate);
	uint32_t file_size = static_cast<uint32_t>(ifs.tellg());

	std::string runtime_id = file_path.stem().generic_string();
	entry_info = std::make_unique<EntryInfo>(std::stoull(runtime_id, nullptr, 16));
	entry_descriptor = std::make_unique<EntryDescriptor>(file_path.extension().generic_string().c_str(), file_size);

	ifs.seekg(0, std::ios::beg);
	data = std::unique_ptr<char[]>(new char[file_size]);
	ifs.read(data.get(), file_size);

	ifs.close();
}

PkgFile::PkgFile(PkgFile&& pf) {
	entry_descriptor = std::move(pf.entry_descriptor);
	entry_info = std::move(pf.entry_info);
	data = std::move(pf.data);
}

PkgFile::~PkgFile() {

}

const EntryInfo& PkgFile::getEntryInfo() const {
	return *entry_info;
}

const EntryDescriptor& PkgFile::getEntryDescriptor() const {
	return *entry_descriptor;
}

void PkgFile::serializeData(BinaryWriter& bw) const {
	bw.write(data.get(), entry_descriptor->size);
}

void PkgFile::setOffset(uint32_t off) {
	entry_info->offset = off;
}

RpkgHeader::RpkgHeader(uint32_t file_count) :
	magic{ 'G', 'K', 'P', 'R' },
	file_count(file_count),
	info_offset(file_count * sizeof(EntryInfo)),
	info_size(sizeof(EntryDescriptor)*file_count),
	override_hash_table_size(0) {

	}

void RpkgHeader::serialize(BinaryWriter& bw) const {
	bw.write(magic, sizeof(magic));
	bw.write(file_count);
	bw.write(info_offset);
	bw.write(info_size);
	bw.write(override_hash_table_size);
}

Rpkg::Rpkg(std::filesystem::path directory_path) {
	uint32_t file_count = static_cast<uint32_t>(std::count_if(
		std::filesystem::directory_iterator(directory_path),
		std::filesystem::directory_iterator(),
		static_cast<bool(*)(const std::filesystem::path&)>(std::filesystem::is_regular_file))
	);

	header = std::make_unique<RpkgHeader>(file_count);

	for (const auto& file : std::filesystem::directory_iterator(directory_path)) {
		if (!std::filesystem::is_regular_file(file))
			continue;
		files.push_back(PkgFile(file.path()));
	}

	//Go through file list and calculate all the file data offsets
	uint32_t cur_offset = static_cast<uint32_t>(sizeof(RpkgHeader) + files.size() * sizeof(EntryDescriptor) + files.size() * sizeof(EntryInfo));
	for (size_t i = 0; i < files.size(); i++) {
		files[i].setOffset(cur_offset);
		cur_offset += files[i].getEntryDescriptor().size;
	}
}

size_t Rpkg::getFileCount() const {
	return files.size();
};

void Rpkg::printFileList() const {
	printf("File list:\n");
	for (const auto& f : files)
	{
		printf("\t%I64X.%s\n", f.getEntryInfo().runtimeID, f.getEntryDescriptor().type);
	}
}

void Rpkg::writeToFile(std::filesystem::path dst_path) const {
	BinaryWriter bw(dst_path);

	header->serialize(bw);

	for (const auto& f : files)
		f.getEntryInfo().serialize(bw);

	for (const auto& f : files)
		f.getEntryDescriptor().serialize(bw);

	for (const auto& f : files)
		f.serializeData(bw);
}