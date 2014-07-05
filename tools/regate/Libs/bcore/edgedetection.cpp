#include "Image.h"
#include "misc.h"

void EdgeDetector::sobel(const Image &image, std::vector< Matrix<double> > &gradient, 
						std::vector< Matrix<double> > &angle)
{
	if (image.isEmpty())
		return;

	// sobel filter
	const double sobX[3][3] = {{1, 2, 1},
							 {0, 0, 0},
							 {-1, -2, -1}};
	const double sobY[3][3] = {{1, 0, -1},
							 {2, 0, -2},
							 {1, 0, -1}};
	const int size = 3;
	int border = (size/2.0);	// size of the border is half the filter size

	size_t w = image.getWidth();
	size_t h = image.getHeight();
	size_t nChannels = image.getChannelsNum();
	
	// allocate space for results
	gradient.resize(nChannels);
	angle.resize(nChannels);
	for (size_t c=0; c<nChannels; ++c) {
		gradient[c] = Matrix<double>(w,h,0);
		angle[c] = Matrix<double>(w,h,0);
	}

	// run the sobel filter - x and y axis
	for (int x=border+1; x<(int)w-border-1; ++x)
		for (int y=border+1; y<(int)h-border-1; ++y)
		{
			if (x<0 || x>=w || y<0 || y>=h)
				continue;

			for (size_t k=0; k<nChannels; ++k)
			{
				double respX = 0, respY = 0;

				// apply the sobel operator
				for (int i=0; i<size; ++i)
					for (int j=0; j<size; ++j)
					{
						if (x+i-border < 0 || x+i-border >= w ||
							y+j-border < 0 || y+j-border >= h)
							continue;
						
						double img_sample=0;
						switch (image.getFormat())
						{
						case Image::I8BITS : img_sample = image.at<uint8_t >(x+i-border, y+j-border, k) / 255.0; break;
						case Image::I16BITS: img_sample = image.at<uint16_t>(x+i-border, y+j-border, k) / 255.0; break;
						case Image::I32BITS: img_sample = image.at<uint32_t>(x+i-border, y+j-border, k) / 255.0; break;
						case Image::F32BITS: img_sample = image.at<float32_t>(x+i-border, y+j-border, k); break;
						case Image::F64BITS: img_sample = image.at<float64_t>(x+i-border, y+j-border, k); break;
						default: throw std::exception("image format not supported");
						}

						respX += img_sample * sobX[i][j];
						respY += img_sample * sobY[i][j];
					}
				
				// get the gradient magnitude and orientation
				double G = sqrt(respX*respX + respY*respY);
				if (respX == 0)
					respX = 0.0001;
				double theta = atan(respY/respX);

				gradient[k](x,y) = G;
				angle[k](x,y) = theta;
			}
		}
}

void EdgeDetector::canny(const Image &image, std::vector< Matrix<double> > &gradient,
						std::vector< Matrix<double> > &angle)
{
	if (image.isEmpty())
		return;

	/*// convolve the image with a gaussian
	Matrix<double> filter;
	filter.createGaussian(gaussian_size);
	convolution(&filter(0,0), gaussian_size);*/

	// image info
	size_t w = image.getWidth();
	size_t h = image.getHeight();
	size_t nChannels = image.getChannelsNum();

	// run the sobel filter
	sobel(image, gradient, angle);

	for (size_t c=0; c<nChannels; ++c)
	{
		Matrix<int> angle_approx(w, h, 0);

		for (size_t x=0; x<w; ++x)
			for (size_t y=0; y<h; ++y)
			{
				double theta = angle[c](x,y) * 180.0 / PI;

				// compute the approximate gradient theta
				if (theta < 22.5  || theta > -22.5)
					angle_approx(x,y) = 0;	// 0 degrees
				else if (theta >= 22.5 && theta < 67.5)
					angle_approx(x,y) = 1;	// 45 degrees
				else if (theta >= 67.5 && theta < -67.5)
					angle_approx(x,y) = 2;	// 90 degrees
				else if (theta <= -22.5 && theta >= -67.5)
					angle_approx(x,y) = 3;	// 135 degrees
			}

		// perform non-maximal suppression
		Matrix<double> grad_supr(w, h, 0);
		for (size_t x=2; x<w-2; ++x)
			for (size_t y=2; y<h-2; ++y)
			{
				// perpendicular direction for each of the quantized angles
				const int pxoffs[] = {0, 1, 1, 1};
				const int pyoffs[] = {1, -1, 0, 1};

				// get the direction perpendicular to this edge
				int xoffs = pxoffs[angle_approx(x,y)];
				int yoffs = pyoffs[angle_approx(x,y)];

				// suppress non-maximal values along this direction
				// if the pixel belongs on a ridge, the gradient magnitude will
				// be higher than either of the gradient magnitudes of pixels on
				// the two sides of the edge (= along the direction perpendicular
				// to the edge)
				double g1 = gradient[c](x+xoffs, y+yoffs);
				double g2 = gradient[c](x-xoffs, y-yoffs);
				double g = gradient[c](x,y);
				if (g > g1 && g > g2)
					grad_supr(x,y) = g;
			}

		// store the gradients after non-max suppression
		gradient[c] = grad_supr;

		// perform hysterisis thresholding
	}
}

void EdgeDetector::combineResponses(const std::vector<Matrix<double> > &resp, Matrix<double> &out)
{
	size_t w = resp[0].numCols();
	size_t h = resp[0].numRows();

	out.create(w,h);

	for (size_t x=0; x<w; ++x)
		for (size_t y=0; y<h; ++y)
		{
			double max_resp = 0;
			for (size_t c=0; c<resp.size(); ++c)
			{
				if (resp[c](x,y) > max_resp)
					max_resp = resp[c](x,y);
			}

			out(x,y) = clamp(max_resp, 0.0, 1.0);
		}
}