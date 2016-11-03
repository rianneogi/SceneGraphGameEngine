#include "Material.h"

Material::Material() : mTexture(NULL), mSpecularIntensity(0.f), mShineDamper(0.f)
{
}

Material::Material(Texture* tex, float intensity, float damper) : mTexture(tex), mSpecularIntensity(intensity), mShineDamper(damper)
{
}

Material::~Material()
{
}
