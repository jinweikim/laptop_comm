# -*- coding: utf-8 -*-
import serial
#打开串口
serialPort="COM13" #串口
baudRate=115200#波特率
ser=serial.Serial(serialPort,baudRate,timeout=0.5) 
print("参数设置：串口={}，波特率={}".format(serialPort,baudRate))

#收发数据
while(1):
    str = input("请输入要发送的数据: ")
    if str == 'exit':
        break
    ser.write((str+'\n').encode())
    print(ser.readline())
ser.close()