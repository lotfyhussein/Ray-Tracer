#include <rt/textures/imagetex.h>
#include <core/interpolate.h>

namespace rt {


    ImageTexture::ImageTexture() { }

    ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i)
	{
		mBorderHandlingType = bh;
		mInterpolationType = i;
		mSourceImage = image;
	}

    ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i)
	{
		mBorderHandlingType = bh;
		mInterpolationType = i;
		mSourceImage.readPNG(filename);
	}

	RGBColor ImageTexture::getColorAtPixelCoordinates(float x, float y)
	{
		if (mInterpolationType == InterpolationType::NEAREST)
		{
			int ix = floor(x);
			float dx = x - ix;
			if (dx >=0.5) ix++;

			int iy = floor(y);
			float dy = y - iy;
			if (dy >=0.5) iy++;
			
			Point p = getTextureCoordinates(ix, iy);

			return mSourceImage(p.x, p.y);
		}

		if (mInterpolationType == InterpolationType::BILINEAR)
		{
			int x0 = floor(x);
			int x1 = ceil(x);

			int y0 = floor(y);
			int y1 = ceil(y);

			float dx = abs(x - x0);
			float dy = abs(y - y0);
			
			Point p00 = getTextureCoordinates(x0, y0);
			Point p01 = getTextureCoordinates(x0, y1);
			Point p10 = getTextureCoordinates(x1, y0);
			Point p11 = getTextureCoordinates(x1, y1);

			RGBColor col00 = mSourceImage(p00.x, p00.y);
			RGBColor col01 = mSourceImage(p01.x, p01.y);
			RGBColor col10 = mSourceImage(p10.x, p10.y);
			RGBColor col11 = mSourceImage(p11.x, p11.y);

			return lerp2d(col00, col10, col01, col11, dx, dy);
		}
	}

    RGBColor ImageTexture::getColor(const Point& coord)
	{
		float x = coord.x * mSourceImage.width();
		float y = coord.y * mSourceImage.height();

		return getColorAtPixelCoordinates(x, y);
	}

    RGBColor ImageTexture::getColorDX(const Point& coord)
	{
		float x = coord.x * mSourceImage.width();
		float y = coord.y * mSourceImage.height();

		RGBColor a = getColorAtPixelCoordinates(x, y);
		RGBColor b = getColorAtPixelCoordinates(x+1, y);
		float c = a.r - b.r;
		return RGBColor::rep(c);
	}

    RGBColor ImageTexture::getColorDY(const Point& coord)
	{
		float x = coord.x * mSourceImage.width();
		float y = coord.y * mSourceImage.height();

		RGBColor a = getColorAtPixelCoordinates(x, y);
		RGBColor b = getColorAtPixelCoordinates(x, y+1);
		float c = a.r - b.r;
		return RGBColor::rep(c);
	}

	Point ImageTexture::getTextureCoordinates(int x, int y)
	{
		if (mBorderHandlingType == BorderHandlingType::CLAMP)
		{
			if (x < 0) x = 0;
			if (x >= mSourceImage.width()) x = mSourceImage.width() - 1;

			if (y < 0) y = 0;
			if (y >= mSourceImage.height()) y = mSourceImage.height() - 1;

			return Point(x, y, 0);
		}

		if (mBorderHandlingType == BorderHandlingType::MIRROR)
		{
			int addX = 0;
			if (x < 0) addX = -1;

			int addY = 0;
			if (y < 0) addY = -1;


			x = abs(x) + addX;
			y = abs(y) + addY;

			int xParity = x / mSourceImage.width();
			int yParity = y / mSourceImage.height();
			
			x = x % mSourceImage.width();
			if (xParity % 2 != 0) x = mSourceImage.width() - x - 1;
			//x = x % mSourceImage.width();

			y = y % mSourceImage.height();
			if (yParity % 2 != 0) y = mSourceImage.height() - y - 1;
			// y = y % mSourceImage.height();

			return Point(x,y,0);
		}

		if (mBorderHandlingType == BorderHandlingType::REPEAT)
		{
			x = x % mSourceImage.width();
			y = y % mSourceImage.height();

			return Point(x,y,0);
			
		}
	}
}

