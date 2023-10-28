#pragma once

#include "rtweekend.h"

class perlin
{
public:
    perlin()
    {
        ranvec = new vec3[point_count];
        for (int i = 0; i < point_count; ++i)
            ranvec[i] = normalize(vec3::random(-1, 1));

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~perlin()
    {
        delete[] ranvec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double noise(const point3 &p) const
    {
        auto frac_i = p.x() - floor(p.x());
        auto frac_j = p.y() - floor(p.y());
        auto frac_k = p.z() - floor(p.z());

        auto i = static_cast<int>(floor(p.x()));
        auto j = static_cast<int>(floor(p.y()));
        auto k = static_cast<int>(floor(p.z()));
        vec3 c[2][2][2];

        for (int di = 0; di < 2; ++di)
            for (int dj = 0; dj < 2; ++dj)
                for (int dk = 0; dk < 2; ++dk)
                    c[di][dj][dk] = ranvec[perm_x[(di + i) & 255] ^ perm_y[(dj + j) & 255] ^ perm_z[(dk + k) & 255]];

        return trilinear_interp(c, frac_i, frac_j, frac_k);
    }

    double turb(const point3 &p, int depth = 7) const
    {
        auto accm = 0.0;
        auto tmp = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i)
        {
            accm += weight * noise(tmp);
            weight *= 0.5;
            tmp *= 2;
        }

        return fabs(accm); // perlin.noise(p) could return negative value
    }

private:
    static const int point_count = 256;
    vec3 *ranvec;
    int *perm_x;
    int *perm_y;
    int *perm_z;

    static int *perlin_generate_perm()
    {
        auto p = new int[point_count];

        for (int i = 0; i < point_count; ++i)
            p[i] = i;

        permute(p, point_count);
        return p;
    }

    static void permute(int *p, int n)
    {
        for (int i = n - 1; i > 0; --i)
        {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double trilinear_interp(vec3 c[2][2][2], double u, double v, double w)
    {
        // Hermitian Smoothing
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);

        double accum = 0;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k)
                {
                    vec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
                }

        return accum;
    }
};