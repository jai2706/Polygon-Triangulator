// Author: Jai Vaidya

/* POLYGON TRIANGULATION ALGORITHM: O(nlogn)
If the polygon is monotone, it can be triangulated in the following way:
A polygon is L-monotone if all lines perpendicular to L intersect the polygon at atmost 2 points. Here we describe the triangulation of a y-monotone polygon (here L is the y-axis):
We sort all vertices of the polygon in decreasing y, or if equal y, then increasing x, using mergesort in O(nlogn) time. Let the vertices be {v1, v2, .. , vn} in sorted order. Now, we split the monotone polygon into 2 y-monotone chains. We maintain a stack of vertices for triangulation.
We push vertices v1, v2 onto stack and iterate over other vertices in sorted order except the last vertex, in O(n) time. Now, for each vertex we check if that vertex and top vertex of stack are on same or opposite chains. If opposite chain, we keep on adding diagonals from current vertex to members of stack except the last one, and then push previous vertex and current vertex onto stack. If same chain, we keep on adding diagonals from current vertex to members of stack until one member denies diagonal formation, then we break, and push the previous 2 vertices and current vertex onto stack. When all vertices are processed except the last one, we keeps on adding diagonals from the last vertex to the members of the stack except top and bottom ones. Our triangulation is finished and it takes O(n) time to do so.
If the polygon is non-monotone, then we split the polygon into y-monotone subpolygons and carry out the above procedure for each monotone subpolygon. Here, we describe the procedure of y-monotone subdivision of a non-monotone polygon:
First, we find y-cusps of the polygon. A y-cusp is a reflex vertex such that its endpoints have y-coordinates either both less (down-y-cusp) or both more than its y-coordinate (up-y-cusp). Then, if it is a down-y-cusp, then we join it with its previous vertex in the sorted order, and if up-y-cusp, then we join it with its next vertex in the sorted order. There are O(n) cusps, and processing each cusp takes O(1) time. In this way, we divide the polygon into monotone subpolygons in O(n) time.
So the algorithm takes O(nlogn) time for sorting vertices, O(n) time for monotone subdivision, O(n) time for monotone triangulation, resulting in worst case O(nlogn) overall time. (The monotone subdivision routine has not been done in the code, hence the following code can only triangulate a y-monotone polygon) */
#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;
// structure of a polygon point
struct Point
{
    int idx;
    float x, y;
};
// struture of stack of polygon vertices
struct Stack
{
    int val;
    Stack *next = NULL;
};
// structure of a diagonal which contributes to the triangulation
struct Diagonal
{
    int idx1, idx2;
    Diagonal *next = NULL;
};
// pushes a vertex on top of stack
Stack *push(Stack *s, int v)
{
    Stack *s0 = new Stack();
    s0->val = v;
    s0->next = s;
    return s0;
}
// pops a vertex from top of stack
Stack *pop(Stack *s)
{
    return s->next;
}
// returns the element at top of stack
int top(Stack *s)
{
    return s->val;
}
// inserts a new diagonal contributing to the triangulation
Diagonal *insert(Diagonal *diag, int i1, int i2)
{
    Diagonal *t_diag = new Diagonal();
    t_diag->idx1 = i1;
    t_diag->idx2 = i2;
    t_diag->next = diag;
    return t_diag;
}
// returns the point to the left of i in clockwise-ordering in polygon
int left(int n, int i)
{
    return (i - 1 + n) % n;
}
// returns the point to the right of i in clockwise-ordering in polygon
int right(int n, int i)
{
    return (i + 1) % n;
}
// checks ordering of points p1, p2, p3 in space (cw / ccw)
float signed_area_triangle(Point p1, Point p2, Point p3)
{
    return (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2;
}
// merge routine of mergesort
void merge(Point *polygon, int l, int m, int h)
{
    int n1 = m - l + 1, n2 = h - m;
    Point l_polygon[n1], r_polygon[n2];
    for (int i = 0; i < n1; i++)
        l_polygon[i] = polygon[l + i];
    for (int i = 0; i < n2; i++)
        r_polygon[i] = polygon[m + 1 + i];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if ((l_polygon[i].y > r_polygon[j].y) || (l_polygon[i].y == r_polygon[j].y && l_polygon[i].x < r_polygon[j].x))
        {
            polygon[k] = l_polygon[i];
            i++;
        }
        else
        {
            polygon[k] = r_polygon[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        polygon[k] = l_polygon[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        polygon[k] = r_polygon[j];
        j++;
        k++;
    }
}
// mergesort routine
void mergesort(Point *polygon, int low, int high)
{
    if (high <= low)
        return;
    int mid = (low + high) / 2;
    mergesort(polygon, low, mid);
    mergesort(polygon, mid + 1, high);
    merge(polygon, low, mid, high);
}
// checks if line segment joining i1 and i2 is a diagonal of the polygon
bool check_diag(Point *polygon, Point *t_polygon, int n, int i1, int i2)
{
    i1 = polygon[i1].idx, i2 = polygon[i2].idx;
    int i1_left = left(n, i1), i1_right = right(n, i1);
    if (signed_area_triangle(t_polygon[i1_left], t_polygon[i1], t_polygon[i1_right]) < 0)
    {
        if (signed_area_triangle(t_polygon[i1_left], t_polygon[i1], t_polygon[i2]) <= 0 && signed_area_triangle(t_polygon[i1_right], t_polygon[i1], t_polygon[i2]) >= 0)
        {
            return true;
        }
        return false;
    }
    else
    {
        if (signed_area_triangle(t_polygon[i1_left], t_polygon[i1], t_polygon[i2]) >= 0 && signed_area_triangle(t_polygon[i1_right], t_polygon[i1], t_polygon[i2]) <= 0)
        {
            return false;
        }
        return true;
    }
}
// assigns monotone chain numbers 1 and 2 to all polygon vertices
int *form_chain(Point *polygon, int n)
{
    int *chain = new int[n];
    for (int i = 1; i <= n - 2; i++)
    {
        if (i == 1)
        {
            chain[i] = 0;
            continue;
        }
        int j = i - 1;
        int i0 = polygon[i].idx, j0 = polygon[j].idx;
        int j0_left = left(n, j0), j0_right = right(n, j0);
        if (j0_left == i0 || j0_right == i0)
            chain[i] = chain[j];
        else
            chain[i] = (chain[j] + 1) % 2;
    }
    return chain;
}
// triangulates the monotone polygon by adding certain diagonals
Diagonal *triangulate_monotone(Point *polygon, Point *t_polygon, int n)
{
    mergesort(polygon, 0, n - 1); // sorts the polygon vertices in decreasing y, or if equal y then increasing x
    for (int i = 0; i < n; i++)
    {
        int j = polygon[i].idx;
        t_polygon[j].idx = i;
    }
    int *chain = form_chain(polygon, n); // divides the monotone polygon into two monotone chains
    Diagonal *diag = NULL;
    Stack *s = NULL;
    // pushes 1st two vertices into stack
    s = push(s, 0);
    s = push(s, 1);
    // processes remaining vertices except the last one
    for (int i = 2; i <= n - 2; i++)
    {
        // if current vertex and vertex on top of stack are on opposite chains
        if (chain[i] != chain[top(s)])
        {
            // keep on adding diagonals from current vertex to members of stack except the last one
            while (s->next != NULL)
            {
                int j = top(s);
                s = pop(s);
                diag = insert(diag, polygon[i].idx, polygon[j].idx);
            }
            s = pop(s);
            // inserts previous vertex and current vertex into stack for further processing
            s = push(s, i - 1);
            s = push(s, i);
        }
        // if current vertex and vertex on top of stack are on same chain
        else
        {
            int l = top(s);
            s = pop(s);
            int j = top(s);
            s = pop(s);
            // keep on adding diagonals from current vertex to members of stack until one member denies diagonal formation, then we break
            while (check_diag(polygon, t_polygon, n, i, j))
            {
                diag = insert(diag, polygon[i].idx, polygon[j].idx);
                l = j;
                if (s == NULL)
                    break;
                j = top(s);
                s = pop(s);
            }
            // inserts previous 2 vertices and current vertex into stack for further processing
            if (j != l)
                s = push(s, j);
            s = push(s, l);
            s = push(s, i);
        }
    }
    s = pop(s);
    int i = n - 1;
    // keeps on adding diagonals from the last vertex to the members of the stack except top and bottom ones
    while (s->next != NULL)
    {
        int j = top(s);
        s = pop(s);
        diag = insert(diag, polygon[i].idx, polygon[j].idx);
    }
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
    int n = 5;
    float tt_polygon[n][2] = {{0, 0}, {2, 1}, {1, 1}, {2, 2}, {10, 0}};
    Point polygon[n], t_polygon[n];
    for (int i = 0; i < n; i++)
    {
        polygon[i].idx = i;
        polygon[i].x = tt_polygon[i][0];
        polygon[i].y = tt_polygon[i][1];
        t_polygon[i] = polygon[i];
    }
    Diagonal *diag = triangulate_monotone(polygon, t_polygon, n);
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
    draw(tt_polygon, triangulation, n);
}