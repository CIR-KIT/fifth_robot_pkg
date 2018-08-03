-- include "trajectory_builder_2d.lua"
include "trajectory_builder_3d.lua"

TRAJECTORY_BUILDER = {
   -- cartographer.mapping_2d.proto.LocalTrajectoryBuilderOptions trajectory_builder_2d_options
   -- trajectory_builder_2d = TRAJECTORY_BUILDER_2D,

   -- cartographer.mapping_3d.proto.LocalTrajectoryBuilderOptions trajectory_builder_3d_options
   -- Learn more at
   -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-3d-proto-localtrajectorybuilderoptions
   trajectory_builder_3d = TRAJECTORY_BUILDER_3D,

   -- bool pure_localization
   -- pure_localization = false
}
