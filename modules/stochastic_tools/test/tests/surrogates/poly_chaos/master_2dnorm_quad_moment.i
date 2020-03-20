[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
  ny = 1
[]

[Variables]
  [u]
  []
[]

[Distributions]
  [D_dist]
    type = Normal
    mean = 5
    standard_deviation = 0.5
  []
  [S_dist]
    type = Normal
    mean = 8
    standard_deviation = 0.7
  []
[]

[Samplers]
  [quadrature]
    type = Quadrature
    distributions = 'D_dist S_dist'
    execute_on = INITIAL
    order = 5
  []
[]

[MultiApps]
  [quad_sub]
    type = SamplerFullSolveMultiApp
    input_files = sub.i
    sampler = quadrature
    mode = batch-restore
  []
[]

[Transfers]
  [quad]
    type = SamplerParameterTransfer
    multi_app = quad_sub
    sampler = quadrature
    parameters = 'Materials/diffusivity/prop_values Materials/xs/prop_values'
    to_control = 'stochastic'
  []
  [data]
    type = SamplerPostprocessorTransfer
    multi_app = quad_sub
    sampler = quadrature
    to_vector_postprocessor = storage
    from_postprocessor = avg
  []
[]

[VectorPostprocessors]
  [storage]
    type = StochasticResults
    parallel_type = REPLICATED
    samplers = quadrature
  []
  [pc_moments]
    type = PolynomialChaosStatistics
    pc_name = poly_chaos
    compute = 'mean stddev skewness kurtosis'
    execute_on = final
  []
[]

[Surrogates]
  [poly_chaos]
    type = PolynomialChaos
    execute_on = timestep_end
    order = 5
    distributions = 'D_dist S_dist'
    training_sampler = quadrature
    results_vpp = storage
    results_vector = quadrature
  []
[]

[Executioner]
  type = Steady
[]

[Problem]
  solve = false
  kernel_coverage_check = false
[]

[Outputs]
  [out]
    type = CSV
    execute_on = FINAL
  []
[]
