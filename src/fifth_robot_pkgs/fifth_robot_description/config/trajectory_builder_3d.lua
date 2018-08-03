-- cartographer.mapping_3d.proto.LocalTrajectoryBuilderOptions
-- Learn more at  
-- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-3d-proto-localtrajectorybuilderoptions
TRAJECTORY_BUILDER_3D = {

   -- float min_range      
   min_range = 1.,

   -- float max_range
   max_range = 60,

   -- ?????????????????????????????????????????????????????????
   -- MAYBE... int32 num_accumulated_range_data
   scans_per_accumulation = 1,

   -- float voxel_filter_size
   voxel_filter_size = 0.15,

   -- cartographer.sensor.proto.AdaptiveVoxelFilterOptions high_resolution_adaptive_voxel_filter_options
   -- Learn more at
   -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-sensor-proto-adaptivevoxelfilteroptions
   high_resolution_adaptive_voxel_filter = {

      -- float max_length
      max_length = 2.,

      -- float min_num_points
      min_num_points = 150,

      -- float max_range
      max_range = 15.,
   },

   -- cartographer.sensor.proto.AdaptiveVoxelFilterOptions high_resolution_adaptive_voxel_filter_options
   -- Same with high_resolution_adaptive_voxel_filter_options
   low_resolution_adaptive_voxel_filter = {

      -- float max_length      
      max_length = 4.,
      
      -- float min_num_points
      min_num_points = 200,

      -- float max_range
      max_range = 60,
   },

   -- bool use_online_correlative_scan_matching
   -- use_online_correlative_scan_matching = false


   -- cartographer.mapping_2d.scan_matching.proto.RealTimeCorrelativeScanMatcherOptions real_time_correlative_scan_matcher_options
   -- NOT EXIST
   
   -- cartographer.mapping_3d.scan_matching.proto.CeresScanMatcherOptions ceres_scan_matcher_options
   -- Learn more at
   -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-3d-scan-matching-proto-ceresscanmatcheroptions
   ceres_scan_matcher = {

      -- double occupied_space_weight
      occupied_space_weight_0 = 5.,

      -- double occupied_space_weight
      occupied_space_weight_1 = 30.,

      -- double translation_weight
      translation_weight = 0.3,

      -- double rotation_weight
      rotation_weight = 2e3,

      -- bool only_optimize_yaw
      only_optimize_yaw = false,

      -- cartographer.common.proto.CeresSolverOptions ceres_solver_options
      -- Learn more at
      -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-common-proto-ceressolveroptions
      ceres_solver_options = {

         -- bool use_nonmonotonic_steps
         use_nonmonotonic_steps = false,

         -- int32 max_num_iterations
         max_num_iterations = 12,

         -- int32 num_threads
         num_threads = 1,
      },
   },

   -- cartographer.mapping.proto.MotionFilterOptions motion_filter_options
   -- Learn more at
   -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-proto-motionfilteroptions
   motion_filter = {

      -- double max_time_seconds
      max_time_seconds = 0.5,

      -- double max_distance_meters
      max_distance_meters = 0.1,

      -- double max_angle_radians
      max_angle_radians = 0.004,
   },

   -- double imu_gravity_time_constant
   -- imu_gravity_time_constant = 0.0,

   -- int32 rotational_histogram_size
   -- rotational_histogram_size = 0,

   -- cartographer.mapping_3d.proto.SubmapsOptions
   -- Learn more at
   -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-3d-proto-submapsoptions
   submaps = {

      -- double high_resolution
      high_resolution = 0.10,

      -- double high_resolution_max_range
      high_resolution_max_range = 20.,

      -- double low_resolution
      low_resolution = 0.45,

      -- int32 num_range_data
      num_range_data = 160,

      -- cartographer.mapping_3d.proto.RangeDataInserterOptions range_data_inserter_options
      -- Learn more at
      -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-3d-proto-rangedatainserteroptions
      range_data_inserter = {

         -- double hit_probability
         hit_probability = 0.55,

         -- double miss_probability
         miss_probability = 0.49,

         -- int32 num_free_space_voxels
         num_free_space_voxels = 2,
      },
   },

   use = "KALMAN",  -- or "OPTIMIZING".
   kalman_local_trajectory_builder = {
      pose_tracker = {
         orientation_model_variance = 5e-3,
         position_model_variance = 0.00654766,
         velocity_model_variance = 0.53926,
         -- This disables gravity alignment in local SLAM.
         imu_gravity_time_constant = 1e9,
         imu_gravity_variance = 0,
         num_odometry_states = 1,
      },

      use_online_correlative_scan_matching = false,
      real_time_correlative_scan_matcher = {
         linear_search_window = 0.15,
         angular_search_window = math.rad(1.),
         translation_delta_cost_weight = 1e-1,
         rotation_delta_cost_weight = 1e-1,
      },

      scan_matcher_variance = 2.34e-9,
      odometer_translational_variance = 1e-7,
      odometer_rotational_variance = 1e-7,
   },

   optimizing_local_trajectory_builder = {
      high_resolution_grid_weight = 5.,
      low_resolution_grid_weight = 15.,
      velocity_weight = 4e1,
      translation_weight = 1e2,
      rotation_weight = 1e3,
      odometry_translation_weight = 1e4,
      odometry_rotation_weight = 1e4,
   },
}
