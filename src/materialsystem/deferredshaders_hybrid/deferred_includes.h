#ifndef DEFERRED_INCLUDES_H
#define DEFERRED_INCLUDES_H

#include "deferred_global_common.h"

#include "shaderlib/commandbuilder.h"
#include "IDeferredExt.h"

#include "basevsshader.h"
#include "cpp_shader_constant_register_map.h"
#include "mathlib/VMatrix.h"
#include "ConVar.h"

#include "lighting_helper.h"

#include "renderparm.h"

#include "deferred_utility.h"
#include "deferred_context.h"

#include "defpass_gbuffer.h"
#include "defpass_shadow.h"
#include "defpass_composite.h"

#include "lighting_pass_basic.h"
#include "lighting_pass_volum.h"

#endif