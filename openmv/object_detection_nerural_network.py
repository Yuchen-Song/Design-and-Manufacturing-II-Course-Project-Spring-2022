# Edge Impulse - OpenMV Object Detection Example

import sensor, image, time, os, tf, math, uos, gc
from pyb import UART

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=2000)          # Let the camera adjust.
uart = UART(3, 256000)

net = None
labels = None
min_confidence = 0.5
arr=[0x0A,0x0A,0,0,0,0,0,0,0,0,0,0,0,0]
z = {'bottle':220,'orange':40,'eraser':5,'pen':5}
gripper ={'bottle':60,'orange':40,'eraser':80,'pen':55}
#uart.init(9600,8,None,1) #8个数据位，无奇偶校验，1个停止位

try:
    # Load built in model
    labels, net = tf.load_builtin_model('trained')
except Exception as e:
    raise Exception(e)

colors = [ # Add more colors if you are detecting more than 7 types of classes at once.
    (255,   0,   0),
    (  0, 255,   0),
    (255, 255,   0),
    (  0,   0, 255),
    (255,   0, 255),
    (  0, 255, 255),
    (255, 255, 255),
]

#clock = time.clock()
while(True):
    #clock.tick()

    img = sensor.snapshot().lens_corr(1.8)

    # detect() returns all objects found in the image (splitted out per class already)
    # we skip class index 0, as that is the background, and then draw circles of the center
    # of our objects

    for i, detection_list in enumerate(net.detect(img, thresholds=[(math.ceil(min_confidence * 255), 255)])):
        if (i == 0): continue # background class
        if (len(detection_list) == 0): continue # no detections for this class?

        print("********** %s **********" % labels[i])
        for d in detection_list:
            [x, y, w, h] = d.rect()
            center_x = math.floor(x + (w / 2))
            center_y = math.floor(y + (h / 2))
            #print('x %d\ty %d' % (center_x, center_y))
            img.draw_circle((center_x, center_y, 12), color=colors[i], thickness=2)

            x1 = 120-center_y
            y1 = center_x-120
            x2 = 1.38*x1 #1.38表示mm与px的比例
            y2 = 1.38*y1
            X = int(x2+200) # 摄像头和基座的沿X轴线的距离
            Y = int(y2)+300
            Z = z[labels[i]]
            G = gripper[labels[i]]
            arr[2]=int(X/100)
            arr[3]=int(X%100/10)
            arr[4]=X%10
            arr[5]=int(Y/100)
            arr[6]=int(Y%100/10)
            arr[7]=Y%10
            arr[8]=int(Z/100)
            arr[9]=int(Z%100/10)
            arr[10]=int(Z%10)
            arr[11]=int(G/100)
            arr[12]=int(G%100/10)
            arr[13]=int(G%10)
            uart.write('A')
            uart.write('B')
            for index in range (12):
                uart.write(str(arr[2+index]))
            #print('Transmission finished!')
            print(f'X={X},Y={Y},Z={Z},G={G}')

    #print(clock.fps(), "fps", end="\n\n")
    #if uart.any():  # 判断是否有数据
        #data = uart.read()  #将读取的数据存入data
        #print(data)
