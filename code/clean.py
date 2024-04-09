'''
清除 MDK 项目编译后生成的文件，让项目回到最干净的状态。

python clean.py 运行
'''
import os
import shutil

dir_path = os.getcwd()

if __name__ == '__main__':

    if os.path.isdir(dir_path + '\\objects'):
        shutil.rmtree(dir_path + '\\objects')
    else:
        print('文件夹不存在')
        
    if os.path.isdir(dir_path + '\\listings'):
        shutil.rmtree(dir_path + '\\listings')
    else:
        print('文件夹不存在')