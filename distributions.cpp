#include "distributions.hpp"

#include <format>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<DistributionPair>

generatePoissonDistribution(std::uint32_t howMany, std::uint8_t howOften,
                            std::uint8_t numberBins)
{
    std::vector<DistributionPair> result;
    std::random_device rd;
    std::default_random_engine engine{ rd() };
    std::poisson_distribution<int> myDistribution(howOften);
    int binWidth = static_cast<int>((howOften * 3) / numberBins);

    // creating the vector of bins
    for (int j = 0; j < howOften * 3 - 1; j += binWidth)
    {
        result.push_back(DistributionPair(j, j + binWidth - 1));
    }

    // creating random numbers and adding it to the bins
    for (uint32_t i = 0; i < howMany; i++)
    {
        int randNumber = myDistribution(engine);
        if (randNumber > howOften * 3 - 1)
        {
            result[howOften * 3 - 1].count = result[howOften * 3 - 1].count + 1;
        }
        else
        {
            int index = static_cast<int>(floor(randNumber / binWidth));
            result[index].count = result[index].count + 1;
        }
    }
    return result;
}

std::uint32_t maxCount(const std::vector<DistributionPair>& distribution)
{
    std::uint32_t maxCount = 0;
    for (DistributionPair eachPair : distribution)
    {
        if (eachPair.count > maxCount)
        {
            maxCount = eachPair.count;
        }
    }
    return maxCount;
}

std::vector<DistributionPair>
generateUniformDistribution(std::uint32_t howMany, std::uint32_t min,
                            std::uint32_t max, std::uint8_t numberBins)
{

    std::vector<DistributionPair> result;
    std::random_device rd;
    std::default_random_engine engine{ rd() };
    std::uniform_int_distribution<int> myDistribution(min, max);

    // creating the vector of bins
    uint32_t binWidth = static_cast<int>(floor((max - min + 1) / numberBins));
    for (uint32_t j = min; j < max + 1; j = j + binWidth)
    {
        result.push_back(DistributionPair(j, j + binWidth - 1));
    }

    // creating random numbers and adding it to the bins
    for (uint32_t i = 0; i < howMany; i++)
    {
        int randNumber = myDistribution(engine);
        int binIndex = (randNumber - min) / binWidth;
        result[binIndex].count = result[binIndex].count + 1;
    }
    return result;
}

std::vector<DistributionPair>
generateNormalDistribution(std::uint32_t howMany, float mean, float stdev,
                           std::uint8_t numberBins)
{
    uint32_t minValToBin = static_cast<int>(floor(mean - (4 * stdev)));
    uint32_t maxValToBin = static_cast<int>(floor(mean + (4 * stdev)));

    std::random_device rd;
    std::default_random_engine engine{ rd() };
    std::normal_distribution<float> myDistribution(mean, stdev);
    std::vector<DistributionPair> result;
    uint32_t binWidth = static_cast<int>(floor((maxValToBin - minValToBin) / numberBins));

    // creating the vector of bins
    for (uint32_t j = minValToBin; j < maxValToBin; j += binWidth)
    {
        result.push_back(DistributionPair(j, j + binWidth - 1));
    }

    // Creating random numbers and adding it to the bins
    for (uint32_t i = 0; i < howMany; i++)
    {
        double randNumber = myDistribution(engine);
        double floorNumber = std::floor(randNumber);
        int binIndex = static_cast<int>((floorNumber - minValToBin) / binWidth);

        if (binIndex >= 0 && binIndex <= numberBins - 1)
        {
            result[binIndex].count = result[binIndex].count + 1;
        }

        else if (binIndex > numberBins)
        {
            result[numberBins - 1].count = result[numberBins - 1].count + 1;
        }
        else
        {
            result[0].count++;
        }
    }

    return result;
}

void plotDistribution(std::string title,
                      const std::vector<DistributionPair>& distribution,
                      const std::uint8_t maxPlotLineSize)
{
    // print title, find max count, figure out scale factor
    std::cout << title
              << std::endl;
    int maximumCount = maxCount(distribution);
    double scaleFactor = static_cast<double>(maxPlotLineSize) / maximumCount;

    for (DistributionPair eachPair : distribution)
    {
        int numberToPlot = static_cast<int>(eachPair.count * scaleFactor);

        std::cout << "[" << std::format("{:3}", eachPair.minValue) << ", "
                  << std::format("{:3}", eachPair.maxValue) << "] : ";

        std::cout << std::string(numberToPlot, '*') << std::endl;
    }
    std::cout << "\n";
}