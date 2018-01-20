#ifndef FLASHLIGHTEFFECT_DEFERRED_H
#define FLASHLIGHTEFFECT_DEFERRED_H

#include "flashlighteffect.h"

struct def_light_t;

class CFlashlightEffectDeferred : public CFlashlightEffect
{
public:

	CFlashlightEffectDeferred(int nEntIndex = 0, const char *pszTextureName = NULL, float flFov = 0.0f, float flFarZ = 0.0f, float flLinearAtten = 0.0f );
	~CFlashlightEffectDeferred();

protected:

	void UpdateLightProjection( FlashlightState_t &state );
	void LightOff();

private:

	def_light_t *m_pDefLight;
};


#endif