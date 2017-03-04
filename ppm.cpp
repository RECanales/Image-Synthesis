#include "ppm.h"

// flip the image vertically to display correctly
void flipVertically(char *pixels, int width, int height, int bytes_per_pixel)
{
    int stride = width * bytes_per_pixel * 3;	// total amount of memory allocated for width
    char *row = (char*)malloc(stride);			// allocate stride and have it pointed to by row
    char *low = pixels;							// setting low to be at base of pixels memory
    char *high = &pixels[(height - 1) * stride];	// point to highest point of memory allocated to pixels

	// copy highest part of memory down and lowest part up until low and high are swapped
    for (; low < high; low += stride, high -= stride) 
	{
        std::memcpy(row, low, stride);
        std::memcpy(low, high, stride);
        std::memcpy(high, row, stride);
    }
	
    free(row);	// return malloced memory to system
}

unsigned char* PPM::readFile(std::string fileName, int &w, int &h)
{
	int x, y, pixel;
	unsigned char red, green, blue;
    std::ifstream image;
	image.open(fileName, std::ios::binary);

	if(image)
	{
		image >> magicNum;
		image >> width >> height >> maxval;
		image.get();
		size_t size = width * height * 3;
		pixmap = new char[size];
		image.read(pixmap, size);
		flipVertically(pixmap, width, height, sizeof(char));
		
		for(x = 0; x < width/2; ++x) 
		{
			for(y = 0; y < height/2; ++y) 
			{
				pixel = (y * width + x) * 3;
				red = pixmap[pixel+2];
				green = pixmap[pixel];
				blue = pixmap[pixel+1];
				
				pixmap[pixel] = red;
				pixmap[pixel++] = green;
				pixmap[pixel++] = blue;
			}
		}
		
		image.clear();
		image.close();
		w = width; h = height;
		
		return (unsigned char*)pixmap;
	}
	
	std::cout << "Invalid file name!\n";
	return nullptr;
}


void PPM::writeFile(char * pix, int w, int h, std::string fileName)
{
	width = w; height = h;
	flipVertically(pix, width, height, sizeof(char));
	std::ofstream ofs(fileName + ".ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	
	int pixel;
	for(unsigned i = 0; i < height; ++i)
	{
		for(unsigned j  = 0; j < width; ++j)
		{
			pixel = (i * width + j) * 3; 
			char r = (char)(pix[pixel]);
			char g = (char)(pix[pixel+1]);
			char b = (char)(pix[pixel+2]);
			ofs << r << g << b;
		}
	}
	
	ofs.close();
	std::cout << "Saved file to: ./" << fileName << ".ppm\n";	
	flipVertically(pix, width, height, sizeof(char));
}
