/*
 * gaussiannoise.h
 *
 *  Created on: Nov 7, 2013
 *      Author: quannt
 */

#ifndef GAUSSIANNOISE_H_
#define GAUSSIANNOISE_H_

class GaussianNoise
{
    private:
        double muy;
        double sigma;
    public:
        GaussianNoise(double _muy, double _sigma);
        double nextNoise();
};

#endif /* GAUSSIANNOISE_H_ */
