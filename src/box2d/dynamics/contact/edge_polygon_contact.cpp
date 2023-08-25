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

#include "edge_polygon_contact.h"

#include "box2d/common/block_allocator.h"
#include "box2d/dynamics/fixture.h"

#include <new>

b2Contact* b2EdgeAndPolygonContact::Create(b2Fixture* fixtureA, int32, b2Fixture* fixtureB, int32, b2BlockAllocator* allocator)
{
	void* mem = allocator->Allocate(sizeof(b2EdgeAndPolygonContact));
	return new (mem) b2EdgeAndPolygonContact(fixtureA, fixtureB);
}

void b2EdgeAndPolygonContact::Destroy(b2Contact* contact, b2BlockAllocator* allocator)
{
	((b2EdgeAndPolygonContact*)contact)->~b2EdgeAndPolygonContact();
	allocator->Free(contact, sizeof(b2EdgeAndPolygonContact));
}

b2EdgeAndPolygonContact::b2EdgeAndPolygonContact(b2Fixture* fixtureA, b2Fixture* fixtureB)
: b2Contact(fixtureA, 0, fixtureB, 0)
{
	b2Assert(m_fixtureA->GetType() == b2Shape::e_edge);
	b2Assert(m_fixtureB->GetType() == b2Shape::e_polygon);
}

void b2EdgeAndPolygonContact::Evaluate(b2Manifold* manifold, const b2Transform& xfA, const b2Transform& xfB)
{
	b2CollideEdgeAndPolygon(	manifold,
								(b2EdgeShape*)m_fixtureA->GetShape(), xfA,
								(b2PolygonShape*)m_fixtureB->GetShape(), xfB);
}