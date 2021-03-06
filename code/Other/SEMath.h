#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "DimensionRect2.h"
#include "DimensionLine3.h"
#include "DimensionTriangle3.h"
#include "DimensionPlane3.h"
#include "MathRadianDegree.h"
#include "MathCore.h"
#include "MaterialColor.h"

namespace dim
{
	template <typename T> class matrix4;
	typedef matrix4<f32> matrix4f;
}

namespace math
{


    /**
    Returns the distance between the two vectors.
    \see dim::distance
    */
    template < template <typename> class V, typename T > inline T getDistance(const V<T>& A, const V<T>& B)
    {
        return dim::distance(A, B);
    }

    //! Returns the squared distance between two 2D vectors (used for fast distance comparision).
    template <typename T> inline T getDistanceSq(const dim::vector2d<T>& PosA, const dim::vector2d<T>& PosB)
    {
        return pow2(PosB.X - PosA.X) + pow2(PosB.Y - PosA.Y);
    }
    //! Returns the squared distance between two 3D vectors (used for fast distance comparision).
    template <typename T> inline T getDistanceSq(const dim::vector3d<T>& PosA, const dim::vector3d<T>& PosB)
    {
        return pow2(PosB.X - PosA.X) + pow2(PosB.Y - PosA.Y) + pow2(PosB.Z - PosA.Z);
    }

    /**
    Returns the angle between the two given 2D points.
    \param[in] A Specifies the first 2D point.
    \param[in] B Specifies the second 2D point.
    \return Angle between the two points. This is in the range [0.0 .. 360.0).
    If B.X >= A.X and B.Y >= A.Y then the return value is in the range [0.0 .. 90.0).
    If B.X >= A.X and B.Y < A.Y then the return value is in the range [90.0 .. 180.0).
    If B.X < A.X and B.Y < A.Y then the return value is in the range [180.0 .. 270.0).
    If B.X < A.X and B.Y >= A.Y then the return value is in the range [270.0 .. 360.0).
    */
    template <typename T> T getAngle(const dim::vector2d<T>& A, const dim::vector2d<T>& B)
    {
        const dim::vector2d<T> Dir(B - A);
        const T Dist = math::getDistance(A, B);

        if (Dir.X >= T(0))
        {
            if (Dir.Y >= T(0))
                return math::ASin((B.X - A.X) / Dist);
            return T(180) - math::ASin((B.X - A.X) / Dist);
        }

        if (Dir.Y >= T(0))
            return T(360) - math::ASin((A.X - B.X) / Dist);
        return T(180) + math::ASin((A.X - B.X) / Dist);
    }

    /**
    Returns the angle between the two given 2D points to the offset angle.
    This is in the range [-180.0 .. 180.0].
    */
    template <typename T> T getAngle(const dim::vector2d<T>& A, const dim::vector2d<T>& B, const T& OffsetAngle)
    {
        T Angle = getAngle<T>(A, B) - OffsetAngle;

        while (Angle > T(180))
            Angle -= T(360);
        while (Angle < T(-180))
            Angle += T(360);

        return Angle;
    }

    //! Returns normal vector computed by the three specified coordinates.
    template <typename T> inline dim::vector3d<T> getNormalVectorSq(
        const dim::vector3d<T>& PosA, const dim::vector3d<T>& PosB, const dim::vector3d<T>& PosC)
    {
        return (PosA - PosB).cross(PosB - PosC);
    }

    //! Returns normalized normal vector computed by the three specified coordinates.
    template <typename T> inline dim::vector3d<T> getNormalVector(
        const dim::vector3d<T>& PosA, const dim::vector3d<T>& PosB, const dim::vector3d<T>& PosC)
    {
        return getNormalVectorSq<T>(PosA, PosB, PosC).normalize();
    }

    /**
    Returns the barycentric coordinate given by the cartesian point respectively to the given triangle.
    \param[in] Triangle Specifies the 3D triangle.
    \param[in] Point Specifies the cartesian point. This point must lie onto the triangle.
    \return Barycentric coordinate of the given point respectively to the given triangle.
    */
    template <typename T> dim::vector3d<T> getBarycentricCoord(
        const dim::triangle3d<T>& Triangle, const dim::vector3d<T>& Point)
    {
        const dim::vector3d<T>& p = Point;
        const dim::vector3d<T>& a = Triangle.PointA;
        const dim::vector3d<T>& b = Triangle.PointB;
        const dim::vector3d<T>& c = Triangle.PointC;

        // Unnormalized triangle normal
        dim::vector3d<T> m = (b - a).cross(c - a);

        // Nominators and one-over-denominator for u and v ratios
        T nu, nv, ood;

        // Absolute components for determining projection plane
        const T x = math::Abs(m.X);
        const T y = math::Abs(m.Y);
        const T z = math::Abs(m.Z);

        // Compute areas in plane of largest projection
        if (x >= y && x >= z)
        {
            // x is largest, project to the yz plane
            nu = math::getTriangleArea2D<T>(p.Y, p.Z, b.Y, b.Z, c.Y, c.Z);  // Area of PBC in yz plane
            nv = math::getTriangleArea2D<T>(p.Y, p.Z, c.Y, c.Z, a.Y, a.Z);  // Area of PCA in yz plane
            ood = T(1) / m.X;                                               // 1/(2*area of ABC in yz plane)
        }
        else if (y >= x && y >= z)
        {
            // y is largest, project to the xz plane
            nu = math::getTriangleArea2D<T>(p.X, p.Z, b.X, b.Z, c.X, c.Z);
            nv = math::getTriangleArea2D<T>(p.X, p.Z, c.X, c.Z, a.X, a.Z);
            ood = T(1) / -m.Y;
        }
        else
        {
            // z is largest, project to the xy plane
            nu = math::getTriangleArea2D<T>(p.X, p.Y, b.X, b.Y, c.X, c.Y);
            nv = math::getTriangleArea2D<T>(p.X, p.Y, c.X, c.Y, a.X, a.Y);
            ood = T(1) / m.Z;
        }

        const T u = nu * ood;
        const T v = nv * ood;

        return dim::vector3d<T>(u, v, T(1.0) - u - v);
    }

