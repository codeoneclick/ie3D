indie Engine 3D
=======

Crossplatform 3d game engine based on OGL ES 2.0, 3.0.

ie3D-Core - is a core functionality layer. It includes low level rendering system, material system, resource system, etc.

ie3D-Engine - is a second layer of engine abstraction. It includes extensions for basic core layer, such as complex models, custom models, etc.

ie3D-EMap - map/level editor. It also provides edit mode for game objects and particle emitters. 

ie3D-Demo - basic demo, playground for engine feature on iOS and OSX. 

ie3D-Bundle - just folder with resources (textures, meshes, shaders etc).


Features: 

- supports OpenGL ES 2.0, 3.0 and OpenGL core_profile 3.2 APIs.
- supports only dynamic render pipeline which presenting with GLSL shaders.
- includes basic game objects such as “Static Models”, “Animated Models”, “Landscape”, “Ocean”, “Skybox”, “AtmosphericScattering”, “Particles”, etc.
- static and animated objects have own format “.mdl” and can be exported with custom 3D Max plugin “ie3D-Exporter”. Objects animation are presented by skeleton with some set of sequences.
- chunked LODs, based on quad tree is used for landscape rendering. Also landscape has “sew-seams” technology that helps to resolve problem with edge artefacts. Landscape texturing is presented with two different technologies - “Texture Splatting” and “Triplanar mapping”.
- ocean is presented with simple quad, ripples texture, reflection texture, refraction texture and height map texture. Reflection and refraction textures is dynamic and retrieving from game scene on demand (usual once per frame).
- skybox is presented with box with CUBE texture.
- atmospheric scattering is presented with sphere, which has complex shader to interpolate sky color, based on global light direction.
- all game objects are configurable. Game Engine is based on “data driven” design. All configurations are based on “xml’s”. Serialisers and deserialisers for configurations are generated on precompile phase with “python” script. This allows to add new configuration types with minimal changes in source code.

MODEL XML EXAMPLE:

<model
    mesh_filename="model_03.MDL"
    is_batching="0">
    <materials>
	<material filename="material.model.base.xml"/>
        <material filename="material.model.reflection.xml"/>
        <material filename="material.model.refraction.xml"/>
        <material filename="material.model.shadowmap.xml"/>
	</materials>
    <animations>
        <animation name=“IDLE” filename="model_00.MDL_anim"/>
        <animation name=“RUN” filename="model_01.MDL_anim"/>
    </animations>
</model>

MATERIAL XML EXAMPLE:

<material
    render_operation_name="ws.base"
    is_cull_face="0"
    is_depth_test="1"
    is_depth_mask="1"
    is_blending="0"
    cull_face_mode="GL_FRONT"
    blending_function_source="GL_SRC_ALPHA"
    blending_function_destination="GL_ONE_MINUS_SRC_ALPHA"
    is_cliping="0"
    clipping_x="0.0"
    clipping_y="0.0"
    clipping_z="0.0"
    clipping_w="0.0"
    is_reflecting="0"
    is_shadowing="0"
    is_debugging="0">
    <shader
        vs_filename="shaderModel.vert"
        fs_filename="shaderModel.frag"/>
    <textures>
        <texture
            filename="animated_model_03.png"
            render_operation_name=""
            sampler_index="0"
            wrap_mode="GL_REPEAT"
            mag_filter="GL_LINEAR"
            min_filter="GL_MIPMAP"/>
    </textures>
</material>

- all render techniques such as “shadow mapping”, “reflection”, “refraction”, “SSAO” also are presented with “xml’s” configurations. 

<ws_technique
    guid="ws.shadowmap"
    are_draw_bounding_boxes="0"
    index="2"
    screen_width="512"
    screen_height="512"
    clear_color_r="0.0"
    clear_color_g="0.0"
    clear_color_b="0.0"
    clear_color_a="0.0"/>


Screenshots:

![image 1](https://github.com/codeoneclick/indieEngine3D/blob/develop/ie3D-Doc/Screenshots/ScreenShot04.png)

![image 2](https://github.com/codeoneclick/indieEngine3D/blob/develop/ie3D-Doc/Screenshots/ScreenShot05.png)

![image 3](https://github.com/codeoneclick/indieEngine3D/blob/develop/ie3D-Doc/Screenshots/ScreenShot01.png)

![image 4](https://github.com/codeoneclick/indieEngine3D/blob/develop/ie3D-Doc/Screenshots/ScreenShot02.png)

![image 5](https://github.com/codeoneclick/indieEngine3D/blob/develop/ie3D-Doc/Screenshots/ScreenShot03.png)
