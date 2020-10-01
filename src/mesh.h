#ifndef MESH_H
#define MESH_H

#include "vec.h"
#include <vector>
#include <deque>

class Mesh
{
public:
    Mat4x4 objectToWorld;

    int nTriagles;
    std::vector<Vec3> vertices;
    std::vector<int> triangles;
    // todo: texture

    Mesh()
    {
    }
};

namespace test
{
#define GENERATE_FACE_WRAP(posX, posY) generate_face_help(origin, x, y, posX, posY)
#define GENERATE_CUBE_WRAP(posA, posB, posC) generate_cube_help(origin, a, b, c, posA, posB, posC)

    // test mesh
    // outer + inner
    // outer
    /*
    generate(){
        Vector<Vec3*4> faces;
        Vector<Vec3*2> cubes;

        cubes.add((0,0,0),(1,1,1));
        faces.add();

        for i in range n:
            for each face in faces:
                use hole to build 8 new faces, add to faces

            for each cube in cubes:
                generate inner faces(24), add to faces
                generate next cubes, add to cubes

        for each face in faces:
            generate 2 triangles, add to mesh

    }
    */
    inline void push_cube(std::deque<Vec3> &v, const Vec3 &min, const Vec3 &max)
    {
        v.push_back(min);
        v.push_back(max);
    }

    inline void push_face(std::deque<Vec3> &v, const Vec3 &a, const Vec3 &b, const Vec3 &c, const Vec3 &d)
    {
        v.push_back(a);
        v.push_back(b);
        v.push_back(c);
        v.push_back(d);
    }

    inline void pop_cube(std::deque<Vec3> &v)
    {
        v.pop_front();
        v.pop_front();
    }

    inline void pop_face(std::deque<Vec3> &v)
    {
        v.pop_front();
        v.pop_front();
        v.pop_front();
        v.pop_front();
    }

    inline Vec3 generate_face_help(Vec3 origin, Vec3 x, Vec3 y, int posX, int posY)
    {
        return origin + x * posX + y * posY;
    }

    inline Vec3 generate_cube_help(Vec3 origin, Vec3 a, Vec3 b, Vec3 c, int posA, int posB, int posC)
    {
        return origin + a * posA + b * posB + c * posC;
    }

    void face_split(std::deque<Vec3> &target, const Vec3 &origin, const Vec3 &x, const Vec3 &y, const std::vector<int> &index)
    {
        int n = 3;
        for (int i : index)
        {
            int posX = i % n;
            int posY = i / n;
            push_face(target, GENERATE_FACE_WRAP(posX, posY), GENERATE_FACE_WRAP(posX + 1, posY), GENERATE_FACE_WRAP(posX + 1, posY + 1), GENERATE_FACE_WRAP(posX, posY + 1));
        }
    }

    void generate_triangles(Mesh &mesh)
    {
        int n = 3;
        std::deque<Vec3> faces;
        std::deque<Vec3> cubes;

        Vec3 vertices[8] = {Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 0), Vec3(0, 1, 1),
                            Vec3(1, 0, 0), Vec3(1, 0, 1), Vec3(1, 1, 0), Vec3(1, 1, 1)};

        push_cube(cubes, Vec3(0, 0, 0), Vec3(1, 1, 1));
        push_face(faces, vertices[0], vertices[2], vertices[6], vertices[4]);
        push_face(faces, vertices[2], vertices[3], vertices[7], vertices[6]);
        push_face(faces, vertices[1], vertices[3], vertices[7], vertices[5]);
        push_face(faces, vertices[0], vertices[2], vertices[3], vertices[1]);
        push_face(faces, vertices[4], vertices[6], vertices[7], vertices[5]);
        push_face(faces, vertices[0], vertices[1], vertices[5], vertices[4]);

        std::vector<int> faceSplitIndex; // split to index 0,1,2,3,5,6,7,8
        std::vector<int> cubeSplitIndex; // split to index 1,3,5,7
        for (int i = 0; i < 9; i++)
        {
            if (i != 4)
            {
                faceSplitIndex.push_back(i);
            }

            if (i % 2 == 1)
            {
                cubeSplitIndex.push_back(i);
            }
        }
        for (int i = 0; i < n; i++)
        {
            int faceNum = faces.size() / 4;
            for (int m = 0; m < faceNum; m++)
            {
                Vec3 a = faces[0], b = faces[1], c = faces[2], d = faces[3];
                Vec3 origin = a;
                Vec3 x = (b - a) / 3.0, y = (d - a) / 3.0;
                face_split(faces, origin, x, y, faceSplitIndex);

                pop_face(faces);
            }

            int cubeNum = cubes.size() / 2;
            for (int m = 0; m < cubeNum; m++)
            {
                Vec3 min = cubes[0], max = cubes[1];
                Vec3 a = Vec3(max.a - min.a, 0, 0), b = Vec3(0, max.b - min.b, 0), c = Vec3(0, 0, max.c - min.c);

                face_split(cubes, min, a, b, cubeSplitIndex);
                face_split(cubes, min + b, a, c, cubeSplitIndex);
                face_split(cubes, min + c, a, b, cubeSplitIndex);
                face_split(cubes, min, a, c, cubeSplitIndex);
                face_split(cubes, min, c, b, cubeSplitIndex);
                face_split(cubes, min + a, c, b, cubeSplitIndex);

                Vec3 origin = min;
                Vec3 newCubeDiagonal = (max - min) / 3.0;
                a = a / 3.0;
                b = b / 3.0;
                c = c / 3.0;

                for (int j = 0; j < 3; j++)
                    for (int k = 0; k < 3; k++)
                        for (int l = 0; l < 3; l++)
                        {
                            if ((j == 1 && k == 1) || (j == 1 && l == 1) || (k == 1 && l == 1))
                            {
                                continue;
                            }
                            Vec3 newOrigin = GENERATE_CUBE_WRAP(j, k, l);
                            push_cube(cubes, newOrigin, newOrigin + newCubeDiagonal);
                        }

                pop_cube(cubes);
            }
        }
    };
} // namespace test
#endif