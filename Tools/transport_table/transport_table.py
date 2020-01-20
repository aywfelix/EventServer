import xlrd
from xlrd import xldate_as_tuple
import multiprocessing
import json
import os
import codecs
import datetime
import traceback

# 数据类型字典
data_type_dic = {
    "INT": "int",
    "FLOAT": "float",
    "DOUBLE": "double",
    "STRING": "std::string",
    "LI": "std::vector<int>",
    "LD": "std::vector<double>",
    "LF": "std::vector<float>",
    "LS": "std::vector<std::string>"
}

config_dic = {"class_name": "",
              "row_fields": "",
              "json_logic": "",
              "key_field": ""
              }

data_type_trans = {
    "int": "asInt()",
    "float": "asFloat()",
    "double": "asFloat()",
    "std::string": "asString()",
    "std::vector<int>": "asInt()",
    "std::vector<float>": "asFloat()",
    "std::vector<double>": "asFloat()",
    "std::vector<std::string>": "asString()"
}

single_tmpl = '                row.%(fields)s = r["%(fields)s"].%(asType)s;\n'

vector_tmpl = '''
                auto end_%(fields)s = r["%(fields)s"].end();
				auto begin_%(fields)s = r["%(fields)s"].end();
				for (auto it = begin_%(fields)s; it != end_%(fields)s; ++it)
				{
					row.%(fields)s.emplace_back(it->%(asType)s);
				}
            '''


class ExcelToJson:
    def __init__(self):
        self.init_attr()

    def __del__(self):
        pass

    def init_attr(self):
        self.row_fields = ""  # row_fields
        self.classes_name = []  # 表名
        self.json_logic = ""
        self.key_field = ""
        self.fields = []  # 保存字段信息
        pass

    def get_excel(self, srcDir):
        files = os.listdir(srcDir)
        excels = [file for file in files if os.path.splitext(file)[
            1] == ".xlsx" and '~' not in file]

        for file in excels:
            tmp_name = os.path.splitext(file)[0]
            self.classes_name.append(tmp_name.split('_')[0])
        return excels, self.classes_name

    def gen_row_fields(self, data_row_type, data_desc):
        self.row_fields = ""
        tmp_field = ""
        for i in range(len(data_row_type)):
            x = data_row_type[i]
            tmp_field = x[0]+" " + x[1] + ";"
            strlen = 50
            data_desc[i] = data_desc[i].replace("\n", " ")
            self.row_fields += (tmp_field + " " *
                                (strlen-len(tmp_field)) + "//"+data_desc[i])
            self.row_fields += "\n"

    def gen_json_logic(self, data_row_type):
        for i in range(len(data_row_type)):
            x_tuple = data_row_type[i]
            if "vector" in x_tuple[0]:
                self.json_logic += (vector_tmpl) % {
                    "fields": x_tuple[1], "asType": data_type_trans[x_tuple[0]]}
            elif x_tuple[0] in ["int", "std::string", "float", "double"]:
                self.json_logic += (single_tmpl) % {
                    "fields": x_tuple[1], "asType": data_type_trans[x_tuple[0]]}

    def transport_config_cpp(self, table_name):
        s = ""
        with codecs.open("./table_cpp.tmpl", "r", "utf-8") as f:
            s = f.read()
        if not s:
            return

        s = s % {"class_name": table_name,
                 "row_fields": self.row_fields, "json_logic": self.json_logic}

        with codecs.open(table_name+".hpp", "w", "utf-8") as f:
            f.write(s)
            f.flush()
            pass

    def fix_row_dict(self, data_row_type, row_values):
        row_dict = {}
        for i in range(len(data_row_type)):
            if row_values[i] is None:
                row_values[i] = ""
            data_type_tuple = data_row_type[i]

            if "int" == data_type_tuple[0]:
                row_dict[data_type_tuple[1]] = int(row_values[i])
            if "float" == data_type_tuple[0]:
                row_dict[data_type_tuple[1]] = float(row_values[i])
            if "double" == data_type_tuple[0]:
                row_dict[data_type_tuple[1]] = float(row_values[i])
            if "std::string" == data_type_tuple[0]:
                row_dict[data_type_tuple[1]] = str(row_values[i])
            if "vector" in data_type_tuple[0] and row_values[i] == "":
                row_dict[data_type_tuple[1]] = []
                continue
            if "vector" in data_type_tuple[0] and row_values[i] != "":
                if isinstance(row_values[i], float):
                    row_dict[data_type_tuple[1]] = [int(row_values[i])]
                    continue
            if "std::vector<int>" == data_type_tuple[0]:
                row_dict[data_type_tuple[1]] = list(
                    map(int, row_values[i].split('|')))
            if "std::vector<double>" == data_type_tuple[0]:
                row_dict[data_type_tuple[1]] = list(
                    map(float, row_values[i].split('|')))
            if "std::vector<float>" == data_type_tuple[0]:
                row_dict[data_type_tuple[1]] = list(
                    map(float, row_values[i].split('|')))
            if "std::vector<std::string>" == data_type_tuple[0]:
                row_dict[data_type_tuple[1]] = list(
                    map(str, row_values[i].split('|')))
        print("=============", row_dict)
        return row_dict
        pass

    def transport_json(self, table_name, data_row_type, sheet):
        rows = 5
        rowe = sheet.nrows
        if rowe <= rows:
            return
        all_rows = {}
        while rows < rowe:
            row_dict = self.fix_row_dict(data_row_type, sheet.row_values(rows))
            all_rows[row_dict["id"]] = row_dict
            rows = rows+1

        # 写入json
        with open(table_name+".json", 'w+') as f:
            jsonStr = json.dumps(
                all_rows, indent=4, sort_keys=False, ensure_ascii=False)
            f.write(jsonStr + '\n')

    def read_excel(self, excelName, table_name):
        try:
            excelFile = xlrd.open_workbook(excelName)
            excelSheetNames = excelFile.sheet_names()
            # print(excelSheetNames)
            sheet = excelFile.sheet_by_name(excelSheetNames[0])
            excel_data_type = sheet.row_values(0)
            # print(excel_data_type)
            self.fields = sheet.row_values(2)
            # print(self.fields)
            data_row_type = {}
            data_type = []
            for x in excel_data_type:
                data_type.append(data_type_dic[x])
            data_row_type = list(zip(data_type, self.fields))
            print(data_row_type)
            data_desc1 = sheet.row_values(3)
            data_desc2 = sheet.row_values(4)
            data_desc = [a+" "+b for a, b in zip(data_desc1, data_desc2)]
            # print(data_desc)
            self.gen_row_fields(data_row_type, data_desc)
            self.gen_json_logic(data_row_type)
            # 生成cpp 文件及json文件
            self.transport_json(table_name, data_row_type, sheet)
            self.transport_config_cpp(table_name)
            self.init_attr()
        except Exception as e:
            print('str(Exception):\t', str(e))
            print('traceback.print_exc():', traceback.print_exc())
        pass


if __name__ == "__main__":
    srcDir = "./"
    excelToJson = ExcelToJson()
    excels, classes_name = excelToJson.get_excel(srcDir)
    print(excels)
    pool = multiprocessing.Pool(processes=5)
    for excel in excels:
        class_name = (os.path.splitext(excel)[0]).split('_')[0]
        pool.apply_async(excelToJson.read_excel, (srcDir+excel, class_name))
    # gc pool
    pool.close()
    pool.join()
    pass
