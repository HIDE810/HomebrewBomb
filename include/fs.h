#include <3ds.h>

#ifndef FS_H
#define FS_H

extern FS_Archive archive, nand_archive;

Result FS_OpenArchive(FS_Archive *archive, FS_ArchiveID id);
Result FS_CloseArchive(FS_Archive archive);
Result FS_RenameDir(FS_Archive archive, const char *old_dirname, const char *new_dirname);

#endif