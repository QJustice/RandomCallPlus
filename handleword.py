# This Python file uses the following encoding: utf-8
from docx import Document
import os


def write_word(file_name, text_data, flag_name, save_path):
    """替换word文本框中的内容"""
#    print("write_word is done")
#    print(file_name)
#    print(text_data)
#    print(flag_name)
#    print(save_path)
    ERROR_CODE = 0  # 无错误
    try:
        docStr = Document(file_name)  # 打开文件
    except:
        ERROR_CODE = 1  # 文件不存在
        return ERROR_CODE  # 返回错误码
    if text_data == "":  # 判断写入内容是否为空
        ERROR_CODE = 2  # 写入内容为空
        return ERROR_CODE  # 返回错误码
    children = docStr.element.body.iter()  # 获取文档中的所有元素
    for child in children:
        # 通过类型判断目录
        if child.tag.endswith('txbx'):  # 文本框
            for ci in child.iter():  # 文本框中的内容
                if ci.tag.endswith('main}r'):  # 文本框中的文本
                    if ci.text == flag_name:  # 判断文本框中的内容是否为需要替换的内容
                        ci.text = text_data  # 替换文本框中的内容
    try:
        # 保存文件
        folder = os.path.exists(save_path)  # 判断是否存在文件夹如果不存在则创建为文件夹
        if not folder:  # 判断结果
            os.makedirs(save_path)  # makedirs 创建文件时如果路径不存在会创建这个路径
        path = save_path + "/" + text_data + ".docx"  # 保存路径
        docStr.save(path)  # 保存文件
    except:
        ERROR_CODE = 3  # 保存文件失败
        return ERROR_CODE  # 返回错误码
    return ERROR_CODE  # 返回错误码

if __name__ == "__main__":
    write_word("001.docx", "aaa", "NAME", "save")
    print("MAIN")
