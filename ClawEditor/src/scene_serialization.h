#pragma once

#include "scene_manager.h"

void SceneSerialize(const scene_context_t* world, const std::string& file);
void SceneDeserialize(scene_context_t* world, const std::string& file);