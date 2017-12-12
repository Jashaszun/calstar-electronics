#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <math.h>
#include <utility>
#include <unistd.h>
#define PI 3.14159265f

#define debug(X) cout << #X << ": " << X << endl;

#define PROJECTION_SIZE 4 // 2x2 meters
#define PROJECTION_RES 150 // points per meter

using namespace std;

class Three {
public:
  float x;
  float y;
  float z;
  Three(float x, float y, float z) : x(x), y(y), z(z) { }

private:
  Three();
};

class Two {
public:
  float x;
  float y;
  Two(float x, float y) : x(x), y(y) { }
  Two(Three three) : x(three.x), y(three.y) { }
};

ostream& operator<<(ostream& os, const Three& t)
{
    os << "(" << t.x << ", " << t.y << ", " << t.z << ")";
    return os;
}

ostream& operator<<(ostream& os, const Two& t)
{
    os << "(" << t.x << ", " << t.y << ")";
    return os;
}

// Add more as needed
Three operator+(const Three& x, const Three& y) {
  return Three(x.x + y.x, x.y + y.y, x.z + y.z);
}
Three operator-(const Three& x, const Three& y) {
  return Three(x.x - y.x, x.y - y.y, x.z - y.z);
}
Three operator*(const Three& x, const float& y) {
  return Three(x.x * y, x.y * y, x.z * y);
}
Three operator/(const Three& x, const float& y) {
  return Three(x.x / y, x.y / y, x.z / y);
}

Two operator+(const Two& x, const Two& y) {
  return Two(x.x + y.x, x.y + y.y);
}
Two operator-(const Two& x, const Two& y) {
  return Two(x.x - y.x, x.y - y.y);
}
Two operator*(const Two& x, const float& y) {
  return Two(x.x * y, x.y * y);
}
Two operator/(const Two& x, const float& y) {
  return Two(x.x / y, x.y / y);
}

typedef Three Point;
typedef Three Vec;

typedef Two Point2D;


// Rotate along x axis
Point rotate_x(Point p, float rads) {
  // float rads = angle * PI / 180.0f;
  return Point(p.x, p.y * cos(rads) + p.z * sin(rads), p.z * cos(rads) - p.y * sin(rads));
}

// Rotate along y axis
Point rotate_y(Point p, float rads) {
  // float rads = angle * PI / 180.0f;
  return Point(p.x * cos(rads) + p.z * sin(rads), p.y, p.z * cos(rads) - p.x * sin(rads));
}

// Rotate along z axis
Point rotate_z(Point p, float rads) {
  // float rads = angle * PI / 180.0f;
  return Point(p.x * cos(rads) - p.y * sin(rads), p.y * cos(rads) + p.x * sin(rads), p.z);
}

// Vec is in degrees
// Point rotate_by(Point p, Three v) {
//   return rotate_z(rotate_y(rotate_x(p, v.x), v.y), v.z);
// }

// Magnitude of vector with vals
float dist(vector<float> vals) {
  float sum = 0;
  for (float val : vals) {
    sum += val * val;
  }
  return sqrt(sum);
}

float mag(Vec v) {
  return dist({v.x, v.y, v.z});
}

float mag(Point2D v) {
  return dist({v.x, v.y});
}

Vec normalize(Vec v) {
  return v / mag(v);
}

// Distance between two points
float pdist(Point x, Point y) {
  return dist({x.x - y.x, x.y - y.y, x.z - y.z});
}

// Distance between two points
float pdist(Point2D x, Point2D y) {
  return dist({x.x - y.x, x.y - y.y});
}

