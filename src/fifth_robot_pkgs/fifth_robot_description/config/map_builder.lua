include "pose_graph.lua"

-- cartographer.mapping.proto.MapBuilderOptions
-- Learn more at
-- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-proto-mapbuilderoptions
MAP_BUILDER = {

   -- bool use_trajectory_builder_2d  
   -- use_trajectory_builder_2d = false,

   -- bool use_trajectory_builder_3d
   use_trajectory_builder_3d = false,

   -- int32 num_background_threads
   num_background_threads = 4,

   -- cartographer.mapping.proto.PoseGraphOptions pose_graph_options
   -- Learn more at
   -- https://google-cartographer.readthedocs.io/en/latest/configuration.html#cartographer-mapping-proto-posegraphoptions
   pose_graph = POSE_GRAPH,
}
