//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EqualGradientConstraint.h"
#include "SubProblem.h"
#include "FEProblem.h"

registerMooseObject("MooseApp", EqualGradientConstraint);

InputParameters
EqualGradientConstraint::validParams()
{
  InputParameters params = ADMortarConstraint::validParams();
  params.addClassDescription(
      "EqualGradientConstraint enforces continuity of a gradient component between slave and "
      "master sides of a mortar interface using lagrange multipliers");
  params.addRequiredParam<unsigned int>("component", "Gradient component to constrain");
  return params;
}

EqualGradientConstraint::EqualGradientConstraint(const InputParameters & parameters)
  : ADMortarConstraint(parameters), _component(getParam<unsigned int>("component"))
{
}

ADReal
EqualGradientConstraint::computeQpResidual(Moose::MortarType mortar_type)
{
  switch (mortar_type)
  {
    case Moose::MortarType::Slave:
      return -_lambda[_qp] * _grad_test_slave[_i][_qp](_component);
    case Moose::MortarType::Master:
      return _lambda[_qp] * _grad_test_master[_i][_qp](_component);
    case Moose::MortarType::Lower:
      return (_grad_u_master[_qp](_component) - _grad_u_slave[_qp](_component)) * _test[_i][_qp];
    default:
      return 0;
  }
}
