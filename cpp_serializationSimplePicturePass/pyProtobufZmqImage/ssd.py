import os
import math
import random

import numpy as np
import tensorflow as tf
import cv2

slim = tf.contrib.slim

import matplotlib.pyplot as plt
import matplotlib.image as mpimg

import sys

#import RealSense
sys.path.append("C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\bin\\x64")
import pyrealsense2 as rs

sys.path.append(r"D:\temp\Deep_Learning\SSD-Tensorflow")
from nets import ssd_vgg_300, ssd_common, np_methods
from preprocessing import ssd_vgg_preprocessing

num_classes = 32

# TensorFlow session: grow memory when needed. TF, DO NOT USE ALL MY GPU MEMORY!!!
gpu_options = tf.GPUOptions(allow_growth=True)
config = tf.ConfigProto(log_device_placement=False, gpu_options=gpu_options)
isess = tf.InteractiveSession(config=config)

# Input placeholder.
net_shape = (300, 300)
data_format = 'NHWC'
img_input = tf.placeholder(tf.uint8, shape=(None, None, 3))
# Evaluation pre-processing: resize to SSD net shape.
image_pre, labels_pre, bboxes_pre, bbox_img = ssd_vgg_preprocessing.preprocess_for_eval(
    img_input, None, None, net_shape, data_format, resize=ssd_vgg_preprocessing.Resize.WARP_RESIZE)
image_4d = tf.expand_dims(image_pre, 0)

# Define the SSD model.
reuse = True if 'ssd_net' in locals() else None
ssd_class = ssd_vgg_300.SSDNet
ssd_params = ssd_class.default_params._replace(num_classes=num_classes)
ssd_net = ssd_class(ssd_params)
with slim.arg_scope(ssd_net.arg_scope(data_format=data_format)):
    predictions, localisations, _, _ = ssd_net.net(image_4d, is_training=False, reuse=reuse)

# Restore SSD model.
ckpt_filename = r'D:/temp1/tless_log7_p3/model.ckpt-43529'
isess.run(tf.global_variables_initializer())
saver = tf.train.Saver()
saver.restore(isess, ckpt_filename)

# SSD default anchor boxes.
ssd_anchors = ssd_net.anchors(net_shape)

# Main image processing routine.
def process_image(img, select_threshold=0.01, nms_threshold=0.45, net_shape=(300, 300), nms_within_same_class=True):
    # Run SSD network.
    rimg, rpredictions, rlocalisations, rbbox_img = isess.run([image_4d, predictions, localisations, bbox_img],
                                                              feed_dict={img_input: img})
    #print(rpredictions[2][0][1][1])
    # Get classes and bboxes from the net outputs.
    rclasses, rscores, rbboxes = np_methods.ssd_bboxes_select(
            rpredictions, rlocalisations, ssd_anchors,
            select_threshold=select_threshold, img_shape=net_shape, num_classes=num_classes, decode=True)
    
    rbboxes = np_methods.bboxes_clip(rbbox_img, rbboxes)
    
    rclasses, rscores, rbboxes = np_methods.bboxes_sort(rclasses, rscores, rbboxes, top_k=400)
    rclasses, rscores, rbboxes = np_methods.bboxes_nms(rclasses, rscores, rbboxes,
                                                       nms_threshold=nms_threshold, within_same_class=nms_within_same_class)
    
    # Resize bboxes to original image shape. Note: useless for Resize.WARP!
    rbboxes = np_methods.bboxes_resize(rbbox_img, rbboxes)
    return rclasses, rscores, rbboxes
	

def drawbbox(img, classes, scores, bboxes):
    
    bboxes[:,0] = img.shape[0]*bboxes[:,0]
    bboxes[:,2] = img.shape[0]*bboxes[:,2]
    bboxes[:,1] = img.shape[1]*bboxes[:,1]
    bboxes[:,3] = img.shape[1]*bboxes[:,3]
    
    for i, label in enumerate(classes):
        color = np.random.rand(3)
        cv2.rectangle(img,(bboxes[i][1],bboxes[i][0]), (bboxes[i][3],bboxes[i][2]), 255*color,4)
    
    cv2.namedWindow('frame', cv2.WINDOW_NORMAL)
    cv2.imshow("frame", img)
    #cv2.waitKey(0)


path = r"D:\temp\Deep_Learning\SSD-Tensorflow\demo_own_gray\01.bmp"
img2=cv2.imread(path)
rclasses, rscores, rbboxes =  process_image(img2, select_threshold=0.2,nms_threshold=0.1, nms_within_same_class=False)
for i, rclass in enumerate(rclasses):
    print("class = ", rclasses[i])
    print("score = ", rscores[i])
    print("box = ", rbboxes[i])
    