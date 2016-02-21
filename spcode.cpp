#include "spcode.h"
#include<cstring>
spcode::spcode()
{
    //change data
    m_step_type= NULL;
    m_driver = NULL;
};
int spcode::RunFunction (double t, const double* y, double* dydt, void *params)
{
    if(!params) return -1;
    int _status = GSL_SUCCESS;
    spcode* theode = (spcode*)params;
     list<__MODEL>::const_iterator  _citera= theode->m_run_model.end();
    list<__MODEL>::iterator _iter = theode->m_run_model.begin();
    while (_citera!=_iter && !_status) {
        _status = _iter->m_func(_iter->m_pparam,t,_iter->m_pin,_iter->m_pout,y,dydt);
        ++_iter;
    }
    return _status;
}

int spcode::Evajacobian (double t, const double* y, double* dfdy, double* dfdt,
                 void *params)
{
    return GSL_SUCCESS;
}

int spcode::AddModel(__MODEL _themodel)
{
    m_modelall.push_back(_themodel);
    return m_modelall.size();
}

int spcode::AddModel(__MODEL* _pthemodel)
{
    m_modelall.push_back(*_pthemodel);
    return m_modelall.size();
}

int spcode::AddModel(__MODEL& _rthemodel)
{
    m_modelall.push_back(_rthemodel);
    return m_modelall.size();
};

int spcode::SetToRunModel(char* _name)
{
    list<__MODEL>::iterator  _itera = m_modelall.begin();
    list<__MODEL>::const_iterator  _modelend = m_modelall.end();
    while(_modelend!=_itera)
    {
        if(!strcmp(_itera->m_name,_name))
        {
            m_run_model.push_back(*_itera);
            return 0;
        }
        ++_itera;
    }
    return -1;
}

int spcode::Ready()
{
    if(m_run_model.size()<1 || !m_step_type || 1>m_statalength)return -1;
        m_odesys.function = RunFunction;
        m_odesys.jacobian = Evajacobian;
        m_odesys.dimension =  m_statalength;
        m_odesys.params =  (void*)this;
        if(!m_driver)
        {
            gsl_odeiv2_driver_free(m_driver);
            m_driver = NULL;
        }
        switch (m_odetype) {
        case ODE_RK23:
            m_driver = gsl_odeiv2_driver_alloc_y_new(&m_odesys, gsl_odeiv2_step_rk2,
                                                                         1e-6, 1e-6, 0.0);
            break;
        case ODE_RK4:
            m_driver = gsl_odeiv2_driver_alloc_y_new(&m_odesys, gsl_odeiv2_step_rk4,
                                                                         1e-6, 1e-6, 0.0);
            break;
        case ODE_RK45:
            m_driver = gsl_odeiv2_driver_alloc_y_new(&m_odesys, gsl_odeiv2_step_rkf45,
                                                                         1e-6, 1e-6, 0.0);
            break;
        case ODE_RK89:
            m_driver = gsl_odeiv2_driver_alloc_y_new(&m_odesys, gsl_odeiv2_step_rk8pd,
                                                                         1e-6, 1e-6, 0.0);
            break;
        default:
            m_driver = gsl_odeiv2_driver_alloc_y_new(&m_odesys, gsl_odeiv2_step_rk4,
                                                                         1e-6, 1e-6, 0.0);
            break;
        }
        if(!m_driver) return -1;
        return 0;
};

int spcode::SetOdeStepType(_ODESTEP  _step)
{
    m_odetype = _step;
    return 0;
}

int spcode::OneStep()
{
     m_status = gsl_odeiv2_driver_apply (m_driver, &m_time, m_timeend, m_pstata);
    return m_status;
}
