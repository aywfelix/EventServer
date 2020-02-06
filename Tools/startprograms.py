import os,sys

class StartProgram(object):
    def __init__(self):
        pass

    @staticmethod
    def start(proto_path, program):
        list_dirs = os.listdir(proto_path)
        for proto in list_dirs:
            if not proto.endswith((".exe")):
                continue
            if program and proto == program:
                if proto_path.endswith("/"):
                    protofile = proto_path + proto
                else:
                    protofile = os.path.join(proto_path, proto)
                # print(protofile)
                os.startfile(protofile)



if __name__ == "__main__":
    
    workPath = os.path.abspath('..')
    binPath = workPath +"\\bin\\"
    StartProgram.start(binPath, "MasterServer.exe")
    StartProgram.start(binPath, "GateServer.exe")
    StartProgram.start(binPath, "ChatServer.exe")
    StartProgram.start(binPath, "MyClient.exe")
    

    