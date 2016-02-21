#ifndef SPCODE_H
#define SPCODE_H
#include<gsl/gsl_errno.h>
#include<gsl/gsl_odeiv2.h>
#include<list>
#include<boost/array.hpp>
using namespace boost;
using namespace std;
typedef int(*func)(void* param,double time,const double* _in,const double* _out,const double*x,double *dxdt);
typedef int(*jac) (double t, const double* y, double *dfdx,
double* dfdt, void *params);
struct __MODEL{
    char        m_name[255];                //model name
    double* m_pout;
    double* m_pin;
    void*      m_pparam;
    size_t      m_inlength;
    size_t      m_outlength;
    size_t      m_statalength;
    double m_step;      //run step but in this case this data is not use ,just for future reserver.
    int(*m_func)(void* param,double time,const double* _in,const double* _out,const double*x,double *dxdt);
    int(*m_jac) (double t, const double* y, double *dfdx,double* dfdt, void *params);
    void*   (*m_assist)(int ,void*); //assist function with different index may have different beheaever .return different data type.
};
typedef enum {
    ODE_RK23 = 1,
    ODE_RK4 = 2,
    ODE_RK45 = 3,
    ODE_RK89 = 4
}_ODESTEP;

class spcode
{
public:
    spcode();
    int Initial();
    int AddModel(__MODEL);
    int AddModel(__MODEL*);
    int AddModel(__MODEL&);
    int SetToRunModel(char*);
    int SetRunModel(char *,size_t,size_t,size_t,void*);       //in out stata param
    int OneStep();                                      //run one step
    int Ready();
    int SetOdeStepType(_ODESTEP );
private:
    std::list<std::pair<double*,double*> > m_inout;
    std::list<__MODEL>       m_modelall;
    std::list<__MODEL>       m_run_model;
    gsl_odeiv2_system m_odesys;
    gsl_odeiv2_driver   *m_driver;
    gsl_odeiv2_step_type* m_step_type;
    double* m_pstata;               //stata pointer
    size_t m_statalength;           //stata length
    double m_time;
    double m_timeend;
    int         m_status;
    _ODESTEP    m_odetype;
private:
    static int RunFunction (double t, const double* y, double* dydt, void *params);
    static int Evajacobian (double t, const double* y, double* dfdy, double* dfdt,
                     void *params);
};
#endif // SPCODE_H
