#pragma once
@import MetalKit;

@interface CASView : MTKView<MTKViewDelegate>
@property BOOL prepared;
@end

using casein_native_handle = CAMetalLayer;
#include "common.hpp"
