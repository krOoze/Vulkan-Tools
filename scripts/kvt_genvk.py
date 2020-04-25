#!/usr/bin/python3
#
# Copyright (c) 2013-2019 The Khronos Group Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import cProfile
import pdb
import string
import sys
import time
import os

# Simple timer functions
startTime = None


def startTimer(timeit):
    global startTime
    if timeit:
        startTime = time.process_time()


def endTimer(timeit, msg):
    global startTime
    if timeit:
        endTime = time.process_time()
        write(msg, endTime - startTime, file=sys.stderr)
        startTime = None

# Turn a list of strings into a regexp string matching exactly those strings


def makeREstring(list, default=None):
    if len(list) > 0 or default is None:
        return '^(' + '|'.join(list) + ')$'
    else:
        return default

# Returns a directory of [ generator function, generator options ] indexed
# by specified short names. The generator options incorporate the following
# parameters:
#
# args is an parsed argument object; see below for the fields that are used.


def makeGenOpts(args):
    global genOpts
    genOpts = {}

    # Default class of extensions to include, or None
    defaultExtensions = args.defaultExtensions

    # Additional extensions to include (list of extensions)
    extensions = args.extension

    # Extensions to remove (list of extensions)
    removeExtensions = args.removeExtensions

    # Extensions to emit (list of extensions)
    emitExtensions = args.emitExtensions

    # Features to include (list of features)
    features = args.feature

    # Whether to disable inclusion protect in headers
    protect = args.protect

    # Output target directory
    directory = args.directory

    # Descriptive names for various regexp patterns used to select
    # versions and extensions
    allFeatures = allExtensions = '.*'
    noFeatures = noExtensions = None

    # Turn lists of names/patterns into matching regular expressions
    addExtensionsPat = makeREstring(extensions, None)
    removeExtensionsPat = makeREstring(removeExtensions, None)
    emitExtensionsPat = makeREstring(emitExtensions, allExtensions)
    featuresPat = makeREstring(features, allFeatures)

    # Copyright text prefixing all headers (list of strings).
    prefixStrings = [
        '/*',
        '** Copyright (c) 2015-2020 The Khronos Group Inc.',
        '**',
        '** Licensed under the Apache License, Version 2.0 (the "License");',
        '** you may not use this file except in compliance with the License.',
        '** You may obtain a copy of the License at',
        '**',
        '**     http://www.apache.org/licenses/LICENSE-2.0',
        '**',
        '** Unless required by applicable law or agreed to in writing, software',
        '** distributed under the License is distributed on an "AS IS" BASIS,',
        '** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.',
        '** See the License for the specific language governing permissions and',
        '** limitations under the License.',
        '*/',
        ''
    ]

    # Text specific to Vulkan headers
    vkPrefixStrings = [
        '/*',
        '** This file is generated from the Khronos Vulkan XML API Registry.',
        '**',
        '*/',
        ''
    ]

    # Defaults for generating re-inclusion protection wrappers (or not)
    protectFeature = protect

    # An API style conventions object
    conventions = VulkanConventions()

    # Helper file generator options for typemap_helper.h
    genOpts['vk_typemap_helper.h'] = [
        HelperFileOutputGenerator,
        HelperFileOutputGeneratorOptions(
            conventions=conventions,
            filename='vk_typemap_helper.h',
            directory=directory,
            apiname='vulkan',
            profile=None,
            versions=featuresPat,
            emitversions=featuresPat,
            defaultExtensions='vulkan',
            addExtensions=addExtensionsPat,
            removeExtensions=removeExtensionsPat,
            emitExtensions=emitExtensionsPat,
            prefixText=prefixStrings + vkPrefixStrings,
            protectFeature=False,
            apicall='VKAPI_ATTR ',
            apientry='VKAPI_CALL ',
            apientryp='VKAPI_PTR *',
            alignFuncParam=48,
            expandEnumerants=False,
            helper_file_type='typemap_helper_header')
    ]

    # Options for mock ICD header
    genOpts['mock_icd_extension_list.h'] = [
        MockICDOutputGenerator,
        MockICDGeneratorOptions(
            conventions=conventions,
            filename='mock_icd_extension_list.h',
            directory=directory,
            apiname='vulkan',
            profile=None,
            versions=featuresPat,
            emitversions=featuresPat,
            defaultExtensions='vulkan',
            addExtensions=addExtensionsPat,
            removeExtensions=removeExtensionsPat,
            emitExtensions=emitExtensionsPat,
            prefixText=prefixStrings + vkPrefixStrings,
            protectFeature=False,
            apicall='VKAPI_ATTR ',
            apientry='VKAPI_CALL ',
            apientryp='VKAPI_PTR *',
            alignFuncParam=48,
            expandEnumerants=False,
            helper_file_type='ext_list')
    ]

    # Options for mock ICD header
    genOpts['mock_icd_commands.h'] = [
        MockICDOutputGenerator,
        MockICDGeneratorOptions(
            conventions=conventions,
            filename='mock_icd_commands.h',
            directory=directory,
            apiname='vulkan',
            profile=None,
            versions=featuresPat,
            emitversions=featuresPat,
            defaultExtensions='vulkan',
            addExtensions=addExtensionsPat,
            removeExtensions=removeExtensionsPat,
            emitExtensions=emitExtensionsPat,
            prefixText=prefixStrings + vkPrefixStrings,
            protectFeature=False,
            apicall='VKAPI_ATTR ',
            apientry='VKAPI_CALL ',
            apientryp='VKAPI_PTR *',
            alignFuncParam=48,
            expandEnumerants=False,
            helper_file_type='commands_header')
    ]

    # Options for mock ICD cpp
    genOpts['mock_icd_commands.cpp.inc'] = [
        MockICDOutputGenerator,
        MockICDGeneratorOptions(
            conventions=conventions,
            filename='mock_icd_commands.cpp.inc',
            directory=directory,
            apiname='vulkan',
            profile=None,
            versions=featuresPat,
            emitversions=featuresPat,
            defaultExtensions='vulkan',
            addExtensions=addExtensionsPat,
            removeExtensions=removeExtensionsPat,
            emitExtensions=emitExtensionsPat,
            prefixText=prefixStrings + vkPrefixStrings,
            protectFeature=False,
            apicall='VKAPI_ATTR ',
            apientry='VKAPI_CALL ',
            apientryp='VKAPI_PTR *',
            alignFuncParam=48,
            expandEnumerants=False,
            helper_file_type='mock_icd_source')
    ]

    # Options for mock ICD cpp
    genOpts['mock_icd_wsi_exports.cpp.inc'] = [
        MockICDOutputGenerator,
        MockICDGeneratorOptions(
            conventions=conventions,
            filename='mock_icd_wsi_exports.cpp.inc',
            directory=directory,
            apiname='vulkan',
            profile=None,
            versions=featuresPat,
            emitversions=featuresPat,
            defaultExtensions='vulkan',
            addExtensions=addExtensionsPat,
            removeExtensions=removeExtensionsPat,
            emitExtensions=emitExtensionsPat,
            prefixText=prefixStrings + vkPrefixStrings,
            protectFeature=False,
            apicall='VKAPI_ATTR ',
            apientry='VKAPI_CALL ',
            apientryp='VKAPI_PTR *',
            alignFuncParam=48,
            expandEnumerants=False,
            helper_file_type='wsi_exports')
    ]

    # Options for vulkaninfo.hpp
    genOpts['vulkaninfo.hpp'] = [
        VulkanInfoGenerator,
        VulkanInfoGeneratorOptions(
            conventions=conventions,
            filename='vulkaninfo.hpp',
            directory=directory,
            apiname='vulkan',
            profile=None,
            versions=featuresPat,
            emitversions=featuresPat,
            defaultExtensions='vulkan',
            addExtensions=addExtensionsPat,
            removeExtensions=removeExtensionsPat,
            emitExtensions=emitExtensionsPat,
            prefixText=prefixStrings + vkPrefixStrings,
            protectFeature=False,
            apicall='VKAPI_ATTR ',
            apientry='VKAPI_CALL ',
            apientryp='VKAPI_PTR *',
            alignFuncParam=48,
            expandEnumerants=False)
    ]


