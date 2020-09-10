'''
Author: jia.lai
Date: 2020-01-20 17:41:45
LastEditTime: 2020-09-10 23:56:20
Description: 
Version: 1.0
'''
# 通过python进行proto编译

import os
import sys


class GenProto(object):
    def __init__(self):
        self.root_path = ""
        pass

    def reverse_genproto(self, proto_path):
        list_dirs = os.listdir(proto_path)
        for proto in list_dirs:
            if proto_path.endswith("/"):
                protofile = proto_path + proto
            else:
                protofile = os.path.join(proto_path, proto)
            # print(protofile)
            if os.path.isdir(protofile):
                self.reverse_genproto(protofile)
            else:
                if not proto.endswith((".proto")):
                    continue
                cmd_str = "protoc -I="+proto_path + " -I=" + \
                    self.root_path + " --cpp_out="+proto_path + " " + proto
                print(cmd_str)
                os.system(cmd_str)

    def genproto(self, proto_path):
        self.root_path = proto_path
        self.reverse_genproto(proto_path)


if __name__ == "__main__":
    gen_proto = GenProto()
    gen_proto.genproto("../NetProto/")
    gen_proto.genproto("../SeNet/")
