#include "GtkImage.hpp"

GtkBufImage::GtkBufImage(std::string fn) { read(fn); }

GtkBufImage::~GtkBufImage() {}

void GtkBufImage::read(std::string fn) { _pixBuff = Gdk::Pixbuf::create_from_file(fn); }

unsigned int GtkBufImage::pixelInt(unsigned int row, unsigned int col) {
  guchar *pixels = _pixBuff->get_pixels(), *p = nullptr;

  auto rowstrid = _pixBuff->get_rowstride();
  auto n_channels = _pixBuff->get_n_channels();

  p =  pixels + row * rowstrid + col * n_channels;

  return static_cast<unsigned int>(p[0]);
}

unsigned int GtkBufImage::height() { return (unsigned int)_pixBuff->get_height(); }

unsigned int GtkBufImage::width() { return (unsigned int)_pixBuff->get_width(); }