# Generate a target based on the options in the matching genOpts{} object.
# This is encapsulated in a function so it can be profiled and/or timed.
# The args parameter is an parsed argument object containing the following
# fields that are used:
#   target - target to generate
#   directory - directory to generate it in
#   protect - True if re-inclusion wrappers should be created
#   extensions - list of additional extensions to include in generated
#   interfaces
def genTarget(args):
    global genOpts

    # Create generator options with specified parameters
    makeGenOpts(args)

    if (args.target in genOpts.keys()):
        createGenerator = genOpts[args.target][0]
        options = genOpts[args.target][1]

        if not args.quiet:
            write('* Building', options.filename, file=sys.stderr)
            write('* options.versions          =',
                  options.versions, file=sys.stderr)
            write('* options.emitversions      =',
                  options.emitversions, file=sys.stderr)
            write('* options.defaultExtensions =',
                  options.defaultExtensions, file=sys.stderr)
            write('* options.addExtensions     =',
                  options.addExtensions, file=sys.stderr)
            write('* options.removeExtensions  =',
                  options.removeExtensions, file=sys.stderr)
            write('* options.emitExtensions    =',
                  options.emitExtensions, file=sys.stderr)

        startTimer(args.time)
        gen = createGenerator(errFile=errWarn,
                              warnFile=errWarn,
                              diagFile=diag)
        reg.setGenerator(gen)
        reg.apiGen(options)

        if not args.quiet:
            write('* Generated', options.filename, file=sys.stderr)
        endTimer(args.time, '* Time to generate ' + options.filename + ' =')
    else:
        write('No generator options for unknown target:',
              args.target, file=sys.stderr)


