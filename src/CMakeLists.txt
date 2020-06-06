set(BVH_HDRS
    bvh/BvhTranslator.h
    bvh/Bvh.h
    bvh/SplitBvh.h
)
set(BVH_SRCS
    bvh/BvhTranslator.cpp
    bvh/Bvh.cpp
    bvh/SplitBvh.cpp
)

set(CORE_HDRS
    core/Light.h
    core/Camera.h
    core/Material.h
    core/Mesh.h
    core/Program.h
    core/Quad.h
    core/Renderer.h
    core/Scene.h
    core/Shader.h
    core/ShaderIncludes.h
    core/Texture.h
    core/TiledRenderer.h
)
set(CORE_SRCS
    core/Light.cpp
    core/Camera.cpp
    core/Mesh.cpp
    core/Program.cpp
    core/Quad.cpp
    core/Renderer.cpp
    core/Scene.cpp
    core/Shader.cpp
    core/Texture.cpp
    core/TiledRenderer.cpp
)

set(PARSER_HDRS
    parser/stb_image_resize.h
    parser/stb_image_write.h
    parser/stb_image.h
    parser/SceneLoader.h
    parser/HDRLoader.h
    parser/tiny_obj_loader.h
    parser/tiny_gltf.h
    parser/json.hpp
    parser/GLBLoader.h
)
set(PARSER_SRCS
    parser/SceneLoader.cpp
    parser/HDRLoader.cpp
    parser/GLBLoader.cpp
)

set(MATH_HDRS
    math/Bounds3D.h
    math/Math.h
    math/Axis.h
    math/Vector2.h
    math/Vector3.h
    math/Vector4.h
    math/Matrix4x4.h
    math/Quat.h
)
set(MATH_SRCS
    math/Math.cpp
)

set(TEST_HDRS
    test/BoyTestScene.h
    test/CornellTestScene.h
)
set(TEST_SRCS

)

set(JOB_HDRS
    job/Runnable.h
    job/RunnableThread.h
    job/TaskThread.h
    job/TaskThreadPool.h
    job/ThreadEvent.h
    job/ThreadManager.h
    job/ThreadTask.h
)
set(JOB_SRCS
    job/RunnableThread.cpp
    job/TaskThread.cpp
    job/TaskThreadPool.cpp
    job/ThreadEvent.cpp
    job/ThreadManager.cpp
)

set(FILE_HDRS
    file/tinydir.h
)
set(FILE_SRCS
    
)

add_library(Core STATIC
	${BVH_HDRS}
	${BVH_SRCS}

	${CORE_HDRS}
    ${CORE_SRCS}
    
    ${PARSER_SRCS}
    ${PARSER_HDRS}

    ${MATH_HDRS}
    ${MATH_SRCS}

    ${TEST_HDRS}
    ${TEST_SRCS}

    ${JOB_HDRS}
    ${JOB_SRCS}

    ${FILE_HDRS}
    ${FILE_SRCS}
)

source_group(src\\file FILES ${FILE_HDRS} ${FILE_SRCS})
source_group(src\\job FILES ${JOB_HDRS} ${JOB_SRCS})
source_group(src\\test FILES ${TEST_HDRS} ${TEST_SRCS})
source_group(src\\math FILES ${MATH_SRCS} ${MATH_HDRS})
source_group(src\\parser FILES ${PARSER_SRCS} ${PARSER_HDRS})
source_group(src\\bvh FILES ${BVH_HDRS} ${BVH_SRCS})
source_group(src\\core FILES ${CORE_HDRS} ${CORE_SRCS})