#pragma once

#include <complex>
#include <vector>
#include <random>

#include "fftw/fftw3.h"
#include "glm/glm.hpp"

namespace Ocean
{
    class Ocean
    {
        public:
            Ocean(int resolutionX, int resolutionZ, int gridWidth, int gridHeight, glm::vec2 windDirection, float windSpeed, float amplitude, float lambda);
            ~Ocean();

            void generateWaves();
            void updateWaves(float time);

            inline int getResolutionX() const { return m_resolutionX; }
            inline int getResolutionZ() const { return m_resolutionZ; }
            inline int getGridWidth() const { return m_gridWidth; }
            inline int getGridHeight() const { return m_gridHeight; }
            inline glm::vec2 getWindDirection() const { return m_windDirection; }
            inline float getWindSpeed() const { return m_windSpeed; }
            inline float getAmplitude() const { return m_amplitude; }

            inline std::vector<glm::vec3> getHeights() const { return m_heights; }
            inline std::vector<glm::vec3> getNormals() const { return m_normals; }

            float phillipsSpectrum(glm::vec2 k);
            float omegaCompute(float k);

            glm::vec2 getKvector(int x, int z);

        private:
            int m_resolutionX;
            int m_resolutionZ;
            int m_gridWidth;
            int m_gridHeight;

            const float m_pi = 3.14159265358979323846f;
            const float m_gravity = 9.81f;

            glm::vec2 m_windDirection;
            float m_windSpeed;

            float m_amplitude;
            float m_lambda;

            std::vector<glm::vec3> m_heights;
            std::vector<glm::vec3> m_normals;

            std::vector<std::complex<float>> m_h0;
            std::vector<std::complex<float>> m_h0_conj;
            std::vector<std::complex<float>> m_h;

            std::default_random_engine m_generator;
            std::normal_distribution<float> m_distribution;
    };
}