# -feature name
# -extension name
# For both, "name" may be a single name, or a space-separated list
# of names, or a regular expression.
if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument('-defaultExtensions', action='store',
                        default='vulkan',
                        help='Specify a single class of extensions to add to targets')
    parser.add_argument('-extension', action='append',
                        default=[],
                        help='Specify an extension or extensions to add to targets')
    parser.add_argument('-removeExtensions', action='append',
                        default=[],
                        help='Specify an extension or extensions to remove from targets')
    parser.add_argument('-emitExtensions', action='append',
                        default=[],
                        help='Specify an extension or extensions to emit in targets')
    parser.add_argument('-feature', action='append',
                        default=[],
                        help='Specify a core API feature name or names to add to targets')
    parser.add_argument('-debug', action='store_true',
                        help='Enable debugging')
    parser.add_argument('-dump', action='store_true',
                        help='Enable dump to stderr')
    parser.add_argument('-diagfile', action='store',
                        default=None,
                        help='Write diagnostics to specified file')
    parser.add_argument('-errfile', action='store',
                        default=None,
                        help='Write errors and warnings to specified file instead of stderr')
    parser.add_argument('-noprotect', dest='protect', action='store_false',
                        help='Disable inclusion protection in output headers')
    parser.add_argument('-profile', action='store_true',
                        help='Enable profiling')
    parser.add_argument('-registry', action='store',
                        default='vk.xml',
                        help='Use specified registry file instead of vk.xml')
    parser.add_argument('-time', action='store_true',
                        help='Enable timing')
    parser.add_argument('-validate', action='store_true',
                        help='Enable group validation')
    parser.add_argument('-o', action='store', dest='directory',
                        default='.',
                        help='Create target and related files in specified directory')
    parser.add_argument('target', metavar='target', nargs='?',
                        help='Specify target')
    parser.add_argument('-quiet', action='store_true', default=True,
                        help='Suppress script output during normal execution.')
    parser.add_argument('-verbose', action='store_false', dest='quiet', default=True,
                        help='Enable script output during normal execution.')

    # This argument tells us where to load the script from the Vulkan-Headers registry
    parser.add_argument('-scripts', action='store',
                        help='Find additional scripts in this directory')

    args = parser.parse_args()

    # default scripts path to be same as registry
    if not args.scripts:
        args.scripts = os.path.dirname(args.registry)

    scripts_directory_path = os.path.dirname(os.path.abspath(__file__))
    registry_headers_path = os.path.join(scripts_directory_path, args.scripts)
    sys.path.insert(0, registry_headers_path)

    from reg import *
    from generator import write
    from cgenerator import CGeneratorOptions, COutputGenerator

    # Generator Modifications
    from mock_icd_generator import MockICDGeneratorOptions, MockICDOutputGenerator
    from vulkan_tools_helper_file_generator import HelperFileOutputGenerator, HelperFileOutputGeneratorOptions
    from vulkaninfo_generator import VulkanInfoGenerator, VulkanInfoGeneratorOptions
    # Temporary workaround for vkconventions python2 compatibility
    import abc
    abc.ABC = abc.ABCMeta('ABC', (object,), {})
    from vkconventions import VulkanConventions

    # This splits arguments which are space-separated lists
    args.feature = [name for arg in args.feature for name in arg.split()]
    args.extension = [name for arg in args.extension for name in arg.split()]

    # Load & parse registry
    reg = Registry()

    startTimer(args.time)
    tree = etree.parse(args.registry)
    endTimer(args.time, '* Time to make ElementTree =')

    if args.debug:
        pdb.run('reg.loadElementTree(tree)')
    else:
        startTimer(args.time)
        reg.loadElementTree(tree)
        endTimer(args.time, '* Time to parse ElementTree =')

    if (args.validate):
        reg.validateGroups()

    if (args.dump):
        write('* Dumping registry to regdump.txt', file=sys.stderr)
        reg.dumpReg(filehandle=open('regdump.txt', 'w', encoding='utf-8'))

    # create error/warning & diagnostic files
    if (args.errfile):
        errWarn = open(args.errfile, 'w', encoding='utf-8')
    else:
        errWarn = sys.stderr

    if (args.diagfile):
        diag = open(args.diagfile, 'w', encoding='utf-8')
    else:
        diag = None

    if (args.debug):
        pdb.run('genTarget(args)')
    elif (args.profile):
        import cProfile
        import pstats
        cProfile.run('genTarget(args)', 'profile.txt')
        p = pstats.Stats('profile.txt')
        p.strip_dirs().sort_stats('time').print_stats(50)
    else:
        genTarget(args)
