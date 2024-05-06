# xml_synchronize

resource/special_enum.txt为特殊元素文件，每个特殊元素1行，同步时跳过这些特殊元素，可以自行添加

bin/synchronize为静态编译的程序，可以直接运行，

运行格式：第一个参数为特殊元素文件的路径，第二个参数为源xml文件，

后续参数为多个需要同步的xml文件

eg: ./bin/synchronize ./resource/special_enum.txt ./resource/cell1.xml ./resource/cell2.xml ./resource/cell3.xml ./resource/cell4.xml
