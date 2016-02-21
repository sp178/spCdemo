#ifndef SPCRAND_HPP
#define SPCRAND_HPP
//this class to gengeator random number with nomoal ditrabution
#include<boost/random.hpp>
#include<time.h>
#include<boost/circular_buffer.hpp>
#include<algorithm>
using namespace  boost ;
class spcseed
{
public:
       static clock_t GetSeed()
        {
            return clock();
        }
};

template<typename seed = spcseed,typename engine=boost::mt19937,typename distribution = boost::normal_distribution<> >
class spcrand
{
public:
    spcrand(double _mean=0,double _sigma=1);
    double operator()();
private:
    seed    m_seed;
    engine m_engine;
    distribution m_distribution;
    variate_generator<engine, distribution > m_random;
};

template<typename seed,typename engine,typename generator>
spcrand<seed,engine,generator>::spcrand(double _mean,double _sigma):m_engine(seed::GetSeed()),m_distribution(_mean,_sigma),m_random(m_engine,m_distribution)
{

};

template<typename seed,typename engine,typename generator>
double spcrand<seed,engine,generator>::operator ()()
{
    return m_random();
};
#include<vector>
using namespace std;

template<int staticnum = 50,typename random = spcrand<> >
class spcstaticram
{
public:
    spcstaticram(random& _random);
    int Initial();
    double operator ()();
    double GetSigma();

    double GetMean();


private:
    double                m_addall;
    size_t                  m_size;
    random&            m_random;
    circular_buffer<double> m_data;
};

template<int staticnum,typename random>
spcstaticram<staticnum,random>::spcstaticram(random& _random):m_data(staticnum),m_random(_random)
{
        m_addall = 0;
        m_size = 0;
};
template<int staticnum,typename random>
int spcstaticram<staticnum,random>::Initial()
{
        m_addall = 0;
        m_size = 0;
        m_data.clear();
        return 0;
}

template<int staticnum,typename random>
double spcstaticram<staticnum,random>::operator ()()
{
    double _tempdata = m_random();
    if(staticnum>m_size) ++m_size;
    if(m_data.full())
        m_addall-=(*m_data.begin());
    m_addall+=_tempdata;
    m_data.push_back(_tempdata);
    return _tempdata;
};
template<int staticnum,typename random>
double spcstaticram<staticnum,random>::GetMean()
{
    if(0==m_size) return 0;
    return m_addall/m_size;
}

template<int staticnum,typename random>
double spcstaticram<staticnum,random>::GetSigma()
{
    if(2>m_size) return 0;
    double _tempmean = GetMean();
    double _tempsigma = 0;
    for_each(m_data.begin(),\
             m_data.end(),\
             [_tempmean,&_tempsigma](double _data){\
           _tempsigma+=pow(_tempmean-_data,2);}
    );
     return sqrt(_tempsigma/(m_size-1));
}



#endif // SPCRAND_HPP
