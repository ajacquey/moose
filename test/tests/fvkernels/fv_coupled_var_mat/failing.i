[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 2
[]

[Variables]
  [v]
    family = MONOMIAL
    order = CONSTANT
    fv = true
  []
  [w]
    family = MONOMIAL
    order = CONSTANT
    fv = true
  []
[]

[FVKernels]
  [diff]
    type = FVDiffusion
    variable = v
    coeff = coeff_w
  []
  [rxn]
    type = FVReaction
    variable = v
  []
  [diff_w]
    type = FVReaction
    variable = w
  []
[]

[FVBCs]
  [left]
    type = FVDirichletBC
    variable = v
    boundary = left
    value = 0
  []
  [right]
    type = FVDirichletBC
    variable = v
    boundary = right
    value = 1
  []
  [leftw]
    type = FVDirichletBC
    variable = w
    boundary = left
    value = 0.1
  []
  [rightw]
    type = FVDirichletBC
    variable = w
    boundary = right
    value = 1
  []
[]

[Materials]
  [diff_v]
    type = ADCoupledMaterial
    coupled_var = 'w'
    ad_mat_prop = 'coeff_w'
    regular_mat_prop = 'ref_coeff_w'
  []
[]

[Problem]
  kernel_coverage_check = off
[]

[Executioner]
  type = Steady
  solve_type = 'NEWTON'
[]

[Outputs]
  exodus = true
[]
