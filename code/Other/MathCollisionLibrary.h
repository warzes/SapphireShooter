#pragma once

#include "SEMath.h"
#include "DimensionPolygon.h"

namespace scene
{
    class ViewFrustum;
}

namespace math
{


    //! Here you will find each function for collision detection, distance computation and intersection tests.
    namespace CollisionLibrary
    {

        /**
        Computes the closest point onto given triangle from an other point to this trianlge.
        \param Triangle: 3D triangle for the computation.
        \param Point: 3D point to which the closest point is to be computed.
        \return Closest 3D point from "Point" to "Triangle".
        */
        dim::vector3df getClosestPoint(const dim::triangle3df& Triangle, const dim::vector3df& Point);

        /**
        Computes the closest point from an other point to a triangle if this computed point lies onto the triangle.
        \param PointOnTriangle: Resulting 3D point.
        \return True if the point lies onto the triangle. In this case "Point" and "PointOnTrianlge" offer
        a normal vector to "Trianlge".
        */
        bool getClosestPointStraight(
            const dim::triangle3df& Triangle, const dim::vector3df& Point, dim::vector3df& PointOnTriangle
        );

        /**
        Computes the closest point from an other point to a box.
        \param Box: 3D AABB (Axis-Aliend-Bounding-Box) for the computation.
        \param Point: 3D point to which the closest point shall be computed.
        \return Closest 3D point from "Point" to "Box" (or rather to a Box's surface).
        */
        dim::vector3df getClosestPoint(const dim::aabbox3df& Box, const dim::vector3df& Point);

        /**
        Same like the function above but computes additional the normalized normal vector
        by "Point" and the resulting 3D point.
        */
        dim::vector3df getClosestPoint(
            const dim::aabbox3df& Box, const dim::vector3df& Point, dim::vector3df& Normal
        );

        /**
        Computes the closest point from an other point to a box.
        \param Box: 3D OBB (Oriented-Bounding-Box) for the computation.
        \param Point: 3D point to which the closest point shall be computed.
        \return Closest 3D point from "Point" to "Box" (or rather to a Box's surface).
        */
        dim::vector3df getClosestPoint(const dim::obbox3df& Box, const dim::vector3df& Point);

        /**
        Computes the closest line between the given triangle and line.
        \param Triangle: 3D triangle for the computation.
        \param Line: 3D line to which the closest point is to be computed.
        \return Closest 3D line from "Line" to "Triangle" where the start point is onto the triangle.
        */
        dim::line3df getClosestLine(const dim::triangle3df& Triangle, const dim::line3df& Line);

        /**
        Computes the closest line between the given triangle and line.
        \param Triangle: 3D triangle for the computation.
        \param Line: 3D line to which the closest point is to be computed.
        \param LineToTriangle: Resulting 3D line.
        \return True if the line's start or end point lies onto the triangle. Otherwise false and the resulting 3d line is empty.
        */
        bool getClosestLineStraight(const dim::triangle3df& Triangle, const dim::line3df& Line, dim::line3df& LineToTriangle);

        /**
        Computes the closest line from the given quadrangle and line.
        \param Quadrangle: 3D quadrangle for the computation.
        \param Line: 3D line to which the clsoest point is to be computed.
        \return Closest 3D line from "Line" to "Quadrangle" where the start point is onto the quadrangle.
        \todo This function has not been tested yet!
        */
        dim::line3df getClosestLine(const dim::quadrangle3df& Quadrangle, const dim::line3df& Line);

        /**
        Computes the closest line from the given axis-aligned bounding box and line.
        \param[in] Box Specifies the AABB.
        \param[in] Line Specifies the 3D line.
        \return Closest 3D line from "Line" to "Box" where the start point is onto the box.
        */
        dim::line3df getClosestLine(const dim::aabbox3df& Box, const dim::line3df& Line);

        //! Computes the squared distance between the given box and point.
        f32 getPointBoxDistanceSq(const dim::obbox3df& Box, const dim::vector3df& Point);

        //! Computes the distance between the given box and point.
        inline f32 getPointBoxDistance(const dim::obbox3df& Box, const dim::vector3df& Point)
        {
            return sqrt(getPointBoxDistanceSq(Box, Point));
        }

        /**
        Computes the squared distance between the given box and line.
        \todo This function has not been tested yet!
        */
        f32 getLineBoxDistanceSq(const dim::aabbox3df& Box, const dim::line3df& Line);

        //! Computes the distance between the given box and line.
        inline f32 getLineBoxDistance(const dim::aabbox3df& Box, const dim::line3df& Line)
        {
            return sqrt(getLineBoxDistanceSq(Box, Line));
        }

