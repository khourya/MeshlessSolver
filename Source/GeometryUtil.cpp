#include "Structures.h"
#include "Triangulate.h"
#include "WriteUtil.h"

// Point Functions


// Triangle Functions
Point* Triangle::GetPoint(int index)
{
    switch (index)
    {
    case 1:
        return m_point1;
        break;
    case 2:
        return m_point2;
        break;
    case 3:
        return m_point3;
        break;
    }

    return nullptr;
}

double Triangle::GetArea()
{
    double x1 = m_point1->GetX();
    double y1 = m_point1->GetY();

    double x2 = m_point2->GetX();
    double y2 = m_point2->GetY();

    double x3 = m_point3->GetX();
    double y3 = m_point3->GetY();

    double a = std::sqrt(std::pow(x2 - x1, 2.) + std::pow(y2 - y1, 2.));
    double b = std::sqrt(std::pow(x3 - x2, 2.) + std::pow(y3 - y2, 2.));
    double c = std::sqrt(std::pow(x1 - x3, 2.) + std::pow(y1 - y3, 2.));
    double s = (a + b + c) / 2;

    double area = std::sqrt(s * (s - a) * (s - b) * (s - c));

    return area;
}

double Triangle::GetVolume(std::string compartment)
{
    double c1, c2, c3 = 0.;

    if (compartment == "bound")
    {
        c1 = m_point1->GetBoundConcentration();
        c2 = m_point2->GetBoundConcentration();
        c3 = m_point3->GetBoundConcentration();
    }
    else if (compartment == "unbound")
    {
        c1 = m_point1->GetUnboundConcentration();
        c2 = m_point2->GetUnboundConcentration();
        c3 = m_point3->GetUnboundConcentration();
    }

    double c_avg = (c1 + c2 + c3) / 3.;

    return (c_avg * GetArea());
}

// Other Stuff
int Triangulate(bool* checker, GeometricData* geometricData)
{
    int errorFlag = 0;  // error reporting flag

    // Turn preprocessor data into data that's ingestible by Delaunator class
    std::vector<double> coords = geometricData->GetOrderedCoordinates();

    // Create Delaunator
    delaunator::Delaunator d(coords);

    // Update List of Triangles
    geometricData->GenerateTriangles(d);
    geometricData->WriteMeshData();

    return errorFlag;
}

std::vector<double> GeometricData::GetOrderedCoordinates()
{
    std::vector<double> coordinates;

    for (Point* point : m_points)
    {
        coordinates.push_back(point->GetX());
        coordinates.push_back(point->GetY());
    }

    return coordinates;
}

Point* GeometricData::GetPointByCoordinates(double x, double y)
{
    Point* pointOfInterest = nullptr;
    double index = -1;

    for (Point* point : m_points)
    {
        if (point->GetX() == x && point->GetY() == y)
        {
            pointOfInterest = point;
            break;
        }
    }

    return pointOfInterest;
}

int GeometricData::GeneratePointsList()
{
    int errorFlag = 0;
    
    for (int n = 0; n < m_nBoundaryPoints; n++)
    {
        double x = Xc[n];
        double y = Yc[n];

        Point* point = new Point(x, y);
        point->SetIndex(n);
        point->SetPointClassification(PointClassification::Boundary);

        m_points.push_back(point);
    }

    for (int n = m_nBoundaryPoints; n < m_nInternalPoints + m_nBoundaryPoints; n++)
    {
        double x = Xc[n];
        double y = Yc[n];

        Point* point = new Point(x, y);
        point->SetIndex(n);
        point->SetPointClassification(PointClassification::Interior);

        m_points.push_back(point);
    }

    return errorFlag;
}

int GeometricData::GenerateTriangles(delaunator::Delaunator d)
{
    int errorFlag = 0;  // error reporting flag

    int nTriangles = static_cast<int>(d.triangles.size()) / 3;
    int index = 0;

    for (std::size_t i = 0; i < d.triangles.size(); i += 3)
    {
        double x1 = d.coords[2 * d.triangles[i]];         //tx0
        double y1 = d.coords[2 * d.triangles[i] + 1];     //ty0
        double x2 = d.coords[2 * d.triangles[i + 1]];     //tx1
        double y2 = d.coords[2 * d.triangles[i + 1] + 1]; //ty1
        double x3 = d.coords[2 * d.triangles[i + 2]];     //tx2
        double y3 = d.coords[2 * d.triangles[i + 2] + 1]; //ty2

        Point* point1 = GetPointByCoordinates(x1, y1);
        Point* point2 = GetPointByCoordinates(x2, y2);
        Point* point3 = GetPointByCoordinates(x3, y3);

        Triangle* triangle = new Triangle(point1, point2, point3);
        triangle->SetIndex(index);
        m_triangles.push_back(triangle);
        index++;
    }

    return errorFlag;
}
