#!/usr/bin/python
# -*- coding: UTF-8 -*-

# 生成原始版本信息
# 生成当前版本信息
# 两个版本进行对比差异
# 记录并压缩文件保存

# -g 生成版本信息  -c 生成对比结果
# todo 可以未压缩文件

import sys,getopt
import os
import hashlib,codecs
import json
from enum import Enum

VERSION_FILE = "Server.ver"
COMP_FILE = "Comp.ver"  # 保存对比结果

VER_FLAG_UPDATE = 1 # 文件更新
VER_FLAG_NEW = 2 # 文件新增
VER_FLAG_DEL = 3 # 文件删除


# 一个文件保存基本信息 name, size, version(md5)
class FileInfo:
    def __init__(self, name, size, ver):
        self.name = name
        self.size = size
        self.ver = ver
    def to_dic(self):
        dic = {"filename":self.name,"filesize":self.size,"filever":self.ver}
        return dic
    def __str__(self):
        return self.name + ":" + str(self.size) + ":" + self.ver

class GenVersion:
    def __init__(self):
        super().__init__()
        self.m = hashlib.md5()
        self.ver_dic = {}
        self.ver_comp = []

    def gen_file_ver(self, file_name):
        if os.path.splitext(file_name)[1] in (".h", ".cpp"):
            file_size = os.path.getsize(file_name)
            file_content = ""
            with codecs.open(file_name, "r", "utf-8") as f:
                file_content = f.read()
            if not file_content:
                return
            file_content = file_content.encode(encoding='utf-8')
            self.m.update(file_content)
            file_ver = self.m.hexdigest()
            file_info = FileInfo(file_name, file_size, file_ver)
            print(file_info)
            self.ver_dic[file_name] = file_info.to_dic()
        pass

    def gen_list_dir(self, root_path):
        list_files = os.listdir(root_path)
        if not list_files:
            return
        print(list_files)
        for file in list_files:
            file_path = root_path+"/"+file
            if os.path.isdir(file_path):
               self.gen_version(file_path)     
            if os.path.isfile(file_path):
                self.gen_file_ver(file_path)

    def gen_version(self, root_path):
        self.gen_list_dir(root_path)
        # 生成原始版本信息
        with codecs.open(VERSION_FILE, "w", "utf-8") as f:
            content = json.dumps(self.ver_dic, ensure_ascii=False)
            f.write(content)
            f.flush()
            pass

    def compare_list(self, old_version, new_version):
        for filename, file_info in old_version.items():
            if filename in new_version.keys():
                if file_info["filever"] != new_version[filename]["filever"]:
                    ver_update = {"filename":filename,"ver":new_version[filename]["filever"],"flag":VER_FLAG_UPDATE}
                    self.ver_comp.append(ver_update)
            if filename not in new_version.keys():
                ver_update = {"filename":filename, "ver":file_info["filever"],"flag":VER_FLAG_DEL}
                self.ver_comp.append(ver_update)

        print(old_version.keys())
        for filename, file_info in new_version.items():
            if filename not in old_version.keys():
                ver_update = {"filename":filename,"ver":file_info["filever"] ,"flag":VER_FLAG_NEW}
                self.ver_comp.append(ver_update)
        pass        

    def comp_version(self, root_path):
        self.gen_list_dir(root_path)
        new_version = self.ver_dic
        old_version = {}
        with codecs.open(VERSION_FILE, "r", "utf-8") as f:
            content = f.read()
            if not content:
                return
            old_version = json.loads(content)
            print(old_version)
        if old_version and new_version:
            self.compare_list(old_version, new_version)
        if self.ver_comp:
            # 生成原始版本信息
            with codecs.open(COMP_FILE, "w", "utf-8") as f:
                content = json.dumps(self.ver_comp, ensure_ascii=False)
                f.write(content)
                f.flush()
                pass            

    def main(self,argv):
        try:
            opts,args = getopt.getopt(argv, "hgc")
        except getopt.GetoptError:
            print("usage:genversion.py -g or -c ")
            sys.exit(2)
        for opt,arg in opts:
            if opt == '-h':
                print("usage:genversion.py -g or -c")
                sys.exit()
            if opt == "-g":
                self.gen_version()
            if opt == "-c":
                self.comp_version()


if __name__ == '__main__':
    genversion = GenVersion()
    # genversion.main(sys.argv[1:])
    # genversion.gen_file_ver("../../LoginServer")
    #genversion.gen_version("../../WorldServer")
    genversion.comp_version("../../WorldServer")