        /**
        Computes the squared distance between two lines. If you want to have the real distance
        take the square-root of the return value.
        \param LineA: First line.
        \param LineB: Second line.
        \param PointP: First point of the closest distance (or rather line).
        \param PointQ: Second point of the closest distance (or rather line).
        \return Closest distance of the two lines or rather the length of the closest line PQ.
        */
        f32 getLineLineDistanceSq(
            const dim::line3df& LineA, const dim::line3df& LineB, dim::vector3df& PointP, dim::vector3df& PointQ
        );

        //! Returns the square root of the result of the getLineLineDistanceSq function.
        inline f32 getLineLineDistance(
            const dim::line3df& LineA, const dim::line3df& LineB, dim::vector3df& PointP, dim::vector3df& PointQ)
        {
            return sqrt(getLineLineDistanceSq(LineA, LineB, PointP, PointQ));
        }

        /**
        Tests if the two lines are intersecting.
        \param LinesA: First line.
        \param LinesB: Second line.
        \param Intersection: Resulting intersection point of the two lines if an intersection has been deteced.
        \return True if an intersection has been detected.
        */
        bool getLineLineIntersection(
            const dim::line3df& LineA, const dim::line3df& LineB, dim::vector3df& Intersection
        );

        /**
        Computes the intersection of two lines (or rather rays because an intersection will always be detected).
        \param A: Start point of the first lines.
        \param B: End point of the first lines.
        \param C: Start point of the second lines.
        \param D: End point of the second lines.
        \return Intersection point of the two lines (or rather rays).
        */
        dim::point2df get2DLineLineIntersectionStraight(
            const dim::point2df& A, const dim::point2df& B, const dim::point2df& C, const dim::point2df& D
        );

        /* === Intersection tests === */

        /**
        Tests if a lines intersects with a triangle.
        \param Triangle: 3D triangle for the test.
        \param Line: 3D line for the test.
        \param Intersection: Resulting 3D intersection point.
        \return True if an intersection has been detected.
        */
        bool checkLineTriangleIntersection(
            const dim::triangle3df& Triangle, const dim::line3df& Line, dim::vector3df& Intersection
        );

        /**
        Tests if a line intersects with a sphere.
        \param[in] Line 3D line for the test.
        \param[in] SpherePosition Sphere's 3D position.
        \param[in] SphereRadius Sphere's radius.
        \param[out] Intersection Resulting 3D intersection point.
        \param[in] MakeRayTest Specifies whether the line is to be interpreted as a line or a ray. By default false.
        \return True if an intersection has been detected.
        */
        bool checkLineSphereIntersection(
            const dim::line3df& Line, const dim::vector3df& SpherePosition, const f32 SphereRadius, dim::vector3df& Intersection, bool MakeRayTest = false
        );

        /**
        Tests if a line intersects with an axis aligned bounding box.
        \param[in] Line 3D line for the test.
        \param[in] Box 3D AABB (Axis Aligned Bounding Box) for the test. In this case you can not
        transform the box (e.g. for rotations). Make an inverse transformation to the line.
        \param[out] Intersection Resulting 3D intersection point.
        \param[in] MakeRayTest Specifies whether the line is to be interpreted as a line or a ray. By default false.
        If the line is interpreted as a ray, the length of the line is ignored.
        \return True if an intersection has been detected.
        */
        bool checkLineBoxIntersection(
            const dim::line3df& Line, const dim::aabbox3df& Box, dim::vector3df& Intersection, bool MakeRayTest = false
        );

        /**
        Tests if a triangle intersects with an other triangle.
        \param TriangleA: First 3D triangle for the test.
        \param TriangleB: Second 3D triangle for the test.
        \param Intersection: Resulting 3D line (or rather "intersection cut").
        \return True if an intersection has been detected.
        */
        bool checkTriangleTriangleIntersection(
            const dim::triangle3df& TriangleA, const dim::triangle3df& TriangleB, dim::line3df& Intersection
        );

        /* === Overlap tests === */

        //! Returns true if an intersection between "Line" and "Box" has been detected.
        bool checkLineBoxOverlap(
            const dim::line3df& Line, const dim::aabbox3df& Box
        );

        //! Returns true if an intersection between "Plane" and "Box" has been detected.
        bool checkPlaneBoxOverlap(
            const dim::plane3df& Plane, const dim::aabbox3df& Box
        );

        //! Returns true if an intersection between "Triangle" and "Box" has been detected.
        bool checkTriangleBoxOverlap(
            const dim::triangle3df& Triangle, const dim::aabbox3df& Box
        );