// Cross product of two vectors
Vec cross(Vec a, Vec b) {
  return Vec(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// Area of triangle formed by three points in 2D
float area(Point2D a, Point2D b, Point2D c) {
  return 0.5 * abs(a.x * b.y + b.x * c.y + c.x * a.y - a.x * c.y - c.x * b.y - b.x * a.y);
}

// 2D discretized projection of 3D shapes
class Projection {
  int sidelen; // In meters, of projection
  int resolution; // In points per meter

  // Convert from projection pos to float
  float tofloat(int x) {
    return (float)x / (float)resolution - (float)sidelen / 2.0f;
  }

  // Convert from float to projection pos
  int toint(float x) {
    return (int)((x + (float)sidelen / 2.0f) * (float)resolution);
  }

public:
  vector<vector<bool>> data; // True = obscured, False = not obscured

  // Generates a list of points within range to test for obscuring
  vector<pair<float, float>> gen_points(float startx, float stopx, float starty, float stopy) {
    vector<pair<float, float>> points;
    assert(startx >= tofloat(0));
    assert(starty >= tofloat(0));
    assert(stopx < tofloat(sidelen * resolution));
    assert(stopy < tofloat(sidelen * resolution));

    for (int i = toint(startx); i < toint(stopx); i++) {
      for (int j = toint(starty); j < toint(stopy); j++) {
        if (!data[i][j]) {
          points.push_back(make_pair(tofloat(i) + 0.5 / resolution, tofloat(j) + 0.5 / resolution));
        }
      }
    }

    return points;
  }

  void obscure(float x, float y) {
    data[toint(x)][toint(y)] = true;
  }

  Projection(int sidelen, int resolution) : sidelen(sidelen), resolution(resolution), data(sidelen * resolution, vector<bool>(sidelen * resolution, false)) { }
};

function<Point (Point)> gen_rot(Vec norm) { // Generates a rotation function that will rotate norm vector to Z axis and any points along with it
  float to_rot_z = -atan2(norm.y, norm.x);
  float to_rot_y = -PI / 2 + atan2(norm.z, dist({norm.x, norm.y}));
  return [to_rot_z, to_rot_y](Point p) -> Point {
    p = rotate_z(p, to_rot_z); // Rotates so norm is in XZ plane
    p = rotate_y(p, to_rot_y); // Rotates so norm is Z axis
    p = rotate_z(p, -to_rot_z); // Rotates back to undo first rotation
    return p;
  };
}

class Shape {
  // Cast shadow onto plane (proj) defined by normal vector (norm)
  void project(Projection* proj, Vec norm) {
    assert(false);
  }
};

class ConFrustum : public Shape {
public:
  Point bottom;
  Point top;
  float bottom_radius;
  float top_radius;

  void project(Projection* proj, Vec norm) {

    // debug(bottom);
    // debug(top);

    // Rotate points for projection
    // float to_rot_z = -atan2(norm.y, norm.x);
    // float to_rot_y = -PI / 2 + atan2(norm.z, dist({norm.x, norm.y}));
    // auto rot = [&](Point p) -> Point {
    //   p = rotate_z(p, to_rot_z); // Rotates so norm is in XZ plane
    //   p = rotate_y(p, to_rot_y); // Rotates so norm is Z axis
    //   p = rotate_z(p, -to_rot_z); // Rotates back to undo first rotation
    //   return p;
    // };
    auto rot = gen_rot(norm);

    // These are the circles projected onto the plane
    Point new_bottom = rot(bottom);
    Point new_top = rot(top);

    Point2D new_bottom_2D(new_bottom);
    Point2D new_top_2D(new_top);

    float maxdist = pdist(new_bottom_2D, new_top_2D) / 2 + max(bottom_radius, top_radius); // Maximum distance from center it can project
    Point2D middle = (new_bottom_2D + new_top_2D) / 2;
    float startx = middle.x - maxdist;
    float stopx = middle.x + maxdist;
    float starty = middle.y - maxdist;
    float stopy = middle.y + maxdist;

    // debug(norm)
    // debug(middle);
    // debug(startx);
    // debug(stopx);
    // debug(starty);
    // debug(stopy);

    // debug(to_rot_z);
    // debug(to_rot_y);

    // debug(new_bottom);
    // debug(new_top);

    Vec v = normalize(new_top - new_bottom); // Vector circles are "pointing" in

    // Same thing as above but to test for circle intersection
    // float circle_to_rot_z = -atan2(v.y, v.x);
    // float circle_to_rot_y = -PI / 2 + atan2(v.z, dist({v.x, v.y})) ;
    // auto circle_rot = [&](Point p) -> Point {
    //   p = rotate_z(p, circle_to_rot_z);
    //   p = rotate_y(p, circle_to_rot_y);
    //   p = rotate_z(p, -circle_to_rot_z);
    //   return p;
    // };
    auto circle_rot = gen_rot(v);

    // debug(circle_to_rot_z);
    // debug(circle_to_rot_y);

    // These are the circles projected onto the plane they lie on
    Point new_new_bottom = circle_rot(new_bottom); // In this one, the circle should be flat
    Point new_new_top = circle_rot(new_top);

    // debug(new_new_bottom);
    // debug(new_new_top);


    // TODO: Fix trapezoid to be correct (see https://math.stackexchange.com/questions/882067/given-two-circles-find-the-length-of-a-pulley-belt-that-connects-the-two)
    float trapezoid_area = pdist(new_bottom_2D, new_top_2D) * (bottom_radius + top_radius);
    Point2D slope(new_top.y - new_bottom.y, new_bottom.x - new_top.x);
    slope = slope / mag(slope);
    Point2D trap1 = new_bottom_2D + slope * bottom_radius;
    Point2D trap2 = new_bottom_2D - slope * bottom_radius;
    Point2D trap3 = new_top_2D + slope * top_radius;
    Point2D trap4 = new_top_2D - slope * top_radius;

    // debug(trapezoid_area)
    // debug(trap1)
    // debug(trap2)
    // debug(trap3)
    // debug(trap4)
    // debug(slope)

    auto points = proj->gen_points(startx, stopx, starty, stopy);

    for (auto point : points) {
      float x = point.first;
      float y = point.second;
      bool obscure = false;

      // cout << "Point: " << x << " " << y << endl;

      Point2D p(x, y);

      Point p1(x, y, 0);
      Point p2(x, y, 1); // Used to draw a line once rotated

      p1 = circle_rot(p1);
      p2 = circle_rot(p2);


      Vec pv = p2 - p1; // Vector pointing from p1 to p2;

      // debug(p1)
      // debug(p2)
      // debug(pv)

      float xslope = pv.z / pv.x;
      float yslope = pv.z / pv.y;

      // debug(xslope)
      // debug(yslope)

      float xhitbottom = p1.x - (p1.z - new_new_bottom.z) / xslope; // Where the line drawn by p1 and p2 intercepts the plane with the bottom circle in the x position
      float yhitbottom = p1.y - (p1.z - new_new_bottom.z) / yslope; // Where the line drawn by p1 and p2 intercepts the plane with the bottom circle in the y position

      float xhittop = p1.x - (p1.z - new_new_top.z) / xslope; // Where the line drawn by p1 and p2 intercepts the plane with the top circle in the x position
      float yhittop = p1.y - (p1.z - new_new_top.z) / yslope; // Where the line drawn by p1 and p2 intercepts the plane with the top circle in the y position

      if (dist({xhitbottom - new_new_bottom.x, yhitbottom - new_new_bottom.y}) < bottom_radius) { // Is in bottom circle
        obscure = true;
      } else if (dist({xhittop - new_new_top.x, yhittop - new_new_top.y}) < top_radius) { // Is in top circle
        obscure = true;
      } else if (area(p, trap1, trap2) + area(p, trap2, trap3) + area(p, trap3, trap4) + area(p, trap4, trap1) < trapezoid_area) { // Is in inner trapezoid
        obscure = true;
      }

      if (obscure) {
        proj->obscure(x, y);
      }
    }
  }

  ConFrustum(Point bottom, Point top, float bottom_radius, float top_radius) : bottom(bottom), top(top), bottom_radius(bottom_radius), top_radius(top_radius) { }
};

class Triangle : public Shape {
public:
  Point p1;
  Point p2;
  Point p3;

  void project(Projection* proj, Vec norm) {
    auto rot = gen_rot(norm);

    Point2D p1_2D(rot(p1));
    Point2D p2_2D(rot(p2));
    Point2D p3_2D(rot(p3));

    float startx = min({p1_2D.x, p2_2D.x, p3_2D.x});
    float stopx = max({p1_2D.x, p2_2D.x, p3_2D.x});
    float starty = min({p1_2D.y, p2_2D.y, p3_2D.y});
    float stopy = max({p1_2D.y, p2_2D.y, p3_2D.y});

    float triangle_area = area(p1_2D, p2_2D, p3_2D);

    debug(triangle_area)

    auto points = proj->gen_points(startx, stopx, starty, stopy);

    for (auto point : points) {
      Point2D p(point.first, point.second);

      if (area(p, p1_2D, p2_2D) + area(p, p2_2D, p3_2D) + area(p, p3_2D, p1_2D) < triangle_area * 1.001) { // Extra .001 needed for reasons
        proj->obscure(p.x, p.y);
      }
    }
  }

  Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) { }
};

class Object3D {
  vector<Shape> shapes;
};




class Rocket {
public:
  Point center_of_mass;
  Point rotational_inertia;
  // Point anchor;
  Object3D shape;
  float mass; // in kg
  float impulse; // in N
  float burntime; // in s

  Rocket(Point com, Point rotational_inertia, float mass, float impulse, float burntime) : center_of_mass(com), rotational_inertia(rotational_inertia), mass(mass), impulse(impulse), burntime(burntime) { }
};

class Simulation {
public:
  float simspeed = 100; // Hertz

  int ticks = 0;

  Rocket rocket;
  Point rocketpos;
  Vec rocketvel;
  Vec rocketdir;

  void tick() {
    Vec accel(0, 0, -9.81);
    if (ticks < (int)(rocket.burntime * simspeed)) {
      accel = accel + rocketdir * (rocket.impulse / rocket.mass);
    }

    rocketvel = rocketvel + accel / simspeed;
    rocketpos = rocketpos + rocketvel / simspeed;

    ticks += 1;
  }

  Simulation(Rocket rocket) : rocket(rocket), rocketpos(0, 0, 0), rocketvel(0, 0, 0), rocketdir(0, 0, 1) { }
};

int main () {

  for (float i = 0; i < 100; i += 0.2) {

    Projection proj(PROJECTION_SIZE, PROJECTION_RES);
    ConFrustum body(Point(0, 0, 0), Point(0, 0, 1), 0.1, 0.1);
    ConFrustum body2(Point(0, 0, 1), Point(0, 0, 1.2), 0.1, 0.07);
    ConFrustum body3(Point(0, 0, 1.2), Point(0, 0, 1.4), 0.07, 0.02);
    ConFrustum body4(Point(0, 0, 1.4), Point(0, 0, 1.5), 0.02, 0);
    Triangle fin1(Point(-0.1, 0, 0), Point(-0.2, 0, 0), Point(-0.1, 0, 0.2));
    Triangle fin2(Point(0.1, 0, 0), Point(0.2, 0, 0), Point(0.1, 0, 0.2));
    Triangle fin3(Point(0, -0.1, 0), Point(0, -0.2, 0), Point(0, 0.1, 0.2));
    Triangle fin4(Point(0, 0.1, 0), Point(0, 0.2, 0), Point(0, 0.1, 0.2));
    // body.project(&proj, Vec(1, 0, 0));
    // body.project(&proj, Vec(0.7071067812, 0, 0.7071067812));
    // body.project(&proj, Vec(0, 0.8660254038, 0.5));

    // Vec tempvec(cos(i / 2), cos(i), sin(i));
    Vec tempvec(0, 1, 1);
    tempvec = tempvec / mag(tempvec);
    body.project(&proj, tempvec);
    body2.project(&proj, tempvec);
    body3.project(&proj, tempvec);
    body4.project(&proj, tempvec);
    fin1.project(&proj, tempvec);
    fin2.project(&proj, tempvec);
    fin3.project(&proj, tempvec);
    fin4.project(&proj, tempvec);

    // body2.project(&proj, Vec(0, cos(i), sin(i)));
    // body3.project(&proj, Vec(0, cos(i), sin(i)));

    string str = "";
    for (int i = 0; i < PROJECTION_RES * PROJECTION_SIZE - 1; i += 2) {
      string str2 = "";
      for (int j = 0; j < PROJECTION_RES * PROJECTION_SIZE; j++) {
        if (proj.data[j][i] && proj.data[j][i + 1]) {
          str2 += "█";
        } else if (proj.data[j][i] && !proj.data[j][i + 1]) {
          str2 += "▄";
        } else if (!proj.data[j][i] && proj.data[j][i + 1]) {
          str2 += "▀";
        } else {
          str2 += " ";
        }
      }
      str = str2 + '\n' + str;
    }
    cout << str << endl;

    usleep(100000);
  }

  // Point center_of_mass(0, 0, 0.5);
  // Point rotational_inertia(0.083333, 0.083333, 0.00005);
  // float mass = 1;
  // float impulse = 100;
  // float burntime = 1;
  //
  //
  // Rocket rocket(center_of_mass, rotational_inertia, mass, impulse, burntime);
  // Simulation sim(rocket);
  //
  // while (sim.rocketpos.z > 0 || sim.ticks == 0) {
  //   sim.tick();
  //   cout << "Pos: " << sim.rocketpos << endl;
  //   // cout << sim.rocketpos.z << " ";
  // }
}
