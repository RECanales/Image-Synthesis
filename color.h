#include <math.h>
#include <vector>
#include <iostream>

class Color
{
	public:
		float r, g, b, w;
		Color() { r = g = b = 0; w = 1; }
		Color(float ri, float gi, float bi) { r = ri; g = gi; b = bi; w = 1; }
		Color(float ri, float gi, float bi, float wi) { r = ri; g = gi; b = bi; w = wi; }
		Color& operator=(const Color &a);
		bool operator==(const Color &a);
		const Color operator-(const Color& a) const;
		const Color operator+(const Color& a) const;
		const Color operator*(const Color& a) const;
		const Color operator/(const Color& a) const;
		friend Color operator/(const Color& a, float b);
		friend Color operator*(const Color& a, float b);
		friend Color operator*(float a, const Color&b);
		friend std::ostream& operator<<(std::ostream&, const Color&); 
};