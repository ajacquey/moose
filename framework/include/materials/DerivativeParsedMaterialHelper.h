//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "DerivativeFunctionMaterialBase.h"
#include "ParsedMaterialHelper.h"
#include "libmesh/fparser_ad.hh"

// Forward Declarations
class DerivativeParsedMaterialHelper;

template <>
InputParameters validParams<DerivativeParsedMaterialHelper>();

/**
 * Helper class to perform the auto derivative taking.
 */
class DerivativeParsedMaterialHelper : public ParsedMaterialHelper
{
public:
  DerivativeParsedMaterialHelper(const InputParameters & parameters,
                                 VariableNameMappingMode map_mode = USE_PARAM_NAMES);

  static InputParameters validParams();

protected:
  struct Derivative;
  struct MaterialPropertyDerivativeRule;

  virtual void initQpStatefulProperties();
  virtual void computeQpProperties();

  virtual void functionsPostParse();
  void assembleDerivatives();

  void
  recurseMatProps(unsigned int var, unsigned int order, const MatPropDescriptorList & parent_mpd);

  void
  recurseDerivative(unsigned int var, unsigned int order, const Derivative & parent_derivative);

  /// maximum derivative order
  unsigned int _derivative_order;

  /// The requested derivatives of the free energy (sorted by order)
  std::vector<Derivative> _derivatives;

  /// variable base name for the dynamically material property derivatives
  const std::string _dmatvar_base;

  /// next available variable number for automatically created material property derivative variables
  unsigned int _dmatvar_index;

private:
  // for bulk registration of material property derivatives
  std::vector<MaterialPropertyDerivativeRule> _bulk_rules;
};

struct DerivativeParsedMaterialHelper::Derivative
{
  MaterialProperty<Real> * _mat_prop;
  ADFunctionPtr _F;
  std::vector<VariableName> _darg_names;
};

struct DerivativeParsedMaterialHelper::MaterialPropertyDerivativeRule
{
  MaterialPropertyDerivativeRule(std::string p, std::string v, std::string c)
    : _parent(p), _var(v), _child(c)
  {
  }

  std::string _parent;
  std::string _var;
  std::string _child;
};
