// Author: Jai Vaidya
/* POLYGON TRIANGULATION ALGORITHM: O(n^2)
First, we generate all possible ears of the polygon. An ear of a polygon is a convex vertex such that the line segment joining the endpoints of the ear is an internal diagonal (i.e., it lies entirely inside the polygon, and no edge of the polygon intersects it). So, we need to iterate over all the vertices of the polygon, which takes O(n) time, and check if each vertex is an ear. This checking takes O(n) time for each vertex, as we need to check for intersecting edges. So, we get all ears in O(n^2) time, and there are O(n) ears. Now, we choose any ear, and join its endpoints by a diagonal, and remove that ear vertex from the polygon, and recursively do this triangulation process for the modified polygon. But, as soon as we process an ear, the ear-configuration of its endpoints may change, and to check that, we need to proceed similarly as we did before to check if a vertex of the polygon is an ear. So, for 2 endpoints, it takes O(2*n) = O(n) time, and if there is any change, our list of existing ears gets updated. We carry on the recursion with the modified set of ears, and the modified polygon, and hence, the whole triangulation completes in O(n^2) time. */
#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;
int nn; // keeps track of number of unvisited vertices
// structure of a polygon point
struct Point
{
    float x, y;
};
// structure of an ear
struct Ear
{
    int idx;
    Ear *next = NULL;
};
// structure of a diagonal which contributes to the triangulation
struct Diagonal
{
    int idx1, idx2;
    Diagonal *next = NULL;
};
// checks if index i is an ear
Ear *find_ear(Ear *ear, int i)
{
    Ear *t_ear = ear;
    while (t_ear != NULL)
    {
        if (t_ear->idx == i)
            break;
        t_ear = t_ear->next;
    }
    return t_ear;
}
// inserts index i as ear
Ear *insert_ear(Ear *ear, int i)
{
    Ear *t_ear = new Ear();
    t_ear->idx = i;
    t_ear->next = ear;
    return t_ear;
}
// deletes ear of index i
Ear *delete_ear(Ear *ear, int i)
{
    if (ear->idx == i)
        return ear->next;
    Ear *c_ear = ear->next;
    Ear *p_ear = ear;
    while (c_ear->idx != i)
    {
        p_ear = c_ear;
        c_ear = c_ear->next;
    }
    p_ear->next = c_ear->next;
    return ear;
}
// inserts a new diagonal contributing to the triangulation
Diagonal *insert_diag(Diagonal *diag, int i1, int i2)
{
    Diagonal *t_diag = new Diagonal();
    t_diag->idx1 = i1;
    t_diag->idx2 = i2;
    t_diag->next = diag;
    return t_diag;
}
// checks ordering of points p1, p2, p3 in space (cw / ccw)
float signed_area_triangle(Point p1, Point p2, Point p3)
{
    return (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2;
}
// computes area of triangle with vertices p1, p2, p3
float area_triangle(Point p1, Point p2, Point p3)
{
    return abs(signed_area_triangle(p1, p2, p3));
}
// checks if point p is inside triangle with vertices p1, p2, p3
bool inside_triangle(Point p1, Point p2, Point p3, Point p)
{
    return area_triangle(p1, p2, p) + area_triangle(p2, p3, p) + area_triangle(p3, p1, p) == area_triangle(p1, p2, p3);
}
// returns the point to the left of i in clockwise-ordering in the modified polygon
int left(Point *polygon, int n, bool *present, int i)
{
    int i1 = (i - 1 + n) % n;
    while (!present[i1])
    {
        i1 = (i1 - 1 + n) % n;
    }
    return i1;
}
// returns the point to the right of i in clockwise-ordering in the modified polygon
int right(Point *polygon, int n, bool *present, int i)
{
    int i2 = (i + 1) % n;
    while (!present[i2])
    {
        i2 = (i2 + 1) % n;
    }
    return i2;
}
// checks if there is ear at index i of the modified polygon
bool check_ear(Point *polygon, int n, int i, int i1, int i2)
{
    if (signed_area_triangle(polygon[i1], polygon[i], polygon[i2]) < 0)
    {
        for (int j = 0; j < n; j++)
        {
            if (j != i1 && j != i && j != i2)
            {
                if (inside_triangle(polygon[i1], polygon[i], polygon[i2], polygon[j]))
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}
// generates all ears for the polygon
Ear *generate_ears(Point *polygon, int n)
{
    Ear *ear = NULL;
    for (int i = 0; i < n; i++)
    {
        int i1 = (i - 1 + n) % n, i2 = (i + 1) % n;
        if (check_ear(polygon, n, i, i1, i2))
        {
            ear = insert_ear(ear, i);
        }
    }
    return ear;
}
// triangulates the polygon by adding certain diagonals
Diagonal *triangulate(Diagonal *diag, Point *polygon, int n, Ear *ear, bool *present)
{
    if (nn == 3)
        return diag; // if number of unvisited vertices is 3, triangulation is done, because polygon has reduced to a triangle
    int i = ear->idx;
    present[i] = false;
    nn--;
    ear = ear->next;
    int i1 = left(polygon, n, present, i), i2 = right(polygon, n, present, i);
    diag = insert_diag(diag, i1, i2); // inserts a diagonal connecting endpoints of an ear (polygon gets modified, ear vertex is visited)
    int i11 = left(polygon, n, present, i1), i22 = right(polygon, n, present, i2);
    // checks the configuration of endpoints of ear for the creation of new ear or deletion of existing ear
    Ear *t_ear = find_ear(ear, i1);
    if (check_ear(polygon, n, i1, i11, i2))
    {
        if (t_ear == NULL)
        {
            ear = insert_ear(ear, i1);
        }
    }
    else
    {
        if (t_ear != NULL)
        {
            ear = delete_ear(ear, i1);
        }
    }
    Ear *tt_ear = find_ear(ear, i2);
    if (check_ear(polygon, n, i2, i1, i22))
    {
        if (tt_ear == NULL)
        {
            ear = insert_ear(ear, i2);
        }
    }
    else
    {
        if (tt_ear != NULL)
        {
            ear = delete_ear(ear, i2);
        }
    }
    diag = triangulate(diag, polygon, n, ear, present); // recursively triangulates the modified polygon
    return diag;
}
void draw(float polygon[][2], int triangulation[][2], int n)
{ // graphical visualization of the triangulation of the polygon using graphics.h library
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    for (int i = 0; i < n - 1; i++)
    { // here, the axes are mirrored over the X axis. So, the polygon visualized will also be a mirrored version
        setcolor(1);
        line(50 * polygon[i][0] + 10, 50 * polygon[i][1] + 10, 50 * polygon[i + 1][0] + 10, 50 * polygon[i + 1][1] + 10);
    }
    line(50 * polygon[n - 1][0] + 10, 50 * polygon[n - 1][1] + 10, 50 * polygon[0][0] + 10, 50 * polygon[0][1] + 10);
    setcolor(2);
    for (int i = 0; i < n - 3; i++)
    {
        line(50 * polygon[triangulation[i][0]][0] + 10, 50 * polygon[triangulation[i][0]][1] + 10, 50 * polygon[triangulation[i][1]][0] + 10, 50 * polygon[triangulation[i][1]][1] + 10);
    }
    getch(); // also, if the points get out of the boundary, for eg if it is negative or some very large value, then it cannot visualise it properly
    closegraph();
}
int main()
{
    int n = 4;
    nn = n;
    float t_polygon[n][2] = {{0, 0}, {1, 7}, {2, 0}, {1, 1}};
    Point polygon[n];
    for (int i = 0; i < n; i++)
    {
        polygon[i].x = t_polygon[i][0];
        polygon[i].y = t_polygon[i][1];
    }
    bool present[n];
    for (int i = 0; i < n; i++)
        present[i] = true;
    Ear *ear = generate_ears(polygon, n);
    Diagonal *diag = NULL;
    diag = triangulate(diag, polygon, n, ear, present);
    Diagonal *t_diag = diag;
    int triangulation[n - 3][2];
    int i = 0;
    while (t_diag != NULL)
    {
        triangulation[i][0] = t_diag->idx1;
        triangulation[i][1] = t_diag->idx2;
        t_diag = t_diag->next;
        i++;
    }
    draw(t_polygon, triangulation, n);
}