---
layout: page
title: 资源使用说明
---

# 资源使用说明

本文件说明如何获取和使用《FPGA 数字电路与系统设计：高层次综合与软硬件协同优化》的配套资源。

## 获取资源

### 下载全部资源

在 GitHub 页面点击 `Code` -> `Download ZIP`，即可下载当前仓库中的全部公开资源。

### 使用 Git 克隆

如果熟悉 Git，可以使用以下命令克隆仓库：

```bash
git clone https://github.com/opentpu/FPGA-Digital-Circuits-and-System-Design.git
```

后续更新时，在仓库目录中执行：

```bash
git pull
```

## 目录说明

| 目录 | 内容 |
| --- | --- |
| `resources/chapter-XX/slides/` | 各章节演示幻灯片 |
| `resources/chapter-XX/code/` | 各章节示例代码 |
| `resources/chapter-XX/labs/` | 各章节实验指导材料 |
| `resources/chapter-XX/assignments/` | 各章节作业材料 |
| `resources/chapter-XX/answers/` | 各章节参考答案 |
| `resources/common/` | 跨章节通用资源 |
| `projects/` | 跨章节项目工程文件 |
| `materials/book/` | 教材相关资料 |
| `docs/` | 说明文档、勘误和更新记录 |

## 教材 PDF

较大的教材 PDF 不直接长期保存在主仓库中，而是通过 GitHub Releases 发布：

https://github.com/opentpu/FPGA-Digital-Circuits-and-System-Design/releases/tag/book-draft-2026-06-08

## 提交反馈

读者可以通过 GitHub Issues 或邮箱反馈问题。

### 勘误反馈

如果发现教材正文、图表、公式、代码片段或页码引用有问题，请提交勘误反馈：

https://github.com/opentpu/FPGA-Digital-Circuits-and-System-Design/issues/new?template=errata.yml

建议提供以下信息：

- 问题类型
- 所在章节
- 页码或小节位置
- 原文内容
- 建议修改
- 问题说明

### 资源问题或资源请求

如果发现资源缺失、无法下载、无法运行，或希望补充某类材料，请提交资源问题：

https://github.com/opentpu/FPGA-Digital-Circuits-and-System-Design/issues/new?template=resource-request.yml

### 邮箱反馈

也可以发送邮件至：

```text
hayj@shanghaitech.edu.cn
hao.sun@seu.edu.cn
jiangweixiong@tongji.edu.cn
```

## 注意事项

- 本仓库会优先发布已经确认可公开的资料。
- 作业答案可以公开，后续会按章节放入 `resources/chapter-XX/answers/`。
- 大型工程文件、视频、教材 PDF 或完整资源包会优先通过 GitHub Releases、学校课程网站或出版社页面发布。
- 请以仓库中的更新日志和公开勘误为准查看最新变更。
