/*
 *
 * Copyright (C) 2015 Valve Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Author: Cody Northrop <cody@lunarg.com>
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Tony Barbour <tony@LunarG.com>
 */

#include <string.h>
#include "wsi_swapchain.h"

static inline void* globalGetProcAddr(const char *name)
{
    if (!name || name[0] != 'v' || name[1] != 'k')
        return NULL;

    name += 2;
    if (!strcmp(name, "CreateInstance"))
        return (void*) vkCreateInstance;
    if (!strcmp(name, "DestroyInstance"))
        return (void*) vkDestroyInstance;
    if (!strcmp(name, "EnumeratePhysicalDevices"))
        return (void*) vkEnumeratePhysicalDevices;
    if (!strcmp(name, "GetPhysicalDeviceFeatures"))
        return (void*) vkGetPhysicalDeviceFeatures;
    if (!strcmp(name, "GetPhysicalDeviceFormatProperties"))
        return (void*) vkGetPhysicalDeviceFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceImageFormatProperties"))
        return (void*) vkGetPhysicalDeviceImageFormatProperties;
    if (!strcmp(name, "GetPhysicalDeviceProperties"))
        return (void*) vkGetPhysicalDeviceProperties;
    if (!strcmp(name, "GetPhysicalDeviceQueueFamilyProperties"))
        return (void*) vkGetPhysicalDeviceQueueFamilyProperties;
    if (!strcmp(name, "GetPhysicalDeviceMemoryProperties"))
        return (void*) vkGetPhysicalDeviceMemoryProperties;
    if (!strcmp(name, "GetInstanceProcAddr"))
        return (void*) vkGetInstanceProcAddr;
    if (!strcmp(name, "GetDeviceProcAddr"))
        return (void*) vkGetDeviceProcAddr;
    if (!strcmp(name, "CreateDevice"))
        return (void*) vkCreateDevice;
    if (!strcmp(name, "DestroyDevice"))
        return (void*) vkDestroyDevice;
    if (!strcmp(name, "EnumerateInstanceExtensionProperties"))
        return (void*) vkEnumerateInstanceExtensionProperties;
    if (!strcmp(name, "EnumerateDeviceExtensionProperties"))
        return (void*) vkEnumerateDeviceExtensionProperties;
    if (!strcmp(name, "EnumerateInstanceLayerProperties"))
        return (void*) vkEnumerateInstanceLayerProperties;
    if (!strcmp(name, "EnumerateDeviceLayerProperties"))
        return (void*) vkEnumerateDeviceLayerProperties;
    if (!strcmp(name, "GetDeviceQueue"))
        return (void*) vkGetDeviceQueue;
    if (!strcmp(name, "QueueSubmit"))
        return (void*) vkQueueSubmit;
    if (!strcmp(name, "QueueWaitIdle"))
        return (void*) vkQueueWaitIdle;
    if (!strcmp(name, "DeviceWaitIdle"))
        return (void*) vkDeviceWaitIdle;
    if (!strcmp(name, "AllocateMemory"))
        return (void*) vkAllocateMemory;
    if (!strcmp(name, "FreeMemory"))
        return (void*) vkFreeMemory;
    if (!strcmp(name, "MapMemory"))
        return (void*) vkMapMemory;
    if (!strcmp(name, "UnmapMemory"))
        return (void*) vkUnmapMemory;
    if (!strcmp(name, "FlushMappedMemoryRanges"))
        return (void*) vkFlushMappedMemoryRanges;
    if (!strcmp(name, "InvalidateMappedMemoryRanges"))
        return (void*) vkInvalidateMappedMemoryRanges;
    if (!strcmp(name, "GetDeviceMemoryCommitment"))
        return (void *) vkGetDeviceMemoryCommitment;
    if (!strcmp(name, "BindBufferMemory"))
        return (void*) vkBindBufferMemory;
    if (!strcmp(name, "BindImageMemory"))
        return (void*) vkBindImageMemory;
    if (!strcmp(name, "GetBufferMemoryRequirements"))
        return (void*) vkGetBufferMemoryRequirements;
    if (!strcmp(name, "GetImageMemoryRequirements"))
        return (void*) vkGetImageMemoryRequirements;
    if (!strcmp(name, "GetImageSparseMemoryRequirements"))
        return (void*) vkGetImageSparseMemoryRequirements;
    if (!strcmp(name, "GetPhysicalDeviceSparseImageFormatProperties"))
        return (void*) vkGetPhysicalDeviceSparseImageFormatProperties;
    if (!strcmp(name, "QueueBindSparse"))
        return (void*) vkQueueBindSparse;
    if (!strcmp(name, "CreateFence"))
        return (void*) vkCreateFence;
    if (!strcmp(name, "DestroyFence"))
        return (void*) vkDestroyFence;
    if (!strcmp(name, "ResetFences"))
        return (void*) vkResetFences;
    if (!strcmp(name, "GetFenceStatus"))
        return (void*) vkGetFenceStatus;
    if (!strcmp(name, "WaitForFences"))
        return (void*) vkWaitForFences;
    if (!strcmp(name, "CreateSemaphore"))
        return (void*) vkCreateSemaphore;
    if (!strcmp(name, "DestroySemaphore"))
        return (void*) vkDestroySemaphore;
    if (!strcmp(name, "CreateEvent"))
        return (void*) vkCreateEvent;
    if (!strcmp(name, "DestroyEvent"))
        return (void*) vkDestroyEvent;
    if (!strcmp(name, "GetEventStatus"))
        return (void*) vkGetEventStatus;
    if (!strcmp(name, "SetEvent"))
        return (void*) vkSetEvent;
    if (!strcmp(name, "ResetEvent"))
        return (void*) vkResetEvent;
    if (!strcmp(name, "CreateQueryPool"))
        return (void*) vkCreateQueryPool;
    if (!strcmp(name, "DestroyQueryPool"))
        return (void*) vkDestroyQueryPool;
    if (!strcmp(name, "GetQueryPoolResults"))
        return (void*) vkGetQueryPoolResults;
    if (!strcmp(name, "CreateBuffer"))
        return (void*) vkCreateBuffer;
    if (!strcmp(name, "DestroyBuffer"))
        return (void*) vkDestroyBuffer;
    if (!strcmp(name, "CreateBufferView"))
        return (void*) vkCreateBufferView;
    if (!strcmp(name, "DestroyBufferView"))
        return (void*) vkDestroyBufferView;
    if (!strcmp(name, "CreateImage"))
        return (void*) vkCreateImage;
    if (!strcmp(name, "DestroyImage"))
        return (void*) vkDestroyImage;
    if (!strcmp(name, "GetImageSubresourceLayout"))
        return (void*) vkGetImageSubresourceLayout;
    if (!strcmp(name, "CreateImageView"))
        return (void*) vkCreateImageView;
    if (!strcmp(name, "DestroyImageView"))
        return (void*) vkDestroyImageView;
    if (!strcmp(name, "CreateShaderModule"))
        return (void*) vkCreateShaderModule;
    if (!strcmp(name, "DestroyShaderModule"))
        return (void*) vkDestroyShaderModule;
    if (!strcmp(name, "CreatePipelineCache"))
        return (void*) vkCreatePipelineCache;
    if (!strcmp(name, "DestroyPipelineCache"))
        return (void*) vkDestroyPipelineCache;
    if (!strcmp(name, "GetPipelineCacheData"))
        return (void*) vkGetPipelineCacheData;
    if (!strcmp(name, "MergePipelineCaches"))
        return (void*) vkMergePipelineCaches;
    if (!strcmp(name, "CreateGraphicsPipelines"))
        return (void*) vkCreateGraphicsPipelines;
    if (!strcmp(name, "CreateComputePipelines"))
        return (void*) vkCreateComputePipelines;
    if (!strcmp(name, "DestroyPipeline"))
        return (void*) vkDestroyPipeline;
    if (!strcmp(name, "CreatePipelineLayout"))
        return (void*) vkCreatePipelineLayout;
    if (!strcmp(name, "DestroyPipelineLayout"))
        return (void*) vkDestroyPipelineLayout;
    if (!strcmp(name, "CreateSampler"))
        return (void*) vkCreateSampler;
    if (!strcmp(name, "DestroySampler"))
        return (void*) vkDestroySampler;
    if (!strcmp(name, "CreateDescriptorSetLayout"))
        return (void*) vkCreateDescriptorSetLayout;
    if (!strcmp(name, "DestroyDescriptorSetLayout"))
        return (void*) vkDestroyDescriptorSetLayout;
    if (!strcmp(name, "CreateDescriptorPool"))
        return (void*) vkCreateDescriptorPool;
    if (!strcmp(name, "DestroyDescriptorPool"))
        return (void*) vkDestroyDescriptorPool;
    if (!strcmp(name, "ResetDescriptorPool"))
        return (void*) vkResetDescriptorPool;
    if (!strcmp(name, "AllocateDescriptorSets"))
        return (void*) vkAllocateDescriptorSets;
    if (!strcmp(name, "FreeDescriptorSets"))
        return (void*) vkFreeDescriptorSets;
    if (!strcmp(name, "UpdateDescriptorSets"))
        return (void*) vkUpdateDescriptorSets;
    if (!strcmp(name, "CreateFramebuffer"))
        return (void*) vkCreateFramebuffer;
    if (!strcmp(name, "DestroyFramebuffer"))
        return (void*) vkDestroyFramebuffer;
    if (!strcmp(name, "CreateRenderPass"))
        return (void*) vkCreateRenderPass;
    if (!strcmp(name, "DestroyRenderPass"))
        return (void*) vkDestroyRenderPass;
    if (!strcmp(name, "GetRenderAreaGranularity"))
        return (void*) vkGetRenderAreaGranularity;
    if (!strcmp(name, "CreateCommandPool"))
        return (void*) vkCreateCommandPool;
    if (!strcmp(name, "DestroyCommandPool"))
        return (void*) vkDestroyCommandPool;
    if (!strcmp(name, "ResetCommandPool"))
        return (void*) vkResetCommandPool;
    if (!strcmp(name, "AllocateCommandBuffers"))
        return (void*) vkAllocateCommandBuffers;
    if (!strcmp(name, "FreeCommandBuffers"))
        return (void*) vkFreeCommandBuffers;
    if (!strcmp(name, "BeginCommandBuffer"))
        return (void*) vkBeginCommandBuffer;
    if (!strcmp(name, "EndCommandBuffer"))
        return (void*) vkEndCommandBuffer;
    if (!strcmp(name, "ResetCommandBuffer"))
        return (void*) vkResetCommandBuffer;
    if (!strcmp(name, "CmdBindPipeline"))
        return (void*) vkCmdBindPipeline;
    if (!strcmp(name, "CmdSetViewport"))
        return (void*) vkCmdSetViewport;
    if (!strcmp(name, "CmdSetScissor"))
        return (void*) vkCmdSetScissor;
    if (!strcmp(name, "CmdSetLineWidth"))
        return (void*) vkCmdSetLineWidth;
    if (!strcmp(name, "CmdSetDepthBias"))
        return (void*) vkCmdSetDepthBias;
    if (!strcmp(name, "CmdSetBlendConstants"))
        return (void*) vkCmdSetBlendConstants;
    if (!strcmp(name, "CmdSetDepthBounds"))
        return (void*) vkCmdSetDepthBounds;
    if (!strcmp(name, "CmdSetStencilCompareMask"))
        return (void*) vkCmdSetStencilCompareMask;
    if (!strcmp(name, "CmdSetStencilWriteMask"))
        return (void*) vkCmdSetStencilWriteMask;
    if (!strcmp(name, "CmdSetStencilReference"))
        return (void*) vkCmdSetStencilReference;
    if (!strcmp(name, "CmdBindIndexBuffer"))
        return (void*) vkCmdBindIndexBuffer;
    if (!strcmp(name, "CmdBindVertexBuffers"))
        return (void*) vkCmdBindVertexBuffers;
    if (!strcmp(name, "CmdDraw"))
        return (void*) vkCmdDraw;
    if (!strcmp(name, "CmdDrawIndexed"))
        return (void*) vkCmdDrawIndexed;
    if (!strcmp(name, "CmdDrawIndirect"))
        return (void*) vkCmdDrawIndirect;
    if (!strcmp(name, "CmdDrawIndexedIndirect"))
        return (void*) vkCmdDrawIndexedIndirect;
    if (!strcmp(name, "CmdDispatch"))
        return (void*) vkCmdDispatch;
    if (!strcmp(name, "CmdDispatchIndirect"))
        return (void*) vkCmdDispatchIndirect;
    if (!strcmp(name, "CmdCopyBuffer"))
        return (void*) vkCmdCopyBuffer;
    if (!strcmp(name, "CmdCopyImage"))
        return (void*) vkCmdCopyImage;
    if (!strcmp(name, "CmdBlitImage"))
        return (void*) vkCmdBlitImage;
    if (!strcmp(name, "CmdCopyBufferToImage"))
        return (void*) vkCmdCopyBufferToImage;
    if (!strcmp(name, "CmdCopyImageToBuffer"))
        return (void*) vkCmdCopyImageToBuffer;
    if (!strcmp(name, "CmdUpdateBuffer"))
        return (void*) vkCmdUpdateBuffer;
    if (!strcmp(name, "CmdFillBuffer"))
        return (void*) vkCmdFillBuffer;
    if (!strcmp(name, "CmdClearColorImage"))
        return (void*) vkCmdClearColorImage;
    if (!strcmp(name, "CmdClearDepthStencilImage"))
        return (void*) vkCmdClearDepthStencilImage;
    if (!strcmp(name, "CmdClearAttachments"))
        return (void*) vkCmdClearAttachments;
    if (!strcmp(name, "CmdResolveImage"))
        return (void*) vkCmdResolveImage;
    if (!strcmp(name, "CmdSetEvent"))
        return (void*) vkCmdSetEvent;
    if (!strcmp(name, "CmdResetEvent"))
        return (void*) vkCmdResetEvent;
    if (!strcmp(name, "CmdWaitEvents"))
        return (void*) vkCmdWaitEvents;
    if (!strcmp(name, "CmdPipelineBarrier"))
        return (void*) vkCmdPipelineBarrier;
    if (!strcmp(name, "CmdBeginQuery"))
        return (void*) vkCmdBeginQuery;
    if (!strcmp(name, "CmdEndQuery"))
        return (void*) vkCmdEndQuery;
    if (!strcmp(name, "CmdResetQueryPool"))
        return (void*) vkCmdResetQueryPool;
    if (!strcmp(name, "CmdWriteTimestamp"))
        return (void*) vkCmdWriteTimestamp;
    if (!strcmp(name, "CmdCopyQueryPoolResults"))
        return (void*) vkCmdCopyQueryPoolResults;
    if (!strcmp(name, "CmdPushConstants"))
        return (void*) vkCmdPushConstants;
    if (!strcmp(name, "CmdBeginRenderPass"))
        return (void*) vkCmdBeginRenderPass;
    if (!strcmp(name, "CmdNextSubpass"))
        return (void*) vkCmdNextSubpass;
    if (!strcmp(name, "CmdEndRenderPass"))
        return (void*) vkCmdEndRenderPass;
    if (!strcmp(name, "CmdExecuteCommands"))
        return (void*) vkCmdExecuteCommands;
    return NULL;
}

