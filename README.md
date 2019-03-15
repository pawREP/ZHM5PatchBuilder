# Hitman 2 Rpkg Patch Builder

This tool allows the building of stand-alone patch archives for Hitman 2 (2018). Files found in patch archives shadow all files with the same runtimeID in archives lower in the hierarchy. As a consequence, the use of patch archives makes the modification of native game archives obsolete which in turn makes the process of modding Hitman 2 a lot easier and quicker. All file formats are supported.

The patch builder accepts an arbitrary number (within reason) of files and packs them into a single patch archive. The game also doesn't seem to impose any restrictions on the number of patch files you can use at the same time, which allows the creation of mods with a very modular structure. This is not only great for testing, but also allows for great compatibility between mods as longs as two patch archives don't modify the same files.



### How to use:
 - Unpack the game archive you want to modify with the included QuickBMS script `Hitman2_rpkg.bms`.
 - Find the files you want to modify and copy them into a separate folder. The unpacked files are named after their runtimeID and can't be renamed.
 - Modify the files to your liking.
 - Pass the folder with the modified files into the patch builder to pack them into a patch file. You can do this by either dragging the folder onto the exe or by using `./ZHM5PatchBuilder.exe ..\FolderWithFilesToPack\ [..\\ExportPath\\chunk0patch4.rpkg]`
 - Copy the newly generated patchrpkg into the game's runtime folder and name it appropriately. If you are building a `chunk0` patch file for example and the highest numbered patch in your folder is `chunk0patch3.rpkg` then name your patch file `chunk0patch4.rpkg`.
 - Decrypt `HITMAN2\Runtime\packagedefinition.txt` with `h6xxtea.exe` and adjust the `patchlevel` of the archive you generated a patch for to match the highest patch number. The game will just ignore all patch archives it can't find so if you don't want to worry about this step in the future, you can just set all the patchlevels to some high value. You can find the xxtea decryption tool at https://github.com/HHCHunter/HITMAN/tree/master/h6xxtea.

### Known issues :
 - Some patches will trigger a "read error" during startup. The cause is not entirely clear at the moment but the error didn't have any effect on the functionality of any of the mods I tested so far.
Let me know if you come across any other issues.

### Important note about online play:
Patches generated with this tool do not and are not intended to work during online play. For obvious reasons, I'll not provide any further info or help regarding this issue.

### License
MIT Licence
