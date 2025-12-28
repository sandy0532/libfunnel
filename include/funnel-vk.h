#pragma once

#include "funnel.h"
#include <vulkan/vulkan.h>

/**
 * Set up a stream for Vulkan integration.
 *
 * Required extensions:
 *
 * - VK_KHR_get_physical_device_properties2
 * - VK_KHR_external_memory
 * - VK_KHR_external_memory_fd
 * - VK_EXT_external_memory_dma_buf
 *
 * @param stream Stream
 * @param instance VkInstance to use for the stream (must outlive stream)
 * @param physical_device VkPhysicalDevice to use for the stream (must outlive
 * stream)
 * @param device VkDevice to use for the stream (must outlive stream)
 */
int funnel_stream_init_vulkan(struct funnel_stream *stream, VkInstance instance,
                              VkPhysicalDevice physical_device,
                              VkDevice device);

/**
 * Set the required buffer usage. This will control the usage for
 * images allocated by libfunnel.
 *
 * funnel_stream_vk_add_format() will fail if the requested usages
 * are not available. In this case, you may reconfigure the usage
 * and try again.
 *
 * @param stream Stream
 * @param usage Required VkImageUsageFlagBits.
 */
int funnel_stream_vk_set_usage(struct funnel_stream *stream,
                               VkImageUsageFlagBits usage);

/**
 * Add a supported Vulkan format. Must be called in preference order (highest to
 * lowest). Only some formats are supported by libfunnel:
 *
 * - VK_FORMAT_R8G8B8A8_SRGB
 * - VK_FORMAT_R8G8B8A8_UNORM
 * - VK_FORMAT_B8G8R8A8_SRGB
 * - VK_FORMAT_B8G8R8A8_UNORM
 *
 * The corresponding UNORM variants are also acceptable, and equivalent.
 * `funnel_buffer_get_vk_format` will always return the SRGB formats. If
 * you need UNORM (because you are doing sRGB/gamma conversion in your shader),
 * you can use UNORM constants when you create a VkImageView.
 *
 * @param stream Stream
 * @param format VkFormat
 * @param alpha Whether alpha is meaningful or ignored
 * @param features Required VkFormatFeatureFlagBits. Adding a format will fail
 * if the requested features are not available.
 * @param usage Required VkImageUsageFlagBits. Adding a format will fail
 * if the requested usages are not available.
 */
int funnel_stream_vk_add_format(struct funnel_stream *stream, VkFormat format,
                                bool alpha, VkFormatFeatureFlagBits features);

/**
 * Get the VkImage for a Funnel buffer.
 *
 * The VkImage is only valid while `buf` is dequeued, or before the destroy
 * callback is used (if you use buffer callbacks).
 *
 * @param buf Buffer
 * @param bo Output EGLImage for the buffer (borrowed)
 */
int funnel_buffer_get_vk_image(struct funnel_buffer *buf, VkImage *image);

/**
 * Get the VkFormat for a Funnel buffer.
 *
 * @param buf Buffer
 * @param format Output EGL format
 * @param has_alpha Output boolean indicating whether alpha is enabled
 */
int funnel_buffer_get_vk_format(struct funnel_buffer *buf, VkFormat *format,
                                bool *has_alpha);