/* These functions require special handling by the loader.
*  They are not just generic trampoline code entrypoints.
*  Thus GPA must return loader entrypoint for these instead of first function
*  in the chain. */
static inline void *loader_non_passthrough_gipa(const char *name)
{
    if (!name || name[0] != 'v' || name[1] != 'k')
        return NULL;

    name += 2;
    if (!strcmp(name, "CreateInstance"))
        return (void*) vkCreateInstance;
    if (!strcmp(name, "DestroyInstance"))
        return (void*) vkDestroyInstance;
    if (!strcmp(name, "GetDeviceProcAddr"))
        return (void*) vkGetDeviceProcAddr;
     // remove once no longer locks
    if (!strcmp(name, "EnumeratePhysicalDevices"))
        return (void*) vkEnumeratePhysicalDevices;
    if (!strcmp(name, "EnumerateDeviceExtensionProperties"))
        return (void*) vkEnumerateDeviceExtensionProperties;
    if (!strcmp(name, "EnumerateDeviceLayerProperties"))
        return (void*) vkEnumerateDeviceLayerProperties;
    if (!strcmp(name, "GetInstanceProcAddr"))
        return (void*) vkGetInstanceProcAddr;
    if (!strcmp(name, "CreateDevice"))
        return (void*) vkCreateDevice;

    return NULL;
}

static inline void *loader_non_passthrough_gdpa(const char *name)
{
    if (!name || name[0] != 'v' || name[1] != 'k')
        return NULL;

    name += 2;

    if (!strcmp(name, "GetDeviceProcAddr"))
        return (void*) vkGetDeviceProcAddr;
    if (!strcmp(name, "CreateDevice"))
        return (void*) vkCreateDevice;
    if (!strcmp(name, "DestroyDevice"))
        return (void*) vkDestroyDevice;
    if (!strcmp(name, "GetDeviceQueue"))
        return (void*) vkGetDeviceQueue;
    if (!strcmp(name, "AllocateCommandBuffers"))
        return (void*) vkAllocateCommandBuffers;

    return NULL;
}
