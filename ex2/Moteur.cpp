#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

struct Vec3 {
    float x, y, z;
    Vec3(float a=0, float b=0, float c=0) : x(a), y(b), z(c) {}
    Vec3 operator-(const Vec3& v) const { return Vec3(x-v.x,y-v.y,z-v.z); }
    float norme() const { return std::sqrt(x*x + y*y + z*z); }
    Vec3 normalise() const { 
        float n = norme();
        if (n == 0) return Vec3();
        return Vec3(x/n,y/n,z/n);
    }
    float dot(const Vec3& v) const { return x*v.x + y*v.y + z*v.z; }
};

class Lumiere {
public:
    virtual float intensite(const Vec3& pos) const = 0;
    virtual ~Lumiere() {}
};

class LumiereDirectionnelle : public Lumiere {
    Vec3 direction;
    float intensiteConst;
public:
    LumiereDirectionnelle(const Vec3& dir, float i) : direction(dir.normalise()), intensiteConst(i) {}
    float intensite(const Vec3&) const override { return intensiteConst; }
};

class LumierePonctuelle : public Lumiere {
    Vec3 position;
    float intensiteMax;
    float attenuation;
public:
    LumierePonctuelle(const Vec3& pos, float i, float att) : position(pos), intensiteMax(i), attenuation(att) {}
    float intensite(const Vec3& p) const override {
        float d = (p - position).norme();
        return intensiteMax / (1 + attenuation * d * d);
    }
};

class LumiereSpot : public Lumiere {
    Vec3 position, direction;
    float angleRad, intensiteMax;
public:
    LumiereSpot(const Vec3& pos, const Vec3& dir, float angleDeg, float i) :
        position(pos), direction(dir.normalise()), intensiteMax(i), angleRad(angleDeg * 3.14159f / 180.f) {}
    float intensite(const Vec3& p) const override {
        Vec3 v = (p - position).normalise();
        float angle = std::acos(direction.dot(v));
        if (angle > angleRad/2) return 0;
        float d = (p - position).norme();
        return intensiteMax * std::cos(angle) / (1 + d*d);
    }
};

float calculerEclairage(const Vec3& p, const std::vector<std::unique_ptr<Lumiere>>& lumieres) {
    float total = 0;
    for (const auto& l : lumieres) total += l->intensite(p);
    return total;
}

int main() {
    std::vector<std::unique_ptr<Lumiere>> lumieres;
    lumieres.push_back(std::make_unique<LumiereDirectionnelle>(Vec3(1,-1,0), 0.5f));
    lumieres.push_back(std::make_unique<LumierePonctuelle>(Vec3(5,5,0), 2.0f, 0.1f));
    lumieres.push_back(std::make_unique<LumiereSpot>(Vec3(2,2,0), Vec3(-1,-1,0), 45, 3.0f));

    std::vector<Vec3> points = {{0,0,0}, {3,3,0}, {6,6,0}};

    for (auto& pt : points) {
        std::cout << "Eclairage en (" << pt.x << "," << pt.y << "," << pt.z << ") = "
                  << calculerEclairage(pt, lumieres) << "\n";
    }
    return 0;
}
