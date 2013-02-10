#pragma once

/** How many EditorCommandModifyElevation instances to keep in the history */
static const int kMaxElevationUndo		= 200;

/** How many EditorCommandPaintTexture instances to keep in history
 This also indicates how many FBO paint layers can be in memory, so it should be as low as possible */
static const int kMaxTexturePaintUndo	= 15;

static const int kParamsWidth			= 300;
static const int kParamsHeight			= 1000;