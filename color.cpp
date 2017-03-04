#include "color.h"

using namespace std; 

// --- rgbTriplet class definitions --- //
bool Color::operator==(const Color& a)
{
	if(this->r == a.r && this->g == a.g && this->b == a.b)
		return true;
	else
		return false;
}

Color& Color::operator=(const Color& a)
{
	if (this != &a)
	{
		this->r = a.r;
		this->g = a.g;
		this->b = a.b;
	}

	return *this;
}

const Color Color::operator+(const Color& a) const
{
	Color sum;
	sum.r = this->r + a.r;
	sum.g = this->g + a.g;
	sum.b = this->b + a.b;
	return sum;
}

const Color Color::operator-(const Color& a) const
{
	Color diff;
	diff.r = this->r - a.r;
	diff.g = this->g - a.g;
	diff.b = this->b - a.b;
	return diff;
}

const Color Color::operator*(const Color& a) const
{
	Color prod;
	prod.r = this->r * a.r;
	prod.g = this->g * a.g;
	prod.b = this->b * a.b;
	return prod;
}

const Color Color::operator/(const Color& a) const
{
	Color div;
	div.r = this->r / a.r;
	div.g = this->g / a.g;
	div.b = this->b / a.b;
	return div;
}

Color operator/(const Color& a, float b)
{
	Color div;
	div.r = a.r / b;
	div.g = a.g / b;
	div.b = a.b / b;
	return div;
}

Color operator*(const Color& a, float b)
{
	Color prod;
	prod.r = a.r * b;
	prod.g = a.g * b;
	prod.b = a.b * b;
	return prod;
}

Color operator*(float a, const Color& b)
{
	Color prod;
	prod.r = a * b.r;
	prod.g = a * b.g;
	prod.b = a * b.b;
	return prod;
}

std::ostream& operator<<(std::ostream& os, const Color& c)
{
	os << "(R " << c.r << ", G " << c.g << ", B " << c.b << ")";
	return os;
}