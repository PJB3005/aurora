#ifndef AURORA_DVD_H
#define AURORA_DVD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <dolphin/types.h>

/**
 * Open a GC/Wii disc image for use by the DVD API.
 * Must be called before DVDInit().
 * Returns true on success, false on failure.
 */
bool aurora_dvd_open(const char* disc_path);

/**
 * Close the disc image and free all resources.
 */
void aurora_dvd_close(void);

typedef struct AuroraOverlayFile {
  const char* fileName;
  void* userData;
  size_t size;
} AuroraOverlayFile;

void aurora_dvd_overlay_files(const AuroraOverlayFile* files, size_t nFiles);

typedef struct AuroraOverlayCallbacks {
  void* (*open)(void* userdata);
  void (*close)(void* handle);
  int64_t (*read)(void* handle, uint8_t *buf, size_t len);
  int64_t (*seek)(void* handle, int64_t offset, int32_t whence);
} AuroraOverlayCallbacks;

void aurora_dvd_overlay_callbacks(const AuroraOverlayCallbacks* callbacks);

#ifdef __cplusplus
}
#endif

#endif
