//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "InterfacePostprocessor.h"

InputParameters
InterfacePostprocessor::validParams()
{
  InputParameters params = InterfaceUserObject::validParams();
  params += Postprocessor::validParams();
  params.addClassDescription(
      "Basic class for Interface Postprocessors. All InterfacePostprocessors "
      "should be derived from this class.");
  return params;
}

InterfacePostprocessor::InterfacePostprocessor(const InputParameters & parameters)
  : InterfaceUserObject(parameters), Postprocessor(parameters), _interface_master_area(0.)
{
}

void
InterfacePostprocessor::initialize()
{
  _interface_master_area = 0;
}

void
InterfacePostprocessor::execute()
{
  _interface_master_area += _current_side_volume;
}

void
InterfacePostprocessor::threadJoin(const UserObject & y)
{
  const InterfacePostprocessor & pps = static_cast<const InterfacePostprocessor &>(y);
  _interface_master_area += pps._interface_master_area;
}
