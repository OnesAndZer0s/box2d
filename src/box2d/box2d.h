// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef BOX2D_H
#define BOX2D_H

// These include files constitute the main Box2D API

#include "common/settings.h"
#include "common/draw.h"
#include "common/timer.h"

#include "collision/shapes/chain_shape.h"
#include "collision/shapes/circle_shape.h"
#include "collision/shapes/edge_shape.h"
#include "collision/shapes/polygon_shape.h"

#include "collision/broad_phase.h"
#include "collision/dynamic_tree.h"

#include "dynamics/body.h"
#include "dynamics/contact/contact.h"
#include "dynamics/fixture.h"
#include "common/time_step.h"
#include "dynamics/world.h"
#include "dynamics/world_callbacks.h"

#include "dynamics/joint/distance_joint.h"
#include "dynamics/joint/friction_joint.h"
#include "dynamics/joint/gear_joint.h"
#include "dynamics/joint/motor_joint.h"
#include "dynamics/joint/mouse_joint.h"
#include "dynamics/joint/prismatic_joint.h"
#include "dynamics/joint/pulley_joint.h"
#include "dynamics/joint/revolute_joint.h"
#include "dynamics/joint/weld_joint.h"
#include "dynamics/joint/wheel_joint.h"

#endif
