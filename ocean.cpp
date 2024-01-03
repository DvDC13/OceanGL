#include "ocean.h"

namespace Ocean
{
    Ocean::Ocean(int resolutionX, int resolutionZ, int gridWidth, int gridHeight, glm::vec2 windDirection, float windSpeed, float amplitude, float lambda)
    {
        m_resolutionX = resolutionX;
        m_resolutionZ = resolutionZ;
        m_gridWidth = gridWidth;
        m_gridHeight = gridHeight;
        m_windDirection = windDirection;
        m_windSpeed = windSpeed;
        m_amplitude = amplitude;
        m_lambda = lambda;

        int resolution = m_resolutionX * m_resolutionZ;

        m_heights.resize(resolution);
        m_normals.resize(resolution);

        std::fill(m_heights.begin(), m_heights.end(), glm::vec3(0.0f, 0.0f, 0.0f));
        std::fill(m_normals.begin(), m_normals.end(), glm::vec3(0.0f, 0.0f, 0.0f));

        m_h0.reserve(resolution);
        m_h0_conj.reserve(resolution);
        m_h.reserve(resolution);

        std::fill(m_h0.begin(), m_h0.end(), std::complex<float>(0.0f, 0.0f));
        std::fill(m_h0_conj.begin(), m_h0_conj.end(), std::complex<float>(0.0f, 0.0f));
        std::fill(m_h.begin(), m_h.end(), std::complex<float>(0.0f, 0.0f));

        m_generator.seed(time(NULL));

        m_distribution = std::normal_distribution<float>(0.0f, 1.0f);

        generateWaves();
    }

    Ocean::~Ocean()
    {
        m_heights.clear();
        m_normals.clear();
    }

    void Ocean::generateWaves()
    {
        for (int z = 0; z < m_resolutionZ; z++)
        {
            for (int x = 0; x < m_resolutionX; x++)
            {
                int index = z * m_resolutionX + x;
                glm::vec2 k = getKvector(x, z);
                
                float epsilon_r = m_distribution(m_generator);
                float epsilon_i = m_distribution(m_generator);

                float P_h = sqrtf(phillipsSpectrum(k));

                std::complex<float> h(sqrtf(0.5f) * epsilon_r * P_h, sqrtf(0.5f) * epsilon_i * P_h);

                m_h0.push_back(h);

                m_h0_conj.push_back(std::conj(h));
            }
        }
    }

    void Ocean::updateWaves(float time)
    {
        fftwf_complex* in_h;
        fftwf_complex* in_D_x;
        fftwf_complex* in_D_z;

        std::vector<std::complex<float>> D_x = std::vector<std::complex<float>>(m_resolutionX * m_resolutionZ);
        std::vector<std::complex<float>> D_z = std::vector<std::complex<float>>(m_resolutionX * m_resolutionZ);

        for (int z = 0; z < m_resolutionZ; z++)
        {
            for (int x = 0; x < m_resolutionX; x++)
            {
                int index = z * m_resolutionX + x;

                glm::vec2 k = getKvector(x, z);
                float kLength = glm::length(k);

                float omega = omegaCompute(kLength);

                std::complex<float> h0 = m_h0[index];
                std::complex<float> h0_conj = m_h0_conj[index];

                std::complex<float> h0_t = h0 * std::exp(std::complex<float>(0.0f, omega * time));
                std::complex<float> h0_conj_t = h0_conj * std::exp(std::complex<float>(0.0f, -omega * time));

                std::complex<float> h = h0_t + h0_conj_t;

                m_h[index] = h;

                glm::vec2 k_normalized = glm::normalize(k);

                float kx = k_normalized.x;
                float kz = k_normalized.y;

                D_x[index] = std::complex<float>(0.0f, -kx) * m_h[index];
                D_z[index] = std::complex<float>(0.0f, -kz) * m_h[index];
            }
        }

        in_h = (fftwf_complex*)m_h.data();
        in_D_x = (fftwf_complex*)D_x.data();
        in_D_z = (fftwf_complex*)D_z.data();

        fftwf_complex* out_h = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * m_resolutionX * m_resolutionZ);
        fftwf_complex* out_D_x = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * m_resolutionX * m_resolutionZ);
        fftwf_complex* out_D_z = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * m_resolutionX * m_resolutionZ);

        fftwf_plan plan_h = fftwf_plan_dft_2d(m_resolutionX, m_resolutionZ, in_h, out_h, FFTW_FORWARD, FFTW_ESTIMATE);
        fftwf_plan plan_D_x = fftwf_plan_dft_2d(m_resolutionX, m_resolutionZ, in_D_x, out_D_x, FFTW_FORWARD, FFTW_ESTIMATE);
        fftwf_plan plan_D_z = fftwf_plan_dft_2d(m_resolutionX, m_resolutionZ, in_D_z, out_D_z, FFTW_FORWARD, FFTW_ESTIMATE);

        fftwf_execute(plan_h);
        fftwf_execute(plan_D_x);
        fftwf_execute(plan_D_z);

        for (int z = 0; z < m_resolutionZ; z++)
        {
            for (int x = 0; x < m_resolutionX; x++)
            {
                int index = z * m_resolutionX + x;

                float sign = (x + z) % 2 == 0 ? 1.0f : -1.0f;

                m_heights[index] = glm::vec3(
                    x - (m_resolutionX / 2.0f) * m_gridWidth / m_resolutionX - sign * m_lambda * out_h[index][0],
                    sign * m_lambda * out_h[index][0],
                    z - (m_resolutionZ / 2.0f) * m_gridHeight / m_resolutionZ - sign * m_lambda * out_h[index][1]
                );
            }
        }

        fftwf_destroy_plan(plan_h);
        fftwf_destroy_plan(plan_D_x);
        fftwf_destroy_plan(plan_D_z);

        fftwf_free(in_h);
        fftwf_free(in_D_x);
        fftwf_free(in_D_z);
        fftwf_free(out_h);
        fftwf_free(out_D_x);
        fftwf_free(out_D_z);
    }

    float Ocean::phillipsSpectrum(glm::vec2 k)
    {
        k = glm::normalize(k);

        float L = m_windSpeed * m_windSpeed / m_gravity;

        float kLength = glm::length(k);
        float kLength2 = kLength * kLength;
        float kLength4 = kLength2 * kLength2;

        float kDotW = glm::dot(glm::normalize(k), glm::normalize(m_windDirection));
        float kDotW2 = kDotW * kDotW;

        float wLength = m_windSpeed * m_windSpeed;
        float wLength2 = wLength * wLength;

        float damping = 0.001f;
        float l2 = L * L;

        return m_amplitude * exp(-1.0f / (kLength2 * l2)) / kLength4 * kDotW2 * exp(-kLength2 * damping * damping);
    }

    float Ocean::omegaCompute(float k)
    {
        return sqrt(m_gravity * k);
    }

    glm::vec2 Ocean::getKvector(int x, int y)
    {
        float kx = 2.0f * m_pi * (x - m_resolutionX / 2.0f) / m_gridWidth;
        float kz = 2.0f * m_pi * (y - m_resolutionZ / 2.0f) / m_gridHeight;

        return glm::vec2(kx, kz);
    }
}