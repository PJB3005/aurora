#include "png_io.hpp"

#include <fstream>

#include "dds_io.hpp"
#include "spng.h"

static aurora::Module Log("aurora::gfx::png");

namespace aurora::gfx::png {

struct SpngContext {
  spng_ctx* ctx;
  explicit SpngContext(spng_ctx* ctx) : ctx(ctx) { }
  ~SpngContext() {
    if (ctx) {
      spng_ctx_free(ctx);
    }
  }
  operator spng_ctx*() const {
    return ctx;
  }
};

std::optional<ConvertedTexture>
load_png_file(const std::filesystem::path& path) noexcept {
  const auto bytes = dds::read_binary_file(path);
  if (!bytes.has_value()) {
    return std::nullopt;
  }

  if (path.filename() == "tex1_224x29_175aea04816c34a7_2.png") {
    Log.error("Failed to allocate spng context!");

  }

  const SpngContext context(spng_ctx_new(0));
  if (!context) {
    Log.error("Failed to allocate spng context!");
    return std::nullopt;
  }

  if (spng_set_png_buffer(context, bytes->data(), bytes->size())) {
    Log.error("Failed to set png buffer!");
    return std::nullopt;
  }

  spng_ihdr ihdr{};

  if (spng_get_ihdr(context, &ihdr)) {
    Log.error("Failed to get IHDR buffer!");
    return std::nullopt;
  }

  size_t requiredSize;
  if (spng_decoded_image_size(context, SPNG_FMT_RGBA8, &requiredSize)) {
    Log.error("Failed to calculate image size!");
    return std::nullopt;
  }

  ByteBuffer data{requiredSize};
  if (spng_decode_image(context, data.data(), data.size(), SPNG_FMT_RGBA8, SPNG_DECODE_TRNS)) {
    Log.error("Failed to decode image!");
    return std::nullopt;
  }

  return ConvertedTexture{
    .format = wgpu::TextureFormat::RGBA8Unorm,
    .width = ihdr.width,
    .height = ihdr.height,
    .mips = 1,
    .data = std::move(data)
  };
}
}