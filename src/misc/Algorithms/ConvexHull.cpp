/////////////////////////////////////////////////////////////////////////////
// Name:            ConvexHull.cpp
//
// Description: This is the implementation file for the ConvexHull base class.
//
// Last modifications:
//      by : ----
/////////////////////////////////////////////////////////////////////////////

#include "ConvexHull.h"

//////////////////////////////////////////////////////////////////////////////////
// Constructor
//
// std::vector< Vector > & i_pointsVector : vector containing all points
//////////////////////////////////////////////////////////////////////////////////
ConvexHull::ConvexHull( std::vector< Vector > & i_pointsVector )
:   m_allPoints( i_pointsVector )

{
}


//////////////////////////////////////////////////////////////////////////////////
// Gives the convex hull points
//
// std::vector< Vector > o_points : Hull points
//
// Returns true if successful, false otherwise
//////////////////////////////////////////////////////////////////////////////////
bool ConvexHull::getHullPoints( std::vector< Vector > &o_points )
{
     // No hull
    if( m_hullPoints.size() == 0 )
        return false;

    o_points = m_hullPoints;
    return true;
}