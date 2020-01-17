
# -*- coding:utf8 -*
# 根据vcxproj生成cmake文件，便于linux下程序编译

import os
import sys
import xml.etree.ElementTree as ET
import codecs
import re


class cmake_tool(object):
    def __init__(self):
        self.clear()

    def clear(self):
        self.include_directories = []
        self.link_directories = []
        self.compile_files = []
        self.definitions = "-D__LINUX__ -D__LINUX64"
        self.project = ""
        self.root = None  # xml parse
        self.namespace = ""
        self.projectpath = ""
        self.link_libs = ""

    def find_item_group(self, DefinitionType='Debug|x64'):
        groups = self.root.findall('ItemDefinitionGroup')
        item_group = None
        for group in groups:
            if item_group:
                break
            for k, v in group.attrib.items():
                if DefinitionType in v:
                    item_group = group
                    break
        return item_group

    def get_include_directories(self, item_group):
        include_directories_item = item_group.find(
            "ClCompile/AdditionalIncludeDirectories")
        if include_directories_item is None:
            return
        include_directories = ""
        include_directories = include_directories_item.text
        includes = include_directories[0: include_directories.rfind(";")]
        self.include_directories = includes.replace('\\', '/').split(';')
        # print(self.include_directories)

    def get_link_directories(self, item_group):
        link_directories_item = item_group.find(
            "Link/AdditionalLibraryDirectories")
        if link_directories_item is None:
            return
        link_directories = ""
        link_directories = link_directories_item.text
        links = link_directories[0: link_directories.rfind(";")]
        links = links.replace('\\', '/')
        self.link_directories = links.split(';')
        # print(self.link_directories)

    def get_compile_files(self):
        compile_files_item = self.root.findall('ItemGroup/ClCompile')
        for item in compile_files_item:
            for k, v in item.items():
                self.compile_files.append(v.replace('\\', '/'))

    def get_link_libraries(self, item_group):
        link_libraries_item = item_group.find(
            "Link/AdditionalDependencies")
        if link_libraries_item is None:
            return
        link_libraries = ""
        link_libraries = link_libraries_item.text
        link_libraries = link_libraries[0: link_libraries.rfind(";")]
        links = link_libraries.split(';')
        self.link_libs = " ".join(link.replace("ws2_32.lib", "").
                                  replace("win32_interop.lib", "").
                                  replace("libmysql.lib", "").
                                  replace(".lib", ".a") for link in links)
        self.link_libs += " mysqlclient dl stdc++fs pthread"

    def writecmakefile(self, is_lib):
        cmake_str = ""
        cmake_str += 'cmake_minimum_required(VERSION 2.8)'
        cmake_str += '\n'
        cmake_str += 'project(%s)' % self.project
        cmake_str += '\n'
        cmake_str += 'add_definitions(%s)' % self.definitions
        cmake_str += '\n'
        cmake_str += 'include_directories(%s)' % " ".join(
            self.include_directories)
        cmake_str += '\n'
        cmake_str += 'link_directories(%s)' % " ".join(self.link_directories)
        cmake_str += '\n'
        cmake_str += 'set(SOURCE_FILE %s)' % " ".join(self.compile_files)
        cmake_str += '\n'
        cmake_str += '''
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
        '''
        cmake_str += '\n'
        if not is_lib:
            cmake_str += 'add_executable(%s ${SOURCE_FILE})' % self.project
        else:
            cmake_str += 'add_library(%s ${SOURCE_FILE})' % self.project

        cmake_str += '\n'
        cmake_str += 'target_link_libraries(%s  %s)' % (
            self.project, self.link_libs)

        print(cmake_str)

        with open(self.projectpath + 'CMakeLists.txt', 'w') as f:
            f.write(cmake_str)
            f.flush()

        self.clear()

    def loadxml(self, xmlfile, path, is_lib=False):
        if not os.path.exists(path):
            print("path not exist")
            return
        self.project = path.split('/')[-2]
        print(self.project)
        self.projectpath = path

        contents = ""
        with codecs.open(xmlfile, "r", "utf-8") as f:
            contents = f.read()

        # xml_contents = contents.replace(
        #     'xmlns="http://schemas.microsoft.com/developer/msbuild/2003"', '')
        pattern = re.compile(r'xmlns=".*"', re.I)
        xml_contents = re.sub(pattern, "", contents)
        self.root = ET.fromstring(xml_contents)
        item_group = self.find_item_group()
        if item_group is None:
            print("not find x64 setting")
            return
        self.get_include_directories(item_group)
        self.get_link_directories(item_group)
        self.get_link_libraries(item_group)
        self.get_compile_files()
        # write cmake file
        self.writecmakefile(is_lib)


if __name__ == '__main__':
    tool = cmake_tool()
    # tool.loadxml("../GameServer/GameServer.vcxproj", "../GameServer/")
    # tool.loadxml("../ChatServer/ChatServer.vcxproj", "../ChatServer/")
    # tool.loadxml("../LoginServer/LoginServer.vcxproj", "../LoginServer/")
    # tool.loadxml("../DBServer/DBServer.vcxproj", "../DBServer/")
    # tool.loadxml("../WorldServer/WorldServer.vcxproj", "../WorldServer/")
    tool.loadxml("../Common/Common.vcxproj", "../Common/", True)
    tool.loadxml("../SeNet/SeNet.vcxproj", "../SeNet/", True)
