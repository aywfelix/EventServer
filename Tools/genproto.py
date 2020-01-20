#通过python进行proto编译

import os,sys

class GenProto(object):
    def __init__(self):
        self.protodic = {}  #保存proto文件及md5值，然后根据md5决定是否编译文件
        pass

    @staticmethod
    def genproto(proto_path):
        list_dirs = os.listdir(proto_path)
        for proto in list_dirs:
            if proto.endswith((".h", ".cc",".pb.h", ".pb.cc")):
                continue
            if proto_path.endswith("/"):
                protofile = proto_path + proto
            else:
                protofile = os.path.join(proto_path, proto)
            print(protofile)
            cmd_str = "protoc -I="+proto_path + " --cpp_out="+proto_path + " " + proto
            os.system(cmd_str)


if __name__ == "__main__":
    GenProto.genproto("../Common/ServerProto")
    GenProto.genproto("../Common/ClientProto")
    #todo 加入md5验证文件是否需要重新生成