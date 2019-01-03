#pragma once

#include <d3dx9math.h>
#include <vector>

class NavigationNode
{
public:
	NavigationNode();

	void SetPosition( const D3DXVECTOR3& position ) { m_position = position; }
	const D3DXVECTOR3& GetPosition() const { return m_position; }

	void AddNeigbour( const NavigationNode& neightbour );
	const std::vector< const NavigationNode* > GetNeightbours() const { return m_neighbours; }

private:
	D3DXVECTOR3 m_position;
	std::vector< const NavigationNode* > m_neighbours;
};

float CalculateDistance( const NavigationNode& first, const NavigationNode& second );
