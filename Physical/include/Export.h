#pragma once

#ifdef IS_EXPORT
	#define YUH_API_PHYSICAL __declspec(dllexport)
#else
	#define YUH_API_PHYSICAL __declspec(dllimport)
#endif

#define S_NS_PHYSICAL namespace yuh { \
						namespace physical {
#define E_NS } }

#define PTM_RATIO 32


/* World -< Body -< List Collider -< Shape
 *
 */