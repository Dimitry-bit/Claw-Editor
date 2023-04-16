#pragma once

#include <vector>
#include <string>

#include "resource_manager.h"

using std::string;

bool ImGuiTextureGrid(const std::vector<asset_slot_t*>& assets, string& selectedTex, int rowSize = 10);
void DrawAssetTooltip(const asset_header_t& header, const frameMetadata_t* frameMetadata = nullptr);