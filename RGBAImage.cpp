/*
RGBAImage.cpp
Copyright (C) 2006 Yangli Hector Yee

This program is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program;
if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "RGBAImage.h"
#include "tiff.h"
#include "tiffio.h"
#include <stdio.h>

// Reads Tiff Images
RGBAImage* RGBAImage::ReadTiff(char *filename)
{
	RGBAImage *fimg = 0;
	
    TIFF* tif = TIFFOpen(filename, "r");
    char emsg[1024];
    if (tif) {
		TIFFRGBAImage img;
		
		if (TIFFRGBAImageBegin(&img, tif, 0, emsg)) {
			size_t npixels;
			uint32* raster;

			npixels = img.width * img.height;
			raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
			if (raster != NULL) {
				if (TIFFRGBAImageGet(&img, raster, img.width, img.height)) {
					// result is in ABGR
					fimg = new RGBAImage(img.width, img.height);
					for (int y = 0; y < (int) img.height; y++) {
						for (int x = 0; x < (int) img.width; x++) {
						   fimg->Set(x,y, raster[x + y * img.width]);
						}
					}
				}
			_TIFFfree(raster);
			}
	    }
	    TIFFRGBAImageEnd(&img);
	} else {
	    TIFFError(filename, emsg);
	}
	return fimg;
}

bool RGBAImage::WritePPM()
{	
	if (Width <= 0) return false;
	if (Height <=0 ) return false;
	FILE *out = fopen(Name.c_str(), "wb");
	if (!out) return false;
	fprintf(out, "P6\n%d %d 255\n", Width, Height);
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			int i = x + (Height - y - 1) * Width;
			unsigned char r = Get_Red(i);
			unsigned char g = Get_Green(i);
			unsigned char b = Get_Blue(i);
			fwrite(&r, 1, 1, out);
			fwrite(&g, 1, 1, out);
			fwrite(&b, 1, 1, out);
		}
	}
	fclose(out);
	return true;
}