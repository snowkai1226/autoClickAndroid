# autoClickAndroid
自动点击安卓手机的控件按钮
实现在pc端自动控制安卓手机按钮的点击功能。
1、首先通过adb命令获取手机当前屏幕的xml文件，然后推送到pc端。
2、对推送到pc端的xml文件进行解析，获取想要点击的按钮的边界坐标。
3、对边界坐标的中心进行计算，得出按钮中心点的坐标
4、然后利用adb命令点击按钮中心点。