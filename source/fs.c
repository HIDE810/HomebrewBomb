#include "fs.h"

FS_Archive archive, nand_archive;

static void FS_UTF8_To_UTF16(u16 *buf, const u8 *input, size_t bufsize) {
	ssize_t units = utf8_to_utf16(buf, input, bufsize);

	if (units < 0)
		units = 0;

	buf[units] = 0;
}

Result FS_OpenArchive(FS_Archive *archive, FS_ArchiveID archiveID) {
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_OpenArchive(archive, archiveID, fsMakePath(PATH_EMPTY, ""))))
		return ret;

	return 0;
}

Result FS_CloseArchive(FS_Archive archive) {
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_CloseArchive(archive)))
		return ret;

	return 0;
}

Result FS_RenameDir(FS_Archive archive, const char *old_dirname, const char *new_dirname) {
	Result ret = 0;

	u16 old_dirname_u16[strlen(old_dirname) + 1];
	FS_UTF8_To_UTF16(old_dirname_u16, (const u8 *)old_dirname, strlen(old_dirname) + 1);

	u16 new_dirname_u16[strlen(new_dirname) + 1];
	FS_UTF8_To_UTF16(new_dirname_u16, (const u8 *)new_dirname, strlen(new_dirname) + 1);

	if (R_FAILED(ret = FSUSER_RenameDirectory(archive, fsMakePath(PATH_UTF16, old_dirname_u16), archive, fsMakePath(PATH_UTF16, new_dirname_u16))))
		return ret;
	
	return 0;
}