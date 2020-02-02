#pragma once

namespace Zodiac {
	struct QueueFamilyIndices {
		uint32_t graphics_indices = UINT32_MAX;
		uint32_t present_indices = UINT32_MAX;
		uint32_t compute_indices = UINT32_MAX;
		uint32_t transfer_indices = UINT32_MAX;
	};
}