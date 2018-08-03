-- cartographer.mapping.proto.PoseGraphOptions
-- Learn more at
-- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-proto-posegraphoptions
SPARSE_POSE_GRAPH = {

   -- int32 optimize_every_n_nodes
   optimize_every_n_scans = 90,

   -- cartographer.mapping.pose_graph.proto.ConstraintBuilderOptions constraint_builder_options
   -- Learn more at
   -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-pose-graph-proto-constraintbuilderoptions
   constraint_builder = {
      
      -- double sampling_ratio
      sampling_ratio = 0.3,

      -- double max_constraint_distance
      max_constraint_distance = 15.,

      -- cartographer.sensor.proto.AdaptiveVoxelFilterOptions
      -- Learn more at
      -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-sensor-proto-adaptivevoxelfilteroptions
      adaptive_voxel_filter = {

         -- float max_length
         max_length = 0.9,

         -- float min_num_points
         min_num_points = 100,

         -- float max_range
         max_range = 50.,
      },

      -- double min_score
      min_score = 0.55,

      -- double global_localization_min_score
      global_localization_min_score = 0.6,

      -- double loop_closure_translation_weight
      loop_closure_translation_weight = 1.1e4,

      -- double loop_closure_rotation_weight
      loop_closure_rotation_weight = 1e5,

      -- bool log_matches
      log_matches = true,

      -- !!!!!!!!!!!!!!!!2d configures!!!!!!!!!!!!!!!!!!!!!!!!
      fast_correlative_scan_matcher = {
         linear_search_window = 7.,
         angular_search_window = math.rad(30.),
         branch_and_bound_depth = 7,
      },
      ceres_scan_matcher = {
         occupied_space_weight = 20.,
         translation_weight = 10.,
         rotation_weight = 1.,
         ceres_solver_options = {
            use_nonmonotonic_steps = true,
            max_num_iterations = 10,
            num_threads = 1,
         },
      },
      -- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      
      -- cartographer.mapping_3d.scan_matching.proto.FastCorrelativeScanMatcherOptions fast_correlative_scan_matcher_options_3d
      -- Learn more at
      -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-3d-scan-matching-proto-fastcorrelativescanmatcheroptions
      fast_correlative_scan_matcher_3d = {

         -- int32 branch_and_bound_depth
         branch_and_bound_depth = 8,

         -- int32 full_resolution_depth
         full_resolution_depth = 3,

         -- int32 rotational_histogram_size
         -- WHY DOES IT STAND HERE ?????????????????????????????????????????
         rotational_histogram_size = 120,

         -- double min_rotational_score
         min_rotational_score = 0.77,

         -- double min_low_resolution_score
         -- min_low_resolution_score = 0.0,
         
         -- double linear_xy_search_window
         linear_xy_search_window = 4.,

         -- double linear_z_search_window
         linear_z_search_window = 1.,

         -- double angular_search_window
         angular_search_window = math.rad(15.),
      },
      
      -- cartographer.mapping_3d.scan_matching.proto.CeresScanMatcherOptions ceres_scan_matcher_options_3d
      -- Learn more at
      -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-3d-scan-matching-proto-ceresscanmatcheroptions
      ceres_scan_matcher_3d = {

         -- double occupied_space_weight
         occupied_space_weight_0 = 20.,

         -- double translation_weight
         translation_weight = 10.,

         -- double rotation_weight
         rotation_weight = 1.,

         -- bool only_optimize_yaw
         only_optimize_yaw = false,

         -- cartographer.common.proto.CeresSolverOptions ceres_solver_options
         -- Learn more at
         -- http://ceres-solver.org/
         ceres_solver_options = {
            use_nonmonotonic_steps = false,
            max_num_iterations = 10,
            num_threads = 1,
         },
      },
   },

   -- double matcher_translation_weight
   matcher_translation_weight = 5e2,

   -- double matcher_rotation_weight
   matcher_rotation_weight = 1.6e3,

   -- cartographer.mapping.pose_graph.proto.OptimizationProblemOptions optimization_problem_options
   -- Learn more at
   -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-pose-graph-proto-optimizationproblemoptions
   optimization_problem = {

      -- double huber_scale
      huber_scale = 1e1,

      -- double acceleration_weight
      acceleration_weight = 1e3,

      -- double rotation_weight
      rotation_weight = 3e5,

      -- double local_slam_pose_translation_weight
      -- local_slam_pose_translation_weight = 0.0,
      
      -- double local_slam_pose_rotation_weight
      -- local_slam_pose_rotation_weight = 0.0,
      
      -- double odometry_translation_weight
      -- odometry_translation_weight = 0.0,

      -- double odometry_rotation_weight
      -- odometry_rotation_weight = 0.0,
      
      -- double fixed_frame_pose_translation_weight
      -- fixed_frame_pose_translation_weight = 0.0,
      
      -- double fixed_frame_pose_rotation_weight
      -- fixed_frame_pose_rotation_weight = 0.0,

      -- What are they ?????????????????????????????????????????????????????
      consecutive_scan_translation_penalty_factor = 1e5,
      consecutive_scan_rotation_penalty_factor = 1e5,

      -- bool log_solver_summary
      log_solver_summary = false,

      -- cartographer.common.proto.CeresSolverOptions ceres_solver_options
      -- Learn more at
      -- http://ceres-solver.org/
      ceres_solver_options = {
         use_nonmonotonic_steps = false,
         max_num_iterations = 50,
         num_threads = 7,
      },
   },

   -- int32 max_num_final_iterations
   max_num_final_iterations = 200,

   -- double global_sampling_ratio
   global_sampling_ratio = 0.003,

   -- bool log_residual_histograms
   -- log_residual_histograms = false,

   -- double global_constraint_search_after_n_seconds
   -- global_constraint_search_after_n_seconds = 0.0
}