        /**
        \return True if an intersection between "BoxA" and "BoxB" has been detected.
        OOBs (orientated-bounding-boxes) may be transformed for intersection tests
        (With AABBs (axis-aliend-bounding-boxes) you have to transform the opposit intersection
        object by the inverse transformation).
        */
        bool checkOBBoxOBBoxOverlap(
            const dim::obbox3df& BoxA, const dim::obbox3df& BoxB
        );

        /**
        Returns true if the two view frustums overlap each other.
        In this case the two view frustums are aproximated as pyramids for performance optimzation.
        */
        bool checkPyramidPyramidOverlap(
            const dim::vector3df& OriginA, const scene::ViewFrustum& FrustumA,
            const dim::vector3df& OriginB, const scene::ViewFrustum& FrustumB
        );

        /* === Polygon clippint === */

        template <typename T, typename C> bool getLinePlaneIntersection(
            const dim::plane3d<T>& Plane, const C& LineStart, const C& LineEnd, C& Intersection)
        {
            C Direction(LineEnd);
            Direction -= LineStart;

            const T t = (Plane.Distance - Plane.Normal.dot(LineStart.getCoord())) / Plane.Normal.dot(Direction.getCoord());

            if (t >= T(0) && t <= T(1))
            {
                // Intersection := LineStart + Direction * t
                Intersection = Direction;
                Intersection *= t;
                Intersection += LineStart;
                return true;
            }

            return false;
        }

        /**
        Clips the given polygon with the given plane.
        \tparam T Specifies the data type (float, double, int etc.).
        \tparam C Specifies the class type (point2d, vector3d etc.).
        Requires that the class implements a function called "vector3d<T> getCoord() const".
        \param[in] Poly Specifies the polygon which is to be clipped.
        \param[in] Plane Specifies the clipping plane.
        \param[out] FrontPoly Specifies the output polygon which is in front of the clipping plane.
        \param[out] BackPoly Specifies the output polygon which is behind the clipping plane.
        \return True on success. Otherwise the input is invalid.
        \see scene::ClipVertex
        */
        template <typename T, typename C> bool clipPolygon(
            const dim::polygon<C>& Poly, const dim::plane3d<T>& Plane, dim::polygon<C>& FrontPoly, dim::polygon<C>& BackPoly)
        {
            if (Poly.getCount() < 3)
            {
#ifdef SP_DEBUGMODE
                io::Log::debug("CollisionLibrary::clipPolygon", "Polygon has not enough points to be cliped");
#endif
                return false;
            }

            // Test all edges (a, b) starting with edge from last to first vertex
            const u32 PointCount = Poly.getCount();
            C a = Poly.Points.back();

            dim::EPlanePointRelations aSide = Plane.getPointRelation(a.getCoord());

            // Loop over all edges given by vertex pair (n-1, n)
            for (u32 i = 0; i < PointCount; ++i)
            {
                C b = Poly[i];

                dim::EPlanePointRelations bSide = Plane.getPointRelation(b.getCoord());

                if (bSide == dim::POINT_INFRONTOF_PLANE)
                {
                    if (aSide == dim::POINT_BEHIND_PLANE)
                    {
                        // Edge (a, b) straddles, output intersection point to both sides
                        C Intersection;
                        getLinePlaneIntersection(Plane, a, b, Intersection);

                        //if (Plane.getPointRelation(Intersection) != dim::POINT_ON_PLANE)
                        //    return false;

                        FrontPoly.push(Intersection);
                        BackPoly.push(Intersection);
                    }

                    // In all three cases, output b to the front side
                    FrontPoly.push(b);
                }
                else if (bSide == dim::POINT_BEHIND_PLANE)
                {
                    if (aSide == dim::POINT_INFRONTOF_PLANE)
                    {
                        // Edge (a, b) straddles plane, output intersection point
                        C Intersection;
                        getLinePlaneIntersection(Plane, a, b, Intersection);

                        //if (Plane.getPointRelation(Intersection) != dim::POINT_ON_PLANE)
                        //    return false;

                        FrontPoly.push(Intersection);
                        BackPoly.push(Intersection);
                    }
                    else if (aSide == dim::POINT_ON_PLANE)
                    {
                        // Output a when edge (a, b) goes from �on� to �behind� plane
                        BackPoly.push(a);
                    }

                    // In all three cases, output b to the back side
                    BackPoly.push(b);
                }
                else
                {
                    // b is on the plane. In all three cases output b to the front side
                    FrontPoly.push(b);

                    // In one case, also output b to back side
                    if (aSide == dim::POINT_BEHIND_PLANE)
                        BackPoly.push(b);
                }

                // Keep b as the starting point of the next edge
                a = b;
                aSide = bSide;
            }

            return true;
        }

    } // /namespace CollisionLibrary


} // /namespace math