    /**
    Converts the specified sphercial coordinates into cartesian coordinates.
    \param[out] Vec Specifies the output coordinate (in cartesian coordinates).
    \param[in] Theta Specifies the theta angle (in degrees) which is to be converted.
    \param[in] Phi Specifies the phi angle (in degrees) which is to be converted.
    \param[in] Radius Specifies the radius which is to be converted.
    \since Version 3.3
    */
    template <typename T> inline void convertToCartesianCoordiantes(dim::vector3d<T>& Vec, const T& Theta, const T& Phi, const T& Radius)
    {
        Vec.X = Radius * Sin(Theta) * Cos(Phi);
        Vec.Y = Radius * Cos(Theta);
        Vec.Z = Radius * Sin(Theta) * Sin(Phi);
    }

    /**
    Converts the specified cartesian coordinates into sphercial coordinates.
    \param[in] Vec Specifies the coordinate (in cartesian coordinates) which is to be converted.
    \param[out] Theta Specifies the output theta angle (in degrees).
    \param[out] Phi Specifies the output phi angle (in degrees).
    \param[out] Radius Specifies the output radius.
    \since Version 3.3
    */
    template <typename T> inline void convertToSphericalCoordiantes(const dim::vector3d<T>& Vec, T& Theta, T& Phi, T& Radius)
    {
        Radius = Vec.getLength();

        if (Radius != T(0))
        {
            Theta = atan2(Vec.Z, Vec.X) * T(RAD64);
            Phi = ACos(Vec.Y / Radius);
        }
        else
        {
            Theta = T(0);
            Phi = T(0);
        }
    }


    /* === Other math functions === */

    //! \deprecated This function should be redesigned.
    void getVertexInterpolation(
        const dim::triangle3df& Triangle, const dim::vector3df& Pos, f32& Vert, f32& Horz
    );

    //! \deprecated
    template <class T> T getVertexInterpolation(
        const T& VertexA, const T& VertexB, const T& VertexC, const f32 Vert, const f32 Horz)
    {
        return VertexA + (VertexB - VertexA) * Vert + (VertexC - VertexA) * Horz;
    }

    dim::matrix4f getTangentSpace(
        const dim::vector3df PosA, const dim::vector3df PosB, const dim::vector3df PosC,
        const dim::point2df MapA, const dim::point2df MapB, const dim::point2df MapC,
        dim::vector3df& Tangent, dim::vector3df& Binormal, dim::vector3df& Normal
    );

    /**
    Updates the two given 'Elo-Numbers'. Elo numbers are commonly used for rating systems
    in strategy games like chess or star-craft to classify the gamers skill.
    \param[in] Ra Specifies the 'Elo-Nnumber' R from player A.
    \param[in] Rb Specifies the 'Elo-number' R from player B.
    \param[in] Sa Specifies the winning factor from player A.
    This should be 1.0 if player A has won the last game, 0.5 if player A has
    given up the game or 0.0 if player A has lost the game.
    \param[in] k Specifies the modification constant. Use lower values if player A
    has made lots of games and higher values if player A has made less games.
    \return New updated 'Elo-Number' for player A.
    \todo Not tested yet!
    \since Version 3.3
    */
    f32 updateEloNumber(f32 Ra, f32 Rb, f32 Sa, const f32 k = 0.1f);


    /* === Other math functions === */

    //! Returns color in form of a 4D vector. The color components R, G, B, and A are transformed to the range [0.0, 1.0].
    inline dim::vector4df convert(const video::color& Color)
    {
        return dim::vector4df(
            static_cast<f32>(Color.Red) / 255,
            static_cast<f32>(Color.Green) / 255,
            static_cast<f32>(Color.Blue) / 255,
            static_cast<f32>(Color.Alpha) / 255
        );
    }

    //! Returns color in typical form. The color components R, G, B, and A are transformed to the range [0, 255].
    inline video::color convert(const dim::vector4df& Color)
    {
        return video::color(
            static_cast<u8>(Color.X * 255),
            static_cast<u8>(Color.Y * 255),
            static_cast<u8>(Color.Z * 255),
            static_cast<u8>(Color.W * 255)
        );
    }


} // /namespace math