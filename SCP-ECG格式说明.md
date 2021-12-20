[TOC]

# SCP-ECG格式说明

1. 对于所有有符号的二进制值，应采用2的补码。

2. 小端字节序

3. The term "Rhythm Data" is used to indicate the ECG recording over the entire recording time, usually 10-seconds in most recorders.

4. 每个section偶数字节补齐，只在每个section结尾进行补齐，使用NULL进行补齐

5. 默认字符集为 Latin-1 (ISO-8859-1) 

SCP-ECG文档中描述存在4种类别实现，区别在于包含的数据存在差异，如下图：
第1种只包含患者信息，测量参数，诊断结论，
第2种额外包含了波形数据，
第3种额外包含了平均模板数据，
第4种额外包含波形数据和平均模板数据。

其中最大的难点在于平均模板数据，SCP-ECG的最大优势是实现了心电数据的压缩，通过平均模板数据作为参考数据，再将每个QRS波的数据减去这个参考数据，然后进行Huffman编码，从而减少编码数据量。

![RecordOverview](.\SCP-ECG Figure\SCP-ECG Category.jpg)

![all Section](./SCP-ECG Figure/all Section.png)

## 整体结构定义

- 2个字节的CRC
- 4字节总文件长度
- Section区域

![RecordOverview](.\SCP-ECG Figure\RecordOverview.png)

### 每个Section由数据头和数据部分组成

- 16字节 Section ID Header
- Section Data Part

![SectionLayout](.\SCP-ECG Figure\SectionLayout.png)

## Section0

![Section0_data_part](.\SCP-ECG Figure\Section0_data_part.png)

## Section1

![Section1_data_part](W:\zq1219\targetNFS\zq_scp_ecg\SCP-ECG Figure\Section1_data_part.png)

## Section2

![Section2_HuffmanTables](.\SCP-ECG Figure\Section2_HuffmanTables.png)

## Section3

![Section3_lead_definition](.\SCP-ECG Figure\Section3_lead_definition.png)
## Section4

![Section4_qrs](.\SCP-ECG Figure\Section4_qrs.png)
## Section5

![Section5_reference_beat](.\SCP-ECG Figure\Section5_reference_beat.png)
## Section6

![Section6_rhythm_data](.\SCP-ECG Figure\Section6_rhythm_data.png)

## Section7

![Section7_measurements](.\SCP-ECG Figure\Section7_measurements.png)
## Section8

![Section8_diagnostic](.\SCP-ECG Figure\Section8_diagnostic.png)
## Section9
厂家特定的诊断结论和overreading data

## Section10

![Section10_lead_measurements](.\SCP-ECG Figure\Section10_lead_measurements.png)

## Section11

![Section11_diagnostic](.\SCP-ECG Figure\Section11_diagnostic.png)

# 例子文件解析

## Section0~4

![SCP-ECG文件分析Section0-4](.\SCP-ECG example\SCP-ECG文件分析Section0-4.png)

## Section0

![SCP-ECG文件分析Section0](.\SCP-ECG example\SCP-ECG文件分析Section0.png)

## Section6

![SCP-ECG文件分析Section6](.\SCP-ECG example\SCP-ECG文件分析Section6.png)

## Section7

![SCP-ECG文件分析Section7](.\SCP-ECG example\SCP-ECG文件分析Section7.png)