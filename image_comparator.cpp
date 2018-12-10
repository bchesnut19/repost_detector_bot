#include <iostream>
#include <png.h>
#include <jpeglib.h>
#include <dlib/image_io.h>
#include <dlib/array2d.h>
#include <dlib/pixel.h>

using namespace dlib;
using namespace std;

const int NUMSECTORS = 12;
const int TOLERANCE = 10;

class imageHash
{
	rgb_pixel sector[NUMSECTORS];
	string imagepath;
public:
	imageHash(string filepath)
	{
		imagepath = filepath;
		array2d<rgb_pixel> image;
		load_image(image,imagepath);


		int width = (int)image.nc();
		int height = (int)image.nr();
		int widthsecsize = (int)width/NUMSECTORS;
		int heightsecsize = (int)height/NUMSECTORS;
		
		
		int rowval = 0;
		int colval = 0;
		int upperbndcol = 0;
		int upperbndrow = 0;


		for (int i = 0; i< NUMSECTORS; i++)
		{
			if (i != (NUMSECTORS-1))
			{
				upperbndcol = colval + widthsecsize;
				upperbndrow = rowval + heightsecsize;	
			} 
			else
			{
				upperbndcol = width;
				upperbndrow = height;
			}
			sector[i] = average_pixel(image, rowval, upperbndrow, colval, upperbndcol);
			colval = upperbndcol;
			rowval = upperbndrow;
		}
		imagepath = filepath;
	}

	rgb_pixel * getsectors()
	{
		return sector;
	}

	bool compare(imageHash * imageIn)
	{
		rgb_pixel * inpixels = (*imageIn).getsectors();
		bool close = true;

		for (int i = 0; i < NUMSECTORS; i++)
		{
			if (!(comparesectors(sector[i],inpixels[i])))
			{
				close = false;
			}
		}
		return close;
	}
	void print()
	{
		cout << "Image: " << imagepath << "\n";
		for (int i = 0; i < NUMSECTORS; i++)
		{
			rgb_pixel temp = sector[i];
			cout << "Sector " << i << ":" << (int)temp.red << ", " << (int)temp.green << ", " << (int)temp.blue << ".\n";
		}
	}

private:
	rgb_pixel average_pixel(array2d<rgb_pixel>& imagein, int rowstart, int rowend, int collumnstart, int collumnend)
	{
		int rowdiff = rowend - rowstart;
		int collumndiff = collumnend - collumnstart;
		int redsum = 0;
		int greensum = 0;
		int bluesum = 0;
		int totalnum = rowdiff*collumndiff;
		for (int i = rowstart; i < rowend; i++)
		{
			for (int j = collumnstart; j < collumnend; j++)
			{
				rgb_pixel temp = imagein[i][j];
				redsum += (int)temp.red;
				greensum += (int)temp.green;
				bluesum += (int)temp.blue;
			}
		}
		int redavg = redsum/totalnum;
		int greenavg = greensum/totalnum;
		int blueavg = bluesum/totalnum;
		rgb_pixel avg;
		avg.red = (unsigned char)redavg;
		avg.green = (unsigned char)greenavg;
		avg.blue = (unsigned char)blueavg;
		return avg;
	}

	// ensures accuracy with tolerance for degradation of pictures
	bool comparesectors(rgb_pixel pix1,rgb_pixel pix2)
	{
		bool equalish;
		
		bool redcompare = comparedifference((int)pix1.red,(int)pix2.red);
		bool greencompare = comparedifference((int)pix1.green,(int)pix2.green);		
		bool bluecompare = comparedifference((int)pix1.blue,(int)pix2.blue);

		
		
		if ((redcompare == true) && (greencompare == true) && (bluecompare == true)) 
		{
			equalish = true;
		}
		else
		{
			equalish = false;
		}
		return equalish;
	}
	
	// ensures that difference between two values is less than 10
	bool comparedifference(int value1,int value2)
	{
		int temp = value1-value2;
		bool within;
		if((temp < (TOLERANCE/2)) && (temp > -(TOLERANCE/2)))
		{
			within = true;
		}
		else
		{
			within = false;
		}
		return within;
	}

};

int main(int argc, char* argv[])
{
	string name = argv[1];
	string name2  = argv[2];
	imageHash* imageone = new imageHash(name);
	(*imageone).print();

	imageHash* imagetwo = new imageHash(name2);
	(*imagetwo).print();
	
	bool result = (*imageone).compare(imagetwo);
	if (result == true)
	{
		cout << "True\n";
	}
	else
	{
		cout << "False\n";
	}
	delete(imageone);
	delete(imagetwo);
	return 0;
}
