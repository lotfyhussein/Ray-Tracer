#include <core/color.h>
#include <core/scalar.h>
#include <core/float4.h>

namespace rt {

RGBColor::RGBColor(const Float4& f4)
{
/* TODO */ NOT_IMPLEMENTED;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
    return RGBColor(this->r + c.r, this->g + c.g, this->b + c.b);
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
    return RGBColor(this->r - c.r, this->g - c.g, this->b - c.b);
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
    return RGBColor(this->r * c.r, this->g * c.g, this->b * c.b);
}

bool RGBColor::operator == (const RGBColor& c) const {
    return (this->r == c.r && this->g == c.g && this->b == c.b);
}

bool RGBColor::operator != (const RGBColor& b) const {
    return (this->r != b.r || this->g != b.g || this->b != b.b);
}

RGBColor RGBColor::clamp() const {
    // clamps values between 0 and 1
    float r_new, g_new, b_new;
    r_new = min(float(1), max(float(0), this->r));
    g_new = min(float(1), max(float(0), this->g));
    b_new = min(float(1), max(float(0), this->b));
    return RGBColor(r_new, g_new, b_new);
}

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
    return RGBColor(c.r * scalar, c.g * scalar, c.b * scalar);
}

RGBColor operator * (const RGBColor& c, float scalar) {
    return RGBColor(c.r * scalar, c.g * scalar, c.b * scalar);
}

RGBColor operator / (const RGBColor& c, float scalar) {
    return RGBColor(c.r / scalar, c.g / scalar, c.b / scalar);
}

}
