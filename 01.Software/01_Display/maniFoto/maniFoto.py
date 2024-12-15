from rembg.bg import remove

# 待处理图片路径
absolutPath = "/Users/myry/Documents/MyArduinoProject/AWM/Docs/jpgs/"


input_pic_path = absolutPath + "cam_.jpg"

# 处理后图片保存路径
output_pic_path = absolutPath + "cam.jpg"

# 处理图片并保存
with open(input_pic_path, 'rb') as f_input:
    with open(output_pic_path, 'wb') as f_output:
        input_pic = f_input.read()
        output_pic = remove(input_pic)
        f_output.write(output_pic)  # 将处理后的图片数据写入输出文件