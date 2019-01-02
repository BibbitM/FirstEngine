#include "NavigationNode.h"
#include <cassert>

NavigationNode::NavigationNode()
	: m_position( 0.0f, 0.0f, 0.0f )
{
}

void NavigationNode::AddNeigbour( const NavigationNode* neightbour )
{
	assert( neightbour );
	m_neighbours.push_back( neightbour );
}

float CalculateDistance( const NavigationNode& first, const NavigationNode& second )
{
	D3DXVECTOR3 offset = second.GetPosition() - first.GetPosition();
	return D3DXVec3Length( &offset );
}
