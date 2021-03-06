#include "stdafx.h"
#include "Rpkg.h"
#include <filesystem>
#include <iostream>
#include <Windows.h>

#define VERSION "1.00"

namespace fs = std::filesystem;

void invalidInput();

int main(int argc, char *argv[])
{
	printf("\n*** B3's Hitman 2 RPKGPatch builder (v%s) ***\n\n", VERSION);

	struct PatchBuilderContext {
		fs::path in;
		fs::path out;
	} ctx;

	switch (argc) {
	case 2: //./ZHM5PatchBuilder.exe ..\\FolderWithFilesToPack\\ 
		if (!fs::is_directory(argv[1])) {
			invalidInput();
			return EXIT_FAILURE;
		}
		
		ctx.in = argv[1];
		if (fs::path(argv[1]).has_filename())
			ctx.in += "\\";
		ctx.out = ctx.in.parent_path();
		ctx.out += ".rpkg";
		break;

	case 3: //./ ZHM5PatchBuilder.exe ..\\FolderWithFilesToPack\\ ..\\ExportPath\\chunk0patch4.rpkg
		if (!fs::is_directory(argv[1]) || !fs::path(argv[2]).has_filename()) {
			invalidInput();
			return EXIT_FAILURE;
		}

		ctx.in = argv[1];
		if (fs::path(argv[1]).has_filename())
			ctx.in += "\\";
		ctx.out = argv[2];
		break;

	default:
		invalidInput();
		return EXIT_FAILURE;
	}
	
	Rpkg rpkg(ctx.in);
	printf("Packed %zd file(s) into patch archive:\n", rpkg.getFileCount());
	rpkg.printFileList();
	printf("\n");

	rpkg.writeToFile(ctx.out);
	printf("Exported to %s\n\n", ctx.out.generic_string().c_str());

	return EXIT_SUCCESS;
}

void invalidInput() {
	printf("Invalid arguments!\n\n");
	printf("Example usage:\n\t./ZHM5PatchBuilder.exe ..\\FolderWithFilesToPack\\");
	printf("\n\t./ZHM5PatchBuilder.exe ..\\FolderWithFilesToPack\\ ..\\ExportPath\\chunk0patch4.rpkg");
	printf("\n\nPress Enter to exit...");
	std::cin.ignore();
}

