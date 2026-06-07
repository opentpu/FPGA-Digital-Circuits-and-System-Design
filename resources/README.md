# Chapter Resources

本目录按章节组织教材配套资源。每个章节目录采用同一结构：

```text
resources/chapter-XX/
├── slides/
├── code/
├── labs/
├── assignments/
└── answers/
```

目录说明：

| Path | Description |
| --- | --- |
| `slides/` | 演示幻灯片 |
| `code/` | 示例代码 |
| `labs/` | 实验指导材料 |
| `assignments/` | 作业材料 |
| `answers/` | 参考答案 |

跨章节通用资源放入 `resources/common/`。跨章节项目工程放入仓库根目录的 `projects/`。

其中，历史课程资料的归档整理位于 `resources/common/course-archives/`。

## Naming

- 章节目录使用 `chapter-01`、`chapter-02`、...、`chapter-24`。
- 文件名优先使用英文、数字和连字符，避免空格和特殊符号。
- 同一章节内的文件应尽量在文件名中包含章节号或资源类型。
