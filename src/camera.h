#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"

Mat4x4 look_at(Vec3 &pos, Vec3 &look, Vec3 &up) {
  Mat4x4 cameraToWorld;
  cameraToWorld.value[0][3] = pos.a;
  cameraToWorld.value[1][3] = pos.b;
  cameraToWorld.value[2][3] = pos.c;
  cameraToWorld.value[3][3] = 1;

  Vec3 dir = (look - pos).normalize();
  Vec3 right = cross(up.normalize(), dir).normalize();
  Vec3 newUp = cross(dir, right);
  cameraToWorld.value[0][0] = right.a;
  cameraToWorld.value[1][0] = right.b;
  cameraToWorld.value[2][0] = right.c;
  cameraToWorld.value[3][0] = 0;

  cameraToWorld.value[0][1] = newUp.a;
  cameraToWorld.value[1][1] = newUp.b;
  cameraToWorld.value[2][1] = newUp.c;
  cameraToWorld.value[3][1] = 0;

  cameraToWorld.value[0][2] = dir.a;
  cameraToWorld.value[1][2] = dir.b;
  cameraToWorld.value[2][2] = dir.c;
  cameraToWorld.value[3][2] = 0;

  return inverse_mat(cameraToWorld);
}

#endif