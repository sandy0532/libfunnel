project('funnel-test', 'c')

includes = include_directories('include')

pipewire = dependency('libpipewire-0.3')
gbm = dependency('gbm')
egl = dependency('egl')
vulkan = dependency('vulkan')

lib_funnel = library('funnel', 'src/funnel.c',
    dependencies: [gbm, egl, pipewire],
    include_directories : includes)
funnel = declare_dependency(link_with : lib_funnel, include_directories : includes)

lib_funnel_egl = library('funnel-egl', 'src/egl.c', dependencies: [funnel, pipewire, egl, gbm])
lib_funnel_vk = library('funnel-vk', 'src/vulkan.c', dependencies: [funnel, pipewire, vulkan, gbm])

funnel_egl = declare_dependency(link_with : lib_funnel_egl, include_directories : includes)
funnel_vk = declare_dependency(link_with : lib_funnel_vk, include_directories : includes)

gl = dependency('GL')
x11 = dependency('x11')
executable('funnel-test-egl', 'test-egl.c', dependencies: [gl, egl, x11, funnel, funnel_egl])

wayland = dependency('wayland-client')
wl_mod = import('unstable-wayland')

xml = wl_mod.find_protocol('xdg-shell')
xdg_shell = wl_mod.scan_xml(xml)

glsl_compiler = find_program('glslang', 'glslangValidator')
glsl_args = [
  '--quiet',
  '--target-env', 'vulkan1.0',
  '--vn', '@BASENAME@',
  '--depfile', '@DEPFILE@',
  '@INPUT@',
  '-o', '@OUTPUT@',
]
glsl_generator = generator(
  glsl_compiler,
  output    : [ '@BASENAME@.h' ],
  depfile   : '@BASENAME@.h.d',
  arguments : glsl_args,
)

demo_shaders = files([
  'shaders/triangle_frag.frag',
  'shaders/triangle_vert.vert',
])

executable('funnel-test-vk', 'test-vk.c', xdg_shell,
    glsl_generator.process(demo_shaders),
    dependencies: [wayland, funnel, funnel_vk, vulkan